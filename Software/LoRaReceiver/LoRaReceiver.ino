#include <SPI.h>
#include <LoRa.h>
String stringdata = "";

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  LoRa.setPins(7, A0, 2);
  if (!LoRa.begin(433.123E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.setSignalBandwidth(125E3);
  LoRa.setSpreadingFactor(11);
  LoRa.enableCrc();
}

void loop() {
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet

    // read packet
    while (LoRa.available()) {
      stringdata.concat((char)LoRa.read());
    }

    // print RSSI of packet
    Serial.print(stringdata); Serial.print(";");
    stringdata = "";
    Serial.println(LoRa.packetRssi());
  }
}
