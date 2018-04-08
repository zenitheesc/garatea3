#include "hBMP.h"

void hBMP::readBMP(void)
{	
	temp = readTemperature();
	alt = readAltitude();
}

float hBMP::getTemp(){
    return temp;
}

float hBMP::getAlt(){
    return alt;
}


