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

int timerCounts = 0;

int setpoint = 60;
double kP;
double kI;
double kD;
int pos;
int lastE;
int totalE;

int main(void)
{
	  //Enable printf() and setServo()
	  initRBELib();
	  sei();

	  // Write the USARTDebug.c file using the function prototypes in the H file to enable the usart
	  //Set the baud rate of the UART
	  debugUSARTInit(230400);
	  //Part 1 code
//	  ADMUX = (1<<REFS0) | 0x3;
//	  ADCSRA = (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) | (1<<ADEN);

	  //Part 2 code
	  initSPI();
//	  int value = 0;
//	  int direction = 1;
	  writeMotor0(0);
	  ADMUX = (1<<REFS0);
	  ADCSRA = (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) | (1<<ADEN);

	  TCCR0A = BIT(WGM01);
	  TCCR0B = BIT(CS02) | BIT(CS00);
	  OCR0A = 0xB4; //180 ticks for 100 Hz clock

	  //Part 3 code

	  getCharDebug();

	  TIMSK0 = BIT(OCIE0A);

	  while(1){
		  //Part 1 code
//		  ADCSRA = (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) | (1<<ADEN) | (1<<ADSC); //start conversion
//		  while(ADCSRA &(1<<ADSC)); //check for conversion finish
//		  int val = 0;
//		  val += ADCL;
//		  val += (ADCH << 8);
//		  float percent = val/1023.0;
//		  int mV = percent*5000;
//		  int degrees = percent*240;
//		  printf("%i, %i, %i\r\n",val,mV,degrees);

		  //Part 2 code
//		  setDAC(0,value);
//		  setDAC(1,4095-value);
//		  if(direction) value+=5;
//		  if(!direction) value-=5;
//		  if(value > 4094) direction = 0;
//		  if(value<1)direction = 1;
//		  printf("%i, %i\r\n",direction, value);

		  //part 3 code
//		  if(direction) value+=5;
//		  if(!direction) value-=5;
//		  if(value > 4090) direction = 0;
//		  if(value< -4090)direction = 1;
//		  writeMotor0(value);
//		  ADCSRA = (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) | (1<<ADEN) | (1<<ADSC); //start conversion
//		  while(ADCSRA &(1<<ADSC)); //check for conversion finish
//		  int val = 0;
//		  val += ADCL;
//		  val += (ADCH << 8);
//		  printf("%i\r\n",val);
	  }
}

ISR(TIMER0_COMPA_vect){
	//read angle pot
	ADMUX = (1<<REFS0) | 0x2;
	ADCSRA = (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) | (1<<ADEN) | (1<<ADSC); //start conversion
	while(ADCSRA &(1<<ADSC)); //check for conversion finish
	pos = 0;
	pos += ADCL;
	pos += (ADCH<< 8);
	pos = (pos/1023.0)*240;
	int error = pos-setpoint;

	//read kP pot
	ADMUX = (1<<REFS0) | 0x4;
	ADCSRA = (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) | (1<<ADEN) | (1<<ADSC); //start conversion
	while(ADCSRA &(1<<ADSC)); //check for conversion finish
	kP = 0;
	kP += ADCL;
	kP += (ADCH << 8);
	kP = (kP/1023.0)*500;

	//read kI pot
	ADMUX = (1<<REFS0) | 0x5;
	ADCSRA = (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) | (1<<ADEN) | (1<<ADSC); //start conversion
	while(ADCSRA &(1<<ADSC)); //check for conversion finish
	kI = 0;
	kI += ADCL;
	kI += (ADCH << 8);
	kI = (kI/1023.0)*1;

	//read kD pot
	ADMUX = (1<<REFS0) | 0x6;
	ADCSRA = (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) | (1<<ADEN) | (1<<ADSC); //start conversion
	while(ADCSRA &(1<<ADSC)); //check for conversion finish
	kD = 0;
	kD += ADCL;
	kD += (ADCH << 8);
	kD = (kD/1023.0)*10;

	int driveVal = kP*error + kI*totalE + kD*(error-lastE);

	writeMotor0(driveVal);


	ADMUX = (1<<REFS0);
	ADCSRA = (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) | (1<<ADEN) | (1<<ADSC); //start conversion
	while(ADCSRA &(1<<ADSC)); //check for conversion finish
	int current = 0;
	current += ADCL;
	current += (ADCH << 8);

	printf("%i, %i, %i, %i, %f, %f, %f\r\n",driveVal,setpoint,pos,current,kP,kI,kD);

	totalE+=error;
	lastE = error;

	timerCounts++;
	if(timerCounts>500){
//		setpoint = 150;
//		timerCounts = 0;   //make the arm repeat oscillation
	}else if(timerCounts>250){
		setpoint = 120;
	}
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
