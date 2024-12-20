#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <W5500lwIP.h>
//#include <W5100lwIP.h>
//#include <ENC28J60lwIP.h>

#define CSPIN 16

//Uncomment if using Directed broadcast address
//#define DIRECT

Wiznet5500lwIP eth(CSPIN);
//Wiznet5100lwIP eth(CSPIN);
//ENC28J60lwIP eth(CSPIN);
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};

// local port to listen for UDP packets
#define LOCAL_PORT      8888

// remote port to send for UDP packets
#define REMOTE_PORT     9876

// remote address to send for UDP packets
char remote_address[64];

// Time of last packet transmission(ms)
unsigned long lastSendPacketTime = 0;

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print(ESP.getFullVersion());
  Serial.println();
  
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  SPI.setFrequency(4000000);

  eth.setDefault(); // use ethernet for default route
  int present = eth.begin(mac);
  if (!present) {
    Serial.println("no ethernet hardware present");
    while(1);
  }
  
  Serial.print("connecting ethernet");
  while (!eth.connected()) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println();
  Serial.print("ethernet ip address: ");
  Serial.println(eth.localIP());
  Serial.print("ethernet subnetMask: ");
  Serial.println(eth.subnetMask());
  Serial.print("ethernet gateway: ");
  Serial.println(eth.gatewayIP());

  strcpy(remote_address, "255.255.255.255");
  #ifdef DIRECT
  //Serial.println(eth.localIP()[0]);
  //Serial.println(eth.localIP()[1]);
  //Serial.println(eth.localIP()[2]);
  sprintf(remote_address, "%d.%d.%d.255", eth.localIP()[0], eth.localIP()[1], eth.localIP()[2]);
  #endif
  
  Serial.print("remote_address=[");
  Serial.print(remote_address);
  Serial.println("]");
  Serial.println("Starting UDP Send");
  udp.begin(LOCAL_PORT);
}

void loop() {
  long now = millis();
  if (now - lastSendPacketTime > 1000) { // 5 seconds passed
    lastSendPacketTime = now;
    udp.beginPacket(remote_address, REMOTE_PORT);
    byte packetBuffer[64];
    sprintf((char *)packetBuffer, "Hello World! %ld", millis());
    size_t packetSize = strlen((char*)packetBuffer);
    udp.write(packetBuffer, packetSize);
    udp.endPacket();
  }
}
