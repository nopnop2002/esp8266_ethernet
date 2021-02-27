#include <SPI.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <W5500lwIP.h>   // https://github.com/d-a-v/W5500lwIP

#define CSPIN 16

Wiznet5500lwIP eth(SPI, CSPIN);
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};


// local port to listen for UDP packets
#define LOCAL_PORT      8888

// remote port to send for UDP packets
#define REMOTE_PORT     9876

// Time of last packet transmission(ms)
unsigned long lastSendPacketTime = 0;

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

void setup() {

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

  Serial.println("Starting UDP");
  udp.begin(LOCAL_PORT);
}

void loop() {
  long now = millis();
  if (now - lastSendPacketTime > 5000) { // 5 seconds passed
    lastSendPacketTime = now;
    udp.beginPacket("255.255.255.255", REMOTE_PORT);
    byte packetBuffer[64];
    sprintf((char *)packetBuffer, "Hello World! %d", millis());
    size_t packetSize = strlen((char*)packetBuffer);
    udp.write(packetBuffer, packetSize);
    udp.endPacket();
  }
}
