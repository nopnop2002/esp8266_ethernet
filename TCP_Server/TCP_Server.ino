/*
    TCP Server
    End-point name is _esp8266_wifi_tcp
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
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};

#define SERVER_PORT    9876

// Create an instance of the server
// specify the port to listen on as an argument
WiFiServer server(SERVER_PORT);

//#define STATIC

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
  eth.config(ip, gateway, subnet);
#endif

  int present = eth.begin(mac);
  if (!present) {
    Serial.println("no ethernet hardware present");
    while (1);
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

  server.begin();
  Serial.println("Server Start. Wait for Client");

  // Start the mDNS responder for esp8266.local
  if (!MDNS.begin("esp8266")) {
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  // Add service to MDNS-SD
  Serial.println("mDNS responder started");
  MDNS.addService("http", "tcp", SERVER_PORT);

}

void loop() {
  char smsg[30];
  char rmsg[30];

  MDNS.update();

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wait until the client sends some data
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  int size;
  while ((size = client.available()) > 0) {
    Serial.print("TCP Server Receive Size=");
    Serial.println(size);
    size = client.read((uint8_t *)rmsg, size);
    for (uint32_t i = 0; i < size; i++) {
      if (islower(rmsg[i])) {
        smsg[i] = toupper(rmsg[i]);
      } else {
        smsg[i] = tolower(rmsg[i]);
      } // end if
    } // end for

    client.write(smsg, size);

    Serial.write((uint8_t *)rmsg, size);
    Serial.write("->");
    Serial.write((uint8_t *)smsg, size);
    Serial.println("");
  } // end while

  Serial.println("Client disonnected");
  // The client will actually be disconnected
  // when the function returns and 'client' object is detroyed
}
