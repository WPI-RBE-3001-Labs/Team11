/*
 * USART.c
 *
 *  Created on: Jan 26, 2015
 *      Author: joest
 */


#include "RBELib/RBELib.h"
//#include <avr/io.h>

void debugUSARTInit(unsigned long baudrate){
  baudrate = ((18432000/16)/baudrate)-1;
  UBRR1H = (unsigned char) (baudrate>>8);
  UBRR1L = (unsigned char) baudrate;

  UCSR1B = (1<<RXEN1) | (1<<TXEN1);
  
  UCSR1C = (1<<USBS1) | (3<<UCSZ10);
}

void putCharDebug(char byteToSend){
 while ( !(UCSR1A & (1<<UDRE1)));
 UDR1  = byteToSend;
}

unsigned char getCharDebug(void) {
   while (!(UCSR1A & (1<<RXC1)));
   return UDR1;
}

void initSPI(){
	DDRB = (1<<DDB5) | (1<<DDB7) | (1<<DDB4);
	DDRD = (1<<DDD4);
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPI2X);
	PORTD = (1<<4);
	PORTB = (1<<DDB4);

	setDAC(4,0);

}

unsigned char spiTransceive (BYTE data){
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}

void setDAC(int DACn, int SPIVal){
	PORTD = 0;
	BYTE control = (0x2 << 4) | (DACn);
	spiTransceive(control);
	BYTE low = SPIVal & 0x000F;
	BYTE high = SPIVal >> 4;
	spiTransceive(high);
	spiTransceive(low);
	PORTD = (1<<4);
}
