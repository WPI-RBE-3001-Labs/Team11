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

unsigned long timer = 0;
int polling = 0;
int tim2count = 0;
int toggle = 0; //used to generate pwm signal
int state = 0;
unsigned int steps = 36000; //number used as TOP for timer1, (18432000/512)/freq
int freq = 1;
unsigned int posSteps = 0;
int main(void)
{
	  //Enable printf() and setServo()
	  initRBELib();
	  DDRA = 0x00;
	  DDRB = 0xFF;
	  DDRC = 0x00;
	  sei();

	  // Write the USARTDebug.c file using the function prototypes in the H file to enable the usart
	  //Set the baud rate of the UART
	  debugUSARTInit(230400);
	  ADMUX = (1<<REFS0) | 0x4;
	  ADCSRA = (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) | (1<<ADEN);

	  TCCR0B = (1<<CS00) | (1<<CS02);

	  TCCR1B = (1<<CS12) | BIT(WGM13); //set clock 1 in phase and freq correct mode and clock divider to 256
	  TCCR1A = BIT(WGM10);

	  TCCR2B = BIT(CS22) | BIT(CS21);

	  OCR1AH = steps>>8;  //set the TOP for clock 1 to 36000d for 1 Hz clk
	  OCR1AL = steps & 0xF;

	  inchar = getCharDebug(); //wait for any serial message
	  TIMSK0 = (1<<TOIE0); //enable clk0 interrupt
	  TIMSK1 = (1<<OCIE1B) | BIT(OCIE1A) | BIT(TOIE1); //enable clk1 interrupts for changing toggle and updating signal

	  while(1){
		  if(!polling){
			  ADCSRA = (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) | (1<<ADEN) | (1<<ADSC); //start conversion
			  while(ADCSRA &(1<<ADSC)); //check for conversion finish
			  int val = 0;
			  val += ADCL;
			  val += (ADCH << 8);
			  float percent = val/1023.0;
			  int mV = (percent)*5000;
			  int deg =(percent)*300;
//			  printf("%lu, %i, %i, %i\r\n",timer, val,mV,deg);
			  printf("%i %%, %i Hz, %u, %i\r\n",(int)(percent*100),freq,state,val);


			  posSteps = (percent)*steps;
			  //assure that the signal toggles every output by keeping OCR1B from being TOP or BOTTOM
			  if(posSteps == 0)posSteps++;
			  if(posSteps == steps)posSteps--;

			  OCR1BH = posSteps>>8; //set the OCR1B compare register to the TOP value scaled by the pot to cause output change
			  OCR1BL = posSteps & 0xF;

			  unsigned char inputs = PINC;
			  if(!(inputs & BIT(7))){
				  TCCR1B = (1<<CS12) | BIT(WGM13);
				  steps = 36000;
				  freq = 1;
				  OCR1AH = steps>>8;  //set the TOP for clock 1 to 36000d for 1 Hz clk
				  OCR1AL = steps & 0xF;
			  }else if(!(inputs & BIT(6))){
				  TCCR1B = (1<<CS12) | BIT(WGM13);
				  steps = 1800;
				  freq = 20;
				  OCR1AH = steps>>8;  //set the TOP for clock 1 to 1800d for 20 Hz clk
				  OCR1AL = steps & 0xF;
			  }else if(!(inputs & BIT(5))){
				  TCCR1B = BIT(CS11) | BIT(CS10) | BIT(WGM13);
				  steps = 1440;
				  freq = 100;
				  OCR1AH = steps>>8;  //set the TOP for clock 1 to 1440d for 100 Hz clk
				  OCR1AL = steps & 0xF;
			  }else if(!(inputs &BIT(4))){
				  polling  = 1;
				  ADMUX = (1<<REFS0) | 0x5;
				  TIMSK2 = BIT(TOIE2);
			  }
		  }
	  }
}

ISR(TIMER0_OVF_vect){
	timer++;
}

ISR(TIMER1_OVF_vect){
	toggle = 1;
}

ISR(TIMER1_COMPA_vect){
	toggle = 0;
}

ISR(TIMER1_COMPB_vect){
	if(toggle){
		PORTB = !BIT(0) & BIT(0);
		state = 0;
	}else{
		PORTB = BIT(0);
		state = 1;
	}
}

ISR(TIMER2_OVF_vect){
	tim2count++;
	PORTB = BIT(0);
	ADCSRA = (1<<ADPS0) | (1<<ADPS1) | (1<<ADPS2) | (1<<ADEN) | (1<<ADSC); //start conversion
	while(ADCSRA &(1<<ADSC)); //check for conversion finish
	int val = 0;
	val += ADCL;
	val += (ADCH << 8);
//	printf("%i, %i\r\n",tim2count,val);
	PORTB = !BIT(0) & BIT(0);
	if(tim2count > 225){
		tim2count = 0;
		polling = 0;
		TIMSK2 = 0x00; //turn off timer interrupt
	}
}
