#include "hBMP.h"
#include "hDHT.h"
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

hDHT _dht(2, DHT22);
hBMP bmp;
bool _f1, _f2, _f3, _f4;
bool istime1 = true;
bool istime2 = true;
String _B1, _B2;

void receiveEvent(int howMany) {
  Serial.println("recebi algo");
  if(Wire.available()){
    i = Wire.read();    // receive byte as an integer
    Serial.println(i);         // print the integer
  }
}



void requestEvent() {
  // Montar função para enviar a string requisitada, não uma após a outra. "Receber um pacote do master do tipo voidReceiveEvent(xx, yy){ -qual_string- = wire.read()}"

	if(i == 1){
   Serial.print(stringdata1);
	 Serial.println(F("B1 requisitada."));
   char chardata1[32];
   stringdata1.toCharArray(chardata1, 32);
	 Wire.write(chardata1);
	}
  wdt_reset();
	if(i == 2){
    istime1 = true;
    Serial.print(stringdata2);
		Serial.println(F("B2 requisitada."));
    char chardata2[32];
    stringdata2.toCharArray(chardata2, 32);
		Wire.write(chardata2);
	}
  if(i == 3){
    
    Serial.print(stringdata3);
    Serial.println(F("B3 requisitada."));
    char chardata3[32];
    stringdata3.toCharArray(chardata3, 32);
    Wire.write(chardata3);
  }
   if(i == 4){
    istime2 = true;
    Serial.print(stringdata4);
    Serial.println(F("B4 requisitada."));
    char chardata4[32];
    stringdata4.toCharArray(chardata4, 32);
    Wire.write(chardata4);
  }
  if(i == 5){
    Serial.print(stringdata5);
    Serial.println(F("B5 requisitada."));
    char chardata5[32];
    stringdata5.toCharArray(chardata5, 32);
    Wire.write(chardata5);
  }
	
	if(_f1){
		_f1 = false;
    _f2 = true;
    _f3 = false;
    _f4 = false;
	} else if(_f2){
    _f1 = false;
    _f2 = false;
    _f3 = true;
    _f4 = false;
	} else if(_f3){
    _f1 = false;
    _f2 = false;
    _f3 = false;
    _f4 = true;
	} else if(_f4){
    _f1 = true;
    _f2 = false;
    _f3 = false;
    _f4 = false;
 }

	Serial.println(F("Enviado"));
  digitalWrite(7, HIGH);  
  delay(200);              
  digitalWrite(7, LOW);       
}



void setup(){
  pinMode(7, OUTPUT);
  Serial.begin(115200);
  ss.begin(GPSBaud);
  ubk.begin(GPSBaud);
  ubk.listen();
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  wdt_enable(WDTO_8S);
	Serial.begin(115200);
	Serial.println("Setup incializado.");

	_dht.begin();
    Serial.println(F("DHT incializado."));

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

  //stringdata1.concat(gps.time);stringdata1.concat(";"); // arrumar 
  if (flag == 0 && istime1){
    if (gps.altitude.isUpdated()||gps.hdop.isUpdated() || gps.location.isUpdated() || gps.satellites.isUpdated() || gps.course.isUpdated() || gps.speed.isUpdated()){
      istime1 = false;
      stringdata1 = "";
      stringdata2 = "";
      stringdata5 = "";
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
      //stringdata2.concat(cardinalToLondon);stringdata2.concat(";");
      //stringdata2.concat(TinyGPSPlus::cardinal(gps.course.value()));stringdata2.concat("\n");

      stringdata5.concat(_dht.getTemp());stringdata5.concat(";");
      stringdata5.concat(_dht.getHumd());stringdata5.concat(";");
      stringdata5.concat("xxx");
      

  
      flag = 1;
   } 
  }

   if (flag == 1 && istime2){ 
    if (gps.altitude.isUpdated()||gps.hdop.isUpdated() || gps.location.isUpdated() || gps.satellites.isUpdated() || gps.course.isUpdated() || gps.speed.isUpdated()){
      istime2 = false;
      stringdata3 = "";
      stringdata4 = "";
      stringdata5 = "";
      stringdata3.concat("a");stringdata3.concat(";");    
      stringdata3.concat(gps.hdop.value());stringdata3.concat(";"); //saude do fix
      stringdata3.concat((gps.location.lat()*10000));stringdata3.concat(";");
      stringdata3.concat(gps.location.lng()*10000);stringdata3.concat(";");
      stringdata3.concat(gps.satellites.value());stringdata3.concat(";");
      stringdata3.concat("xxx");
      
      
      stringdata4.concat(gps.altitude.meters());stringdata4.concat(";");
      stringdata4.concat(gps.course.deg());stringdata4.concat(";");
      stringdata4.concat(gps.speed.kmph());stringdata4.concat(";");
      stringdata4.concat(distanceKmToHome);stringdata4.concat(";");
      stringdata4.concat(courseToLondon);stringdata4.concat(";");
      stringdata4.concat("xxx");
     
      //stringdata4.concat(cardinalToLondon);stringdata4.concat(";");
      //stringdata4.concat(TinyGPSPlus::cardinal(gps.course.value()));stringdata4.concat("\n");

      stringdata5.concat(_dht.getTemp());stringdata5.concat(";");
      stringdata5.concat(_dht.getHumd());stringdata5.concat(";");
      stringdata5.concat("xxx");
      

      
      flag = 0;
   } 
  }
  
  

  // gps.location.isValid() retorna true se tem fix - Seria legal implementar
  smartDelay(1000); // Mantem a comunicação ativa até que não tenha mais pacotes. Não alterar

  if (millis() > 5000 && gps.charsProcessed() < 10)
    Serial.println(F("No GPS data received: check wiring")); // colocar alarme sonoro
  
	//Serial.println(F("Esperando requisicao."));
	wdt_reset();
}



// As funções para o Ublox

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    if (flag == 0){
    ubk.listen();
    while (ubk.available() > 0){
      gps.encode(ubk.read());
    } 
  }else{
    ss.listen();
    while (ss.available() > 0){
      gps.encode(ss.read());
    }
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



