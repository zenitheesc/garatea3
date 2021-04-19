#include "hGPS.h"

void hGPS::start_GPS(){
  this->begin(9600); //Turn on GPS at 9600 baud
  sendCommand("$PGCMD,33,0*6D");  //Turn off antenna update nuisance data
  sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); //Request RMC and GGA Sentences only
  sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); //Set update rate to 1 hz
  delay(1000);
}

void hGPS::readGPS(){
  clearGPS();
  while(!newNMEAreceived()) { //Loop until you have a good NMEA sentence
    //Serial.println("1");
    c=read();
  }
  parse(lastNMEA()); //Parse that last good NMEA sentence
  NMEA1=lastNMEA();
  
   while(!newNMEAreceived()) { //Loop until you have a good NMEA sentence
        //Serial.println("2");

    c=read();
  }
  parse(lastNMEA()); //Parse that last good NMEA sentence
  NMEA2=lastNMEA();
  
  //Serial.println(NMEA1);
  //Serial.println(NMEA2);
  //Serial.println("");
}

void hGPS::clearGPS() {  //Clear old and corrupt data from serial port
  int it = 0;

  while(!newNMEAreceived()) { //Loop until you have a good NMEA sentence
       // Serial.println("3");

    c=read();
  }
  parse(lastNMEA()); //Parse that last good NMEA sentence
  
  while(!newNMEAreceived()) { //Loop until you have a good NMEA sentence
       // Serial.println("4");

    c=read();
  }
  parse(lastNMEA()); //Parse that last good NMEA sentence
   while(!newNMEAreceived()) { //Loop until you have a good NMEA sentence
        //Serial.println("5");

    c=read();
  }
  parse(lastNMEA()); //Parse that last good NMEA sentence
  
}


void hGPS::hparse(){
	if(fix==1) { //Only save data if we have a fix
    //Serial.println(NMEA1); //Write first NMEA to SD card
    //Serial.println(NMEA2); //Write Second NMEA to SD card
    Serial.print(latitude,4); //Write measured latitude to file
    Serial.print(lat); //Which hemisphere N or S
    Serial.print(",");
    Serial.print(longitude,4); //Write measured longitude to file
    Serial.print(lon); //Which Hemisphere E or W
    Serial.print(",");
    Serial.println(altitude);
  }
}

double hGPS::get_latitude(){
	return latitude;
}
double hGPS::get_longitude(){
	return longitude;
}
double hGPS::get_speed(){
	speed = speed * 0.51444; // Conversao de knots para m/s
	return speed;
}
double hGPS::get_altitude(){
	return altitude;
}

float hGPS::get_fix(){
  return fix;
}
