#ifndef hGPS_h
#define hGPS_h

#include <Adafruit_GPS.h>
//#include <HardwareSerial.h>
#include <SoftwareSerial.h>

class hGPS : public Adafruit_GPS {
private:	
	String NMEA1;
	String NMEA2;
	char c;
 	
public:
	hGPS(SoftwareSerial *as) : Adafruit_GPS(as) {};
	void start_GPS();
	void readGPS();
	void clearGPS();
	void hparse();
	double get_latitude();
	double get_longitude();
	double get_speed();
	double get_altitude();
	float get_fix();
};

#endif
