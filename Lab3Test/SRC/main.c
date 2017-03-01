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
unsigned long EncoderCounts(int __chan);

int timerCounts = 0;

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

int power = 0;

int main(void)
{
	  //Enable printf() and setServo()
	  initRBELib();
	  sei();

	  // Write the USARTDebug.c file using the function prototypes in the H file to enable the usart
	  //Set the baud rate of the UART
	  debugUSARTInit(230400);

	  initSPI();
	  writeMotor0(0);
	  writeMotor1(0);

	  TCCR0A = BIT(WGM01);
	  TCCR0B = BIT(CS02) | BIT(CS00);
	  OCR0A = 0xB4; //180 ticks for 100 Hz clock


	  DDRA = 0x00;
	  PORTA = 0xFF;

	  DDRC = (1<<PC4)|(1<<PC5);
	  PORTC = (1<<PC4)|(1<<PC5);
	  PORTC = ~(1<<PC5);
	  spiTransceive(0x20);
	  PORTC = (1<<PC4)|(1<<PC5);
	  PORTC = ~((1<<PC5));
	  spiTransceive(0x88);
	  spiTransceive(0x01);
	  PORTC = (1<<PC4)|(1<<PC5);
//	  PORTC = ~((1<<PC5));
//	  spiTransceive(0x90);
//	  spiTransceive(0x02);
//	  PORTC = (1<<PC4)|(1<<PC5);


	  getCharDebug();

	  TIMSK0 = BIT(OCIE0A);


	  while(1){
	  }
}

ISR(TIMER0_COMPA_vect){
	//Button Control
	unsigned char inputs = PINA;
	if(~inputs & BIT(7) ){
	  power = 0;
	}else if(~inputs & BIT(6)){
	  power = 1024;
	}else if(~inputs & BIT(5)){
	  power = -1024;
	}else if(~inputs & BIT(4)){
	  power = 4095;
	}
	writeMotor0(power);
	unsigned long x = EncoderCounts(0);
	printf("%lu\r\n",x);
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

unsigned long EncoderCounts(int __chan){
	unsigned long val = 0;
	switch(__chan){
	case 0:
		PORTC = ~(1<<PC5);
		spiTransceive(0x60);
		val = spiTransceive(0x00) << 24;
		val |= spiTransceive(0x00) << 16;
		val |= spiTransceive(0x00) << 8;
		val |= spiTransceive(0x00);
		PORTC = (1<<PC4) || (1<<PC5);
		return val;
	case 1:
		PORTC = ~(1<< PC4);
		spiTransceive(0x60);
		val = spiTransceive(0x00) << 24;
		val |= spiTransceive(0x00) << 16;
		val |= spiTransceive(0x00) << 8;
		val |= spiTransceive(0x00);
		PORTC = (1<<PC4) || (1<<PC5);
		return val;
	default:
		return 10;
	}
}
