#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <avr/wdt.h>
#include "LoRa.h"
#include "Adafruit_BMP085.h" 
#include <stdlib.h>
#include <string.h>
#include <util/crc16.h>
#include <avr/io.h>

#include "eeprom.h"

//#include <Servo.h>

//Servo myservo;
//int inicio = 0;
//int fim = 90;
//int pos = 0;
//int last_pos = inicio;
#define RADIOPIN 5
Adafruit_BMP085 bmp;
int counter = 0;
int i = 1;
String stringdata = "";
String statustampa = "fechado";
char _B[20];
char c;
int x = 1;
bool stp = true;
long lastalt = 0;
long lastmillis = 0;
float vel = 0;
float ref_alt = 0;
bool status_sd = true;
char cstr[150];
uint8_t eeprom_skip_counter = 0;

#define red 3
#define green 6
#define blue 10
#define eeprom_skip 10

File myFile;

void setup(){  
  Serial.begin(9600);
  //myservo.attach(9);
  pinMode(red, OUTPUT);
  Wire.begin();
  if (!bmp.begin()){
    Serial.println("Sensor nao encontrado !!");
  }
  LoRa.setPins(7, A0, 2);
  if (!LoRa.begin(433.123E6)) {
    Serial.println("Starting LoRa failed!");
  }
  LoRa.setTxPower(20, 1);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setSpreadingFactor(11);
  LoRa.enableCrc();
	if (!SD.begin(4)) {
    	Serial.println("Falha no SD.");
      digitalWrite(red, HIGH);
      status_sd = false;
    	delay(1000);
  	}
  else{ 
      Serial.println("SD inicializado.");
  }
  eeprom.init(0);
  DDRB = DDRB & 0b11111110;
  wdt_enable(WDTO_8S);
  ref_alt = bmp.readAltitude();
}


void save_data(){
  digitalWrite(red, LOW);
	wdt_reset(); 
	Serial.println(F("Save."));
	myFile = SD.open("log.txt", FILE_WRITE);
	if(myFile){
		myFile.println(stringdata);
		myFile.close();
		Serial.println(F("Gravado."));
	}else{
    digitalWrite(red, HIGH); 
  }
}

void receive_data(){
  digitalWrite(blue, HIGH);
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
  digitalWrite(blue, LOW);
}

void loop(){
	receive_data();
	wdt_reset();
  if( 0x00 == ( 0x01 & PINB) ){
    Serial.println("ENTROU NESSA PORRA");
    eeprom.mempos.full = reserved;
    eeprom.err = 0x00;
    eeprom.resetlogs();
   } 
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
    float verticalspeed = vertical_speed();
    //hora_de_abrir(verticalspeed, referencia_altitude());
    stringdata.concat(verticalspeed);
    //stringdata.concat(statustampa);
    lastalt = referencia_altitude();
    lastmillis = millis();
    if (status_sd){
        stringdata.concat(";1;");
    }else{
        stringdata.concat(";0;");
    }
    wdt_reset();
    LoRa.print(stringdata);
    LoRa.print(counter);
    LoRa.endPacket();
    counter++;
    //Serial.println(stringdata);
    save_data();
    //eeprom.zipstring(&stringdata[0u], string_comprimida_eeprom);
    //eeprom.writestring(string_comprimida_eeprom, 1);
    strcpy(cstr, stringdata.c_str());
    //cstr[strlen(cstr)] = 0;
    Serial.println(stringdata);
    Serial.println(strlen(cstr)+1);
    Serial.println(eeprom.mempos.full);
    eeprom_skip_counter++;
    if(eeprom_skip_counter > eeprom_skip){
      eeprom.writestring(cstr, 0);
      eeprom.updatelogs();
      eeprom_skip_counter = 0;
    }
    Serial.print("Posicao:");
    Serial.println((uint16_t(eeprom.readbyte(0))<<8)+uint16_t(eeprom.readbyte(1)));
    Serial.println(eeprom.err);
    stringdata = "";
    i = 1;
  }
}

float vertical_speed(){
  vel = (referencia_altitude() - lastalt)*1000/(millis() - lastmillis);
  return vel;
}

float referencia_altitude(){
  ref_alt = 0;
  for (int x = 0; x == 5; x++){
    ref_alt = ref_alt + bmp.readAltitude();
    delay(10);
  }
  ref_alt = ref_alt/5;
  return ref_alt;
}

/*
void hora_de_abrir(float v, float h){
  if (h >= 8000 && h <= 28000 && v >= 1){
    for (pos = last_pos; pos <= fim; pos++){
      myservo.write(pos);              
      delay(15);    
      last_pos = pos;                   
    } statustampa = ";aberto"; 
  }else{
    for (pos = last_pos; pos >= inicio; pos++){
      myservo.write(pos);              
      delay(15);                       
    } statustampa = ";fechado";
    last_pos = inicio;
  }
}
*/



