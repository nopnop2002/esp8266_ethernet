#include <SPI.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient
#include <W5500lwIP.h>
//#include <W5100lwIP.h>
//#include <ENC28J60lwIP.h>

#define MQTT_PORT       1883
#define MQTT_TOPIC      "#"               // You can change
#define MQTT_WILL_TOPIC "/topic/test"     // You can change
#define MQTT_WILL_MSG   "I am leaving..." // You can change
#define MQTT_SERVER     "192.168.10.40"

#define CSPIN 16

Wiznet5500lwIP eth(CSPIN);
//Wiznet5100lwIP eth(CSPIN);
//ENC28J60lwIP eth(CSPIN);
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};

WiFiClient espClient;
PubSubClient client(espClient);

void errorDisplay(char* buff) {
  Serial.print("Error:");
  Serial.println(buff);
  while(1) {
    delay(200);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}


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
  
  client.setServer(MQTT_SERVER, MQTT_PORT);
  //client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  //server_connect("#");

  char clientid[20];
  sprintf(clientid,"ESP8266-%06x",ESP.getChipId());
  Serial.print("clientid=");
  Serial.println(clientid);
  Serial.print("Attempting MQTT connection...");
  // Attempt to connect
  if (strlen(MQTT_WILL_MSG)) {
//    Serial.println("WILL");
    if (!client.connect(clientid,MQTT_WILL_TOPIC,0,0,MQTT_WILL_MSG)) {
      errorDisplay("connect Fail");
    }
  } else {
//    Serial.println("NOT WILL");
    if (!client.connect(clientid)) {
      errorDisplay("connect Fail");
    }
  }
  Serial.println("connected");
  client.subscribe(MQTT_TOPIC);
}

void loop() {
  client.loop(); // Receive MQTT Event
}
