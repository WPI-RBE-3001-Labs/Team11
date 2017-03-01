/* main.c
 *
 *  Created on: August 20, 2016
 *      Author: Joest
 */


#include "RBELib/RBELib.h"
//For use of abs()
#include <stdlib.h>

//character for receiving serial data
char inchar;
int lowADC = 0;
int highADC=1023;

#define BIT(n) (1 << (n))
void writeMotor0(int pwr);
void writeMotor1(int pwr);
int EncoderCounts(int __chan);
int GetAccelerationH48C(int __axis);

int timerCounts = 0;
int powerVal = 4000;

int setpoint1 = 90;
double kP1 = 150;
double kI1 = .1;
double kD1 = 2;
int pos1;
int lastE1;
int totalE1;

int setpoint2 = 25;
double kP2 = 150;
double kI2 = .1;
double kD2 = 2;
int pos2;
int lastE2;
int totalE2;

int main(void)
{
	  //Enable printf() and setServo()
	  initRBELib();
	  sei();

	  // Write the USARTDebug.c file using the function prototypes in the H file to enable the usart
	  //Set the baud rate of the UART
//	  debugUSARTInit(230400);

	  initSPI();
	  writeMotor0(2000);
	  writeMotor1(0);

	  TCCR0A = BIT(WGM01);
	  TCCR0B = BIT(CS02) | BIT(CS00);
	  OCR0A = 0xB4; //180 ticks for 100 Hz clock

//	  getCharDebug();

	  TIMSK0 = BIT(OCIE0A);

	  while(1){
//		  unsigned char inputs = PINC;
//		  if(!(inputs & BIT(7))){
//			  powerVal = 0;
//		  }else if(!(inputs & BIT(6))){
//			  powerVal = 1517;
//		  }else if(!(inputs & BIT(5))){
//			  powerVal = -1517;
//		  }else if(!(inputs &BIT(4))){
//			  powerVal = 3034;
//		  }
	  }
}

ISR(TIMER0_COMPA_vect){
}

void writeMotor0(int pwr){
	if(pwr>4095)pwr = 4095;
	if(pwr<-4095)pwr = -4095;
	if(pwr>0){
	  setDAC(1,0);
	  setDAC(0,pwr);
	}else{
	  setDAC(0,0);
	  setDAC(1,-pwr);
	}
}

void writeMotor1(int pwr){
	if(pwr>4095)pwr = 4095;
	if(pwr<-4095)pwr = -4095;
	if(pwr>0){
	  setDAC(2,0);
	  setDAC(3,pwr);
	}else{
	  setDAC(3,0);
	  setDAC(2,-pwr);
	}
}
int EncoderCounts(int __chan){
	int val = 0;
	switch(__chan){
	case 0:
		PORTC &= ~(1<<PC5);
		val = spiTransceive(0xFF);
		PORTC |= (1<<PC4) || (1<<PC5);
		return val;
	case 1:
		PORTC &= ~(1<< PC4);
		val = spiTransceive(0xFF);
		PORTC |= (1<<PC4) || (1<<PC5);
		return val;
	default:
		return -1;
	}
}

//int GetAccelerationH48C(int __axis){
//	int val = 0;
//	PORTD &= ~(1<<PD1);
//	spiTransceive(0b0110);
//	switch(__axis){
//	case 0:
//		val = (spiTransceive(0b1000)) <<8;
//		val |= spiTransceive(0x00);
//		break;
//	case 1:
//		val = (spiTransceive(0b1001)) <<8;
//		val |= spiTransceive(0x00);
//		break;
//	case 2:
//			val = (spiTransceive(0b1010)) <<8;
//			val |= spiTransceive(0x00);
//			break;
//	}
//	PORTD |= (1<<PD1);
//	return val;
//}
