#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <W5500lwIP.h>
//#include <W5100lwIP.h>
//#include <ENC28J60lwIP.h>

#define CSPIN 16

Wiznet5500lwIP eth(CSPIN);
//Wiznet5100lwIP eth(CSPIN);
//ENC28J60lwIP eth(CSPIN);
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x04};

#define LOCAL_PORT 9876

WiFiUDP Udp;

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

  Udp.begin(LOCAL_PORT);
  Serial.print("Now listening at IP:");
  Serial.print(eth.localIP());
  Serial.print(" UDP port:");
  Serial.print(LOCAL_PORT);
  Serial.println();
}

void loop() {
  char payload[128];
  IPAddress remoteIp;
  int remotePort;

  int packetSize = Udp.parsePacket();
  if (packetSize) {
    // receive incoming UDP packets
    Serial.println();
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    remoteIp = Udp.remoteIP();
    remotePort = Udp.remotePort();
    int payloadSize = Udp.read(payload, sizeof(payload));
    if (payloadSize > 0) {
      payload[payloadSize] = 0;
      Serial.print("UDP packet from ");
      Serial.print(remoteIp);
      Serial.print(" ");
      Serial.println(remotePort);
      Serial.print("UDP packet contents: ");
      Serial.println(payload);
    } // end payloadSize
  } // end packetSize
  
}
