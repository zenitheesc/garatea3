#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <avr/wdt.h>
#include <LoRa.h>
#include <Adafruit_BMP085.h> 
#include <stdlib.h>
#include <string.h>
#include <util/crc16.h>
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

int red = 3;
int green = 6;
int blue = 10;

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
    Serial.println(stringdata);
    save_data();
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
  for (int x = 0; i == 5; i++){
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



