#include <SPI.h>
#include <ESP8266WiFi.h>
#include <W5500lwIP.h>
//#include <W5100lwIP.h>
//#include <ENC28J60lwIP.h>

#define CSPIN 16

//#define STATIC

Wiznet5500lwIP eth(CSPIN);
//Wiznet5100lwIP eth(CSPIN);
//ENC28J60lwIP eth(CSPIN);
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};

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

#ifdef STATIC
  IPAddress ip(192,168,10,130);   
  IPAddress gateway(192,168,10,1);   
  IPAddress subnet(255,255,255,0);
  IPAddress dns1(8,8,8,8);
  IPAddress dns2(8,8,4,4);
  eth.config(ip, gateway, subnet, dns1, dns2);
#endif
  
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
}

void loop() {
  //Serial.print("eth.status=");
  //Serial.println(eth.status());
  if (eth.status() == WL_CONNECTED) {
    Serial.println("Ethernet Connected!");
  }
  else {
    Serial.println("Ethernet Not Connected!");
  }
  delay(1000);
}
