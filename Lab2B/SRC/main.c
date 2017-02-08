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
//	  int value = 0;
//	  int direction = 1;
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

//	//Button Control
//	unsigned char inputs = PINC;
//	if(~inputs & BIT(7)){
//	  setpoint1 = 0;
//	  setpoint2 = 90;
//	}else if(~inputs & BIT(6)){
//	  setpoint1 = 45;
//	  setpoint2 = 45;
//	}else if(~inputs & BIT(5)){
//	  setpoint1 = 90;
//	  setpoint2 = 0;
//	}else if(~inputs & BIT(4)){
//	  setpoint1 = -30;
//	  setpoint2 = 150;
//	}

	//read top link angle
	ADMUX = (1<<REFS0) | 0x2;
	ADCSRA = (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) | (1<<ADEN) | (1<<ADSC); //start conversion
	while(ADCSRA &(1<<ADSC)); //check for conversion finish
	pos1 = 0;
	pos1 += ADCL;
	pos1 += (ADCH<< 8);
	pos1 = (pos1/1023.0)*240;
	pos1 = pos1 - 150;
	int error1 = pos1-setpoint1;

	//read bottom link angle
	ADMUX = (1<<REFS0) | 0x3;
	ADCSRA = (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) | (1<<ADEN) | (1<<ADSC); //start conversion
	while(ADCSRA &(1<<ADSC)); //check for conversion finish
	pos2 = 0;
	pos2 += ADCL;
	pos2 += (ADCH<< 8);
	pos2 = (pos2/1023.0)*240;
	pos2 = pos2-58;
	int error2 = pos2-setpoint2;

	int driveVal1 = kP1*error1 + kI1*totalE1 - kD1*(error1-lastE1);
	int driveVal2 = kP2*error2 + kI2*totalE2 - kD2*(error2-lastE2);

	writeMotor0(driveVal1);
	writeMotor1(driveVal2);

	printf("%i,%i,%hhu,%i\r\n",pos2,pos1,setpoint2,setpoint1);


	totalE1+=error1;
	lastE1 = error1;
	totalE2+=error2;
	lastE2 = error2;

//	//Vertex triangle
//	if(timerCounts < 500 ){
//		setpoint1 = 90;
//		setpoint2 = 25;
//	}else if(timerCounts< 1000){
//		setpoint1 = 12;
//		setpoint2 = 62;
//	}else if(timerCounts < 1500){
//		setpoint1 = -35;
//		setpoint2 = 55;
//	}else if(timerCounts < 2000){
//		setpoint1 = 90;
//		setpoint2 = 25;
//	}

	//Between points triangle
	if(timerCounts < 200 ){
		setpoint1 = -97;
		setpoint2 = 83;
	}else if(timerCounts< 400){
		setpoint1 = -72;
		setpoint2 = 78;
	}else if(timerCounts < 600){
		setpoint1 = -40;
		setpoint2 = 69;
	}else if(timerCounts < 800){
		setpoint1 = -6;
		setpoint2 = 56;
	}else if(timerCounts < 1000 ){
		setpoint1 = -10;
		setpoint2 = 47;
	}else if(timerCounts< 1200){
		setpoint1 = -11;
		setpoint2 = 42;
	}else if(timerCounts < 1400){
		setpoint1 = -12;
		setpoint2 = 35;
	}else if(timerCounts < 1600){
		setpoint1 = -48;
		setpoint2 = 53;
	}else if(timerCounts < 1800 ){
		setpoint1 = -72;
		setpoint2 = 67;
	}else if(timerCounts< 2000){
		setpoint1 = -97;
		setpoint2 = 83;
	}
	timerCounts++;
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
