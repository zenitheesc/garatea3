#include <SD.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include <string.h>
#include <SPI.h>

#define UVA1 A1
#define UVB1 A2
#define UVC1 A3
#define UVA2 A0
#define RED 6
#define BLUE 10 
#define GREEN 3
#define CS 4  


int i = 0; 
File myFile;
int a = 0;
int b = 0;
int a1x = 0;
int b1x = 0;
int c1x = 0;
int a2x = 0;
int uva1_read;
int uvb1_read;
int uvc1_read;
int uva2_read;
String uva1_write = "";
String uvb1_write = "";
String uvc1_write = "";
String uva2_write = "";
bool status_sd = true;
String stringdata = "";


void setup() {
  Serial.begin(9600);
pinMode(UVA1, INPUT);
pinMode(UVB1, INPUT);
pinMode(UVC1, INPUT); 
pinMode(UVA2, INPUT); 
pinMode(RED, OUTPUT);
pinMode(GREEN, OUTPUT); 
pinMode(BLUE, OUTPUT); 
Serial.println("Setuup");
if (!SD.begin(CS)) {
      Serial.println("Falha no SD.");
      digitalWrite(RED, HIGH);
      status_sd = false;
      delay(1000);
    }
  else{ 
      Serial.println("SD inicializado.");
      digitalWrite(GREEN, HIGH);
      delay(1000);
  }    

 digitalWrite(RED, LOW);
 digitalWrite(BLUE, LOW);
 digitalWrite(GREEN, LOW);
  
wdt_enable(WDTO_8S);

}

void loop() {
    digitalWrite(BLUE, HIGH);
    String uva1_write = " A";
    String uvb1_write = " B";
    String uvc1_write = " C";
    String uva2_write = " D";
    stringdata = "";
    wdt_reset();
    Serial.println(F("here."));
    for (b = 0; b < 10 ; b++){
    
    a1x = 0;
    b1x = 0;
    c1x = 0;
    a2x = 0;
    
    for (a = 0; a < 1000 ; a++){
 
    uva1_read = analogRead(UVA1);
    if(uva1_read > a1x){
    a1x = uva1_read;
    }
    
    uvb1_read = analogRead(UVB1);
    if(uvb1_read > b1x){
    b1x = uvb1_read;
    }
    
    uvc1_read = analogRead(UVC1);
    if(uvc1_read > c1x){
    c1x = uvc1_read;
    }
    
    uva2_read = analogRead(UVA2);
    if(uva2_read > a2x){
    a2x = uva2_read;
    }

}
  uva1_write.concat(" ");
  uva1_write.concat(a1x);
  uvb1_write.concat(" ");
  uvb1_write.concat(b1x);
  uvc1_write.concat(" ");
  uvc1_write.concat(c1x);
  uva2_write.concat(" ");
  uva2_write.concat(a2x);
}

 stringdata.concat(uva1_write);
 stringdata.concat(uvb1_write);
 stringdata.concat(uvc1_write);
 stringdata.concat(uva2_write);
 
 
 save_data();
 Serial.println(stringdata);
}

void save_data(){
  digitalWrite(RED, LOW);
  delay(2000);
  wdt_reset(); 
  Serial.println(F("Save."));
  myFile = SD.open("fotodata.txt", FILE_WRITE);
  if(myFile){
    myFile.println(stringdata);
    myFile.close();
    Serial.println(F("Gravado."));
    digitalWrite(BLUE, HIGH);
    delay(100); 
  }else{
    Serial.println(F("Erro ao gravar."));
    digitalWrite(RED, HIGH); 
  }
}

