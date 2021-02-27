#include <SPI.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h> // https://github.com/knolleary/pubsubclient
#include <W5500lwIP.h>    // https://github.com/d-a-v/W5500lwIP

#define INTERVAL        1000
#define MQTT_PORT       1883
#define MQTT_TOPIC      "/topic/test"     // You can change
#define MQTT_WILL_TOPIC "/topic/test"     // You can change
#define MQTT_WILL_MSG   "I am leaving..." // You can change
#define MQTT_SERVER     "192.168.10.40"
#define PAYLOAD_SIZE    50

#define CSPIN 16

Wiznet5500lwIP eth(SPI, CSPIN);
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02};

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long lastMillis;

IPAddress localIP;

void errorDisplay(char* buff) {
  Serial.print("Error:");
  Serial.println(buff);
  while(1) {
    delay(200);
  }
}

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
  localIP = eth.localIP();
  
  client.setServer(MQTT_SERVER, MQTT_PORT);
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
  lastMillis = millis();
}

void loop() {
  static int value = 0;
  char payload[PAYLOAD_SIZE];

  client.loop();

  if (Serial.available() > 0) {
    char inChar = Serial.read();
    Serial.println("KeyIn");
    client.disconnect();
    Serial.println("Publish end");
    while(1) { }
  }

  if (!client.connected()) {
    errorDisplay("not connect");
  }

  long now = millis();
  if (now - lastMillis > INTERVAL) {
    lastMillis = now;
    ++value;
    snprintf (payload, PAYLOAD_SIZE, "hello world #%ld", value);
    Serial.print("loalIP:");
    Serial.print(localIP);
    Serial.print(" Publish message: ");
    Serial.println(payload);
    if (!client.publish(MQTT_TOPIC, payload)) {
      errorDisplay("publish fail");
    }
  }
}
