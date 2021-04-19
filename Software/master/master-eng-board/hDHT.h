#include <Arduino.h>
#include "DHT.h"

class hDHT : public DHT{
private:
	float Temp;
	float Humd;
	float HIdx;

public:
	hDHT(uint8_t pin, uint8_t type) : DHT(pin, type) {};
	void readDHT();
	float getTemp();
	float getHumd();
	float getHIdx();
};

