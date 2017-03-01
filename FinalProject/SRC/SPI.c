#include "RBELib/RBELib.h"

void initSPI(){
	DDRB = (1<<DDB5) | (1<<DDB7) | (1<<DDB4);
	DDRD = (1<<DDD4);
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPI2X);
	PORTD = (1<<4);
	PORTB = (1<<DDB4);

	setDAC(4,0);
}

unsigned char spiTransceive(BYTE data){
	SPDR = data;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}
