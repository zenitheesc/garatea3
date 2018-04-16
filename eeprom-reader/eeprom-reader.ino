#include <inttypes.h>
#include <Wire.h>
#include <string.h>
#include "eeprom.h"

void setup() {
  uint16_t ptr;
  uint16_t last = 0;
  char c;
  delay(3000);
  Wire.begin();
  Serial.begin(9600);
  while (!Serial) { ; }
  Serial.println("eeprom setup...");
  eeprom.init(0);
  Serial.print("Error log: ");
  Serial.println(eeprom.readbyte(2));
  Serial.print("Last address written: ");
  last = ( ( uint16_t( eeprom.readbyte( 0 ) ) << 8 ) + uint16_t( eeprom.readbyte( 1 ) ) );
  Serial.println(last);
  Serial.println("Init done!");
  Serial.println("---EEPROM DATA---");
  ptr = 0;
  while (ptr < last) {
    c = eeprom.readbyte( ptr );
    Serial.print( c );
    if ( c == '\0' ) {
      Serial.println(' ');
    }
    ptr++;
  }
  Serial.println("---END OF DATA---");
  while (1) {
    delay(127);
  }
}

