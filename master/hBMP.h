#include <Adafruit_BMP085.h>
#include <Wire.h>  

class hBMP : public Adafruit_BMP085{
private:
	float temp;
	float alt;

public:
	hBMP() : Adafruit_BMP085() {};
	void readBMP();
	float getTemp();
	float getAlt();
};

