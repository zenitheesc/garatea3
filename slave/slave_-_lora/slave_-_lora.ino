#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <avr/wdt.h>
#include <LoRa.h>
#include <Adafruit_BMP085.h> 
#include <stdlib.h>
#include <string.h>
#include <util/crc16.h>
#include <Servo.h>

Servo myservo;
int inicio = 0;
int fim = 90;
int pos = 0;
int last_pos = inicio;
#define RADIOPIN 5
Adafruit_BMP085 bmp;
int counter = 0;
int i = 1;
String stringdata = "";
char _B[20];
char c;
int x = 1;
bool stp = true;
long lastalt = 0;
long lastmillis = 0;
int vel = 0;
float ref_alt = 0;
File myFile;

void radioSetup(){
	pinMode(RADIOPIN, OUTPUT);
	pinMode(A1, OUTPUT);
	//digitalWrite(A1, HIGH); 
}

void setup(){  
 Serial.begin(115200);
 myservo.attach(9);
 Wire.begin();
 
 if (!bmp.begin()){
    Serial.println("Sensor nao encontrado !!");
  }
 
 LoRa.setPins(7, A0, 2);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSignalBandwidth(125E3);
  LoRa.setSpreadingFactor(11);
  //LoRa.explicitHeaderMode();
  
	
	if (!SD.begin(4)) {
    	Serial.println("Falha no SD.");
    	delay(1000);
  	}
  	Serial.println("SD inicializado.");

  	_B[0] = '\0';

  	radioSetup();
  	wdt_enable(WDTO_8S);
}


void save_data(){
	int time = millis();
	wdt_reset(); 
	Serial.println(F("Save."));
	myFile = SD.open("log.txt", FILE_WRITE);
	if(myFile){
		myFile.print(time);
		myFile.print(';');
		myFile.println(stringdata);
		myFile.close();
		Serial.println(F("Gravado."));
	}
}

void receive_data(){
  Wire.beginTransmission(94);
  Wire.write(i);
  Wire.endTransmission();
	wdt_reset(); 
	Serial.println(F("Receive."));  
  delay (500);
	Wire.requestFrom(94, 32);   
 	while (Wire.available()) { 
    c = Wire.read();
    if (c != 'x' && stp == true ){
      stringdata.concat(c);  
    }else{
      stp = false;
    }
  }
  stp = true;
}

void rtty_txbit (int bit) {
  if (bit) {
    digitalWrite(5, HIGH);
  }
  else {
    digitalWrite(5, LOW);
  }

  delayMicroseconds(10000);
  delayMicroseconds(10150); 
}

void rtty_txbyte (char c) {
	int i;
	rtty_txbit (0);
	for (i = 0; i < 7; i++) { // Change this here 7 or 8 for ASCII-7 / ASCII-8
		if (c & 1) rtty_txbit(1);
		else rtty_txbit(0);
		c = c >> 1;
	}
	rtty_txbit (1); // Stop bit
	rtty_txbit (1); // Stop bit
	wdt_reset();
}

void rtty_txstring (char * string) {
	char c;
	c = *string++;
	while ( c != '\0') {
		rtty_txbyte (c);
		c = *string++;
	}
}

uint16_t gps_CRC16_checksum (char *string) {
  size_t i;
  uint16_t crc;
  uint8_t c;
  crc = 0xFFFF;

  for (i = 2; i < strlen(string); i++) {
    c = string[i];
    crc = _crc_xmodem_update (crc, c);
  }
  return crc;
}

void loop(){
	receive_data();
	wdt_reset(); 
	//rtty_txstring("hi");
	wdt_reset();
	delay(1000);
  i++;
  if (i > 5 ){
    Serial.print("Sending packet: ");
    Serial.println(counter);
    LoRa.beginPacket();
    stringdata.concat(bmp.readAltitude()); stringdata.concat(";");
    stringdata.concat(bmp.readPressure()); stringdata.concat(";");
    stringdata.concat(bmp.readTemperature()); stringdata.concat(";");
    stringdata.concat(millis()/1000); stringdata.concat(";");
    vertical_speed();
    stringdata.concat(vel);
    LoRa.print(stringdata);
    LoRa.print(counter);
    LoRa.endPacket();
    counter++;
    Serial.println(stringdata);
    save_data();
    stringdata = "";
    i = 1;

    lastalt = bmp.readAltitude();
    lastmillis = millis();
  }
}

void vertical_speed(){
  vel = (bmp.readAltitude() - lastalt)*1000/(millis() - lastmillis);
}

float referencia_altitude(){
  ref_alt = 0;
  for (int x = 0; i == 5; i++){
    ref_alt = ref_alt + bmp.readAltitude();
    delay(10);
  }
  ref_alt = ref_alt/5;
  return ref_alt;
}

void hora_de_abrir(float h){
  if (h >= 100000 && h <= 28000){
    for (pos = inicio; pos <= fim; pos++){
      myservo.write(pos);              
      delay(15);    
      last_pos = pos;                   
    } 
  }else{
    for (pos = last_pos; pos >= inicio; pos++){
      myservo.write(pos);              
      delay(15);                       
    }
    last_pos = inicio;
  }
}



