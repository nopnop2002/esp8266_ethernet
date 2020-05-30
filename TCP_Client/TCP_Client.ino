/*
 *  TCP Client
 *  Connect to _esp8266_wifi_tcp
 */
 
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <W5500lwIP.h> // https://github.com/d-a-v/W5500lwIP
//#include <W5100lwIP.h> // https://github.com/d-a-v/W5500lwIP
//#include <ENC28J60lwIP.h> // https://github.com/d-a-v/W5500lwIP

#define CSPIN D2

Wiznet5500lwIP eth(SPI, CSPIN);
//Wiznet5100lwIP eth(SPI, CSPIN);
//ENC28J60lwIP eth(SPI, CSPIN);
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x04};

#define INTERVAL       1000
#define SOCKET_PORT    9876
#define TIME_OUT       10000

IPAddress remoteIp;

WiFiClient client;

unsigned long nextMillis;

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

  char myDomainName[16] = {0};
  sprintf(myDomainName, "ESP_%06X", ESP.getChipId());
  Serial.print("myDomainName: ");
  Serial.println(myDomainName);
  // Set up mDNS responder:
  // - first argument is the domain name, in this example
  //   the fully-qualified domain name is "esp8266.local"
  // - second argument is the IP address to advertise
  //   we send our IP address on the WiFi network
  if (!MDNS.begin(myDomainName)) {
    Serial.println("Error setting up MDNS responder!");
  }

  // Find endpoint for _esp8266_wifi_tcp
  bool valid = false;
  for(int i=0;i<10;i++) {
    // Send out query for esp tcp services
    Serial.println("Sending mDNS query");
    int n = MDNS.queryService("esp8266_wifi", "tcp");
    Serial.println("mDNS query done");
    if (n == 0) {
      Serial.println("no services found");
      delay(1000);
      continue;
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
  
        if (MDNS.port(i) == SOCKET_PORT) {
          remoteIp = MDNS.IP(i);
          valid = true;
          break;
        }
      }
    }
    if (valid) break;
  } // endo for
  
  if (valid) {
    Serial.print("remoteIp=");
    Serial.println(remoteIp);
  } else {
    Serial.println("end-piont not found");
    while(1) delay(100);
  }
  
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
    nextMillis = millis() + INTERVAL;
    Serial.print("Client connect....");
    if (!client.connect(remoteIp, SOCKET_PORT)) {
      Serial.println("failed");
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
//        Serial.println("now="+String(now));
//        Serial.println("timeout="+String(timeout));
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
          Serial.write("->");
          Serial.write((uint8_t *)rmsg,size);
          Serial.println("");
        } // end while
      }

      //disconnect client
      Serial.println("Client disconnect");
      client.stop();
    } // end if
  } // end if
}
