#include "hBMP.h"
#include <Wire.h>
#include <string.h>
#include <SoftwareSerial.h>
#include <avr/wdt.h>
#include "TinyGPS++.h"



int flag = 0;
int i = 1;

String stringdata1 = "";
String stringdata2 = "";
String stringdata3 = "";
String stringdata4 = "";
String stringdata5 = "";

static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;

SoftwareSerial ss(3 , 4);
SoftwareSerial ubk(10 , 11);

hBMP bmp;
bool _f1, _f2, _f3, _f4;
bool istime1 = true;
bool istime2 = true;
String _B1, _B2;
float h = 0;
float t = 0;
 float f = 0;

void receiveEvent(int howMany) {
  digitalWrite(6, HIGH);
  delay(500);
  digitalWrite(6, LOW);
  Serial.println("recebi algo");
  if(Wire.available()){
    i = Wire.read();    // receive byte as an integer
    Serial.println(i);         // print the integer
  }
}



void requestEvent() {
	if(i == 1){
   Serial.println(stringdata1);
	 Serial.println(F("B1 requisitada."));
   char chardata1[32];
   stringdata1.toCharArray(chardata1, 32);
   Serial.println(chardata1);
	 Wire.write(chardata1);
	}
  wdt_reset();
	Serial.println(F("Enviado"));
  digitalWrite(7, HIGH);  
  delay(200);              
  digitalWrite(7, LOW);       
}

void setup(){
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  Serial.begin(115200);
  ss.begin(GPSBaud);
  ubk.begin(GPSBaud);
  ubk.listen();
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  wdt_enable(WDTO_8S);
	Serial.begin(115200);
	Serial.println("Setup incializado.");


	Wire.begin(94);  //endereço i2c             
	Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

	_f1 = true;
	Serial.println("Setup finalizado.");

	wdt_reset();
}


void loop(){
  static const double Home_LAT = -22.000818, Home_LON = -47.901968;
  unsigned long distanceKmToHome =
    (unsigned long)TinyGPSPlus::distanceBetween(
      gps.location.lat(),
      gps.location.lng(),
      Home_LAT, 
      Home_LON) / 1000;

  double courseToLondon =
    TinyGPSPlus::courseTo(
      gps.location.lat(),
      gps.location.lng(),
      Home_LAT, 
      Home_LON);

  const char *cardinalToLondon = TinyGPSPlus::cardinal(courseToLondon);
  
  if (istime1){
    if (gps.altitude.isUpdated()||gps.hdop.isUpdated() || gps.location.isUpdated() || gps.satellites.isUpdated() || gps.course.isUpdated() || gps.speed.isUpdated()){
      istime1 = false;
      stringdata1 = "";
      stringdata2 = "";
      stringdata1.concat("u");stringdata1.concat(";");    
      stringdata1.concat(gps.hdop.value());stringdata1.concat(";"); //saude do fix
      stringdata1.concat((gps.location.lat()*10000));stringdata1.concat(";");
      stringdata1.concat(gps.location.lng()*10000);stringdata1.concat(";");
      stringdata1.concat(gps.satellites.value());stringdata1.concat(";");
      stringdata1.concat("xxx");
      
      stringdata2.concat(gps.altitude.meters());stringdata2.concat(";");
      stringdata2.concat(gps.course.deg());stringdata2.concat(";");
      stringdata2.concat(gps.speed.kmph());stringdata2.concat(";");
      stringdata2.concat(distanceKmToHome);stringdata2.concat(";");
      stringdata2.concat(courseToLondon);stringdata2.concat(";");
      stringdata2.concat("xxx");

   } 
  }

  smartDelay(1000); 

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring")); // colocar alarme sonoro
  
	//Serial.println(F("Esperando requisicao."));
	wdt_reset();
}



// As funções para o Ublox

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do {
    ubk.listen();
    while (ubk.available() > 0){
      gps.encode(ubk.read());
    }    
  } while (millis() - start < ms);
}



static void printDateTime(TinyGPSDate &d, TinyGPSTime &t) 
{
  if (!d.isValid())
  {
    Serial.print(F("********** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d/%02d/%02d ", d.month(), d.day(), d.year());
    Serial.print(sz);
  }
  
  if (!t.isValid())
  {
    Serial.print(F("******** "));
  }
  else
  {
    char sz[32];
    sprintf(sz, "%02d:%02d:%02d ", t.hour(), t.minute(), t.second());
    Serial.print(sz);
  }

  smartDelay(0);
}



