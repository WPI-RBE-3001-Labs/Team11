#include "RBELib/RBELib.h"

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
