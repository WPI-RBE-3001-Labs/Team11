#include "RBELib/RBELib.h"

void initTimer(int timer, int mode, unsigned int comp){
	switch(timer){
	case 0:
		TCCR0A = (mode==NORMAL)?0:(1<< WGM01);
		TCCR0B = (1<<CS02) | (1<<CS00);
		OCR0A = comp;
		TIMSK0 = (mode==NORMAL)?(1<<TOIE0):(1<<OCIE0A);
		break;
	case 1:
		TCCR1A = 0;
		TCCR1B = ((mode==NORMAL)?0:(1<<WGM12)) | (1<<CS12) | (1<<CS10);
		OCR1A = comp;
		TIMSK1 = (mode==NORMAL)?(1<<TOIE1):(1<<OCIE1A);
		break;
	case 2:
		TCCR2A = (mode==NORMAL)?0:(1<<WGM21);
		TCCR2B = (1<<CS22) | (1<<CS21) | (1<<CS20);
		OCR2A = comp;
		TIMSK2 = (mode==NORMAL)?(1<<TOIE2):(1<<OCIE2A);
		break;
	}
}

void setCompValue(unsigned char timer, unsigned short int comp){
	switch(timer){
		case 0:
			TCNT0 = 0x00;
			OCR0A = comp;
			break;
		case 1:
			TCNT1H = 0x00;
			TCNT1L = 0x00;
			OCR1A = comp;
			break;
		case 2:
			TCNT2 = 0x00;
			OCR2A = comp;
			break;
		}
}
