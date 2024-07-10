/*
 *  Simple TCP Client
 *  Connect to esp8266-server._esp._tcp.local
 */
 
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

#define INTERVAL    1000
#define TCP_PORT    9876
#define TIME_OUT    10000

IPAddress remoteIp;

WiFiClient client;

unsigned long nextMillis;

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

  // Start the mDNS responder for esp8266-client.local
  if (!MDNS.begin("esp8266-client")) {
    Serial.println("Error setting up MDNS responder!");
  }

  // Find endpoint for esp tcp
  int validRemoteIp = 0;
  while(1) {
    // Send out query for esp tcp services
    Serial.println("Sending mDNS query");
    int n = MDNS.queryService("http", "tcp");
    Serial.println("mDNS query done");
    if (n == 0) {
      Serial.println("no services found");
      delay(1000);
    } else {
      Serial.print(n);
      Serial.println(" service(s) found");
      for (int i = 0; i < n; ++i) {
        // Print details for each service found
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(MDNS.hostname(i));
        Serial.print(" (");
        Serial.print(MDNS.IP(i));
        Serial.print(":");
        Serial.print(MDNS.port(i));
        Serial.println(")");
  
        if (MDNS.port(i) == TCP_PORT) {
          remoteIp = MDNS.IP(i);
          validRemoteIp++;
        } // end if
      } // end for
    } // end if
    if (validRemoteIp != 0) break;
  } // endo for
  
  Serial.print("remoteIp=");
  Serial.println(remoteIp);
  
  nextMillis = millis();
}


void loop() {
  static int num = 0;
  char smsg[30];
  char rmsg[30];
  int rflag;
  unsigned long timeout;
  unsigned long now;
  
  now = millis();
  if ( long(now - nextMillis) > 0) {
    Serial.print("Client connect to ");
    Serial.print(remoteIp);
    Serial.print(" .....");
    if (!client.connect(remoteIp, TCP_PORT)) {
      Serial.println("failed");
      while(1);
    } else {
      Serial.println("ok");
      sprintf(smsg,"data from ESP8266 %05d",num);
      num++;
      client.write(smsg, strlen(smsg));

      // wait for responce
      rflag = 1;
      timeout = millis();
      while(client.available() == 0) {
        now = millis();
        if (long(now - timeout) > TIME_OUT) {
          rflag = 0;
        }
      } // end while

      Serial.print("Server response....");
      if (rflag == 0) {
        Serial.println("failed");
      } else {
        Serial.println("ok");
        int size;
        while((size = client.available()) > 0) {
          Serial.print("TCP Client Receive Size=");
          Serial.println(size);
          size = client.read((uint8_t *)rmsg,size);
          Serial.write((uint8_t *)smsg,size);
          Serial.write("-->");
          Serial.write((uint8_t *)rmsg,size);
          Serial.println("");
        } // end while
      }

      //disconnect client
      Serial.println("Client disconnect");
      client.stop();
      nextMillis = millis() + INTERVAL;

    } // end if
  } // end if
}
