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
	  ADMUX = (1<<REFS0);
	  ADCSRA = (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) | (1<<ADEN);

	  TCCR0A = BIT(WGM01);
	  TCCR0B = BIT(CS02) | BIT(CS00);
	  OCR0A = 0xB4; //180 ticks for 100 Hz clock


	  DDRC = 0x00;
	  PORTC = 0xFF;


	  getCharDebug();

	  TIMSK0 = BIT(OCIE0A);

	  while(1);
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
