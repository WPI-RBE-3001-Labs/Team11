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
