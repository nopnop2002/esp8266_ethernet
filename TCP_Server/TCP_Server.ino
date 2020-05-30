/*
 *  TCP Server
 *  End-point name is _esp8266_wifi_tcp
 */

#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
//#include <W5500lwIP.h> // https://github.com/d-a-v/W5500lwIP
#include <W5100lwIP.h> // https://github.com/d-a-v/W5500lwIP
//#include <ENC28J60lwIP.h> // https://github.com/d-a-v/W5500lwIP

#define CSPIN D2

//Wiznet5500lwIP eth(SPI, CSPIN);
Wiznet5100lwIP eth(SPI, CSPIN);
//ENC28J60lwIP eth(SPI, CSPIN);
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};

#define SERVER_PORT    9876

// Create an instance of the server
// specify the port to listen on as an argument
//WiFiServer server(SERVER_PORT);
WiFiServer server = WiFiServer(SERVER_PORT);

void setup()
{
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

  server.begin();
  Serial.println("Server Start. Wait for Client");


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
    while (1) {
      delay(1000);
    }
  }
  // Add service to MDNS-SD
  Serial.println("mDNS responder started");
  MDNS.addService("esp8266_wifi", "tcp", SERVER_PORT);

}

void loop() {

  MDNS.update();
 
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // Read the first line of the request
  String rmsg = client.readStringUntil('\0');
  Serial.print(rmsg);
  client.flush();

  // Prepare the response
  char smsg[128];
  memset(smsg,0,sizeof(smsg));
  for(uint32_t i = 0; i < rmsg.length(); i++) {
    if(isalpha(rmsg[i])) {
      smsg[i] = toupper(rmsg[i]);
    } else {
      smsg[i] = rmsg[i];
    } // end if
  } // end for

  Serial.print("->");
  Serial.println(smsg);

  // Send the response to the client
  client.print(smsg);
  delay(1);
  Serial.println("Client disonnected");

  // The client will actually be disconnected 
  // when the function returns and 'client' object is detroyed
}
