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

#define BIT(n) (1 << (n))

void writeMotor0(int pwr);
void writeMotor1(int pwr);

volatile long timerCount = 0;
char str[10];
/*
 * 0: waiting for start bit
 * 1: receiving command code
 * 2-10: receiving packets
 */
volatile int readState = 0;
volatile int readData = 0;
/*
 * 1:drive top link
 * 2:drive bottom link
 * 3:drive gripper
 */
volatile int command = 0;

volatile int driveVal0;
volatile int driveVal1;
volatile int gripperDrive= 0;
volatile int beltDrive = 90;


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
	  //wait to start
	  setServo(1,90);
	  inchar = getCharDebug();

	  initTimer(2,CTC,180); //100 Hz Timer
	  UCSR1B = (1<<RXEN1) | (1<<TXEN1) | (1<<RXCIE1);
	  while(1){
		  writeMotor0(driveVal0);
		  writeMotor1(driveVal1);
		  setServo(0,gripperDrive);
		  setServo(1,beltDrive);
	  }
}

ISR(TIMER2_COMPA_vect){
	timerCount++;

	int lowerPot = ((double)getADC(3)/1024.0)*240-58;
	int upperPot = ((double)getADC(2)/1024.0)*240-150;
	int firstIR = getADC(5);
	int secondIR = getADC(4);
	int upperCurrent = getADC(0);
	int lowerCurrent = getADC(1);

	printf("%i,%i,%i,%i,%i,%i\r\n",lowerPot,upperPot,firstIR,secondIR,lowerCurrent,upperCurrent);
}

ISR(USART1_RX_vect){
	unsigned char val = getCharDebug();
	switch(readState){
	case 0:
		if(val=='B'){
			readState++;
		}
		break;
	case 1:
		command = val;
		readState++;
		break;
	default:
		if(val=='S'){
			switch(command){
			case 10:
				driveVal0 = 0;
				driveVal1 = 0;
				break;
			case 1:
				driveVal0 = readData;
				break;
			case 2:
				driveVal1 = readData;
				break;
			case 3:
				if(gripperDrive){
					gripperDrive = 0;
				}else{
					gripperDrive = 255;
				}
				break;
			case 4:
				if(beltDrive!=90){
					beltDrive = 90;
				}else{
					beltDrive = 255;
				}
			}
			command = 0;
			readState = 0;
			readData = 0;
		}else{
			readData +=(val<<(8*(readState-2)));
			readState++;
		}
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
