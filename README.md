# esp8266_ethernet
Example of using SPI Ethernet module with esp8266.   
Arduino core for ESP8266 uses [lwip](https://savannah.nongnu.org/projects/lwip/) for TCP/IP protocol stack.   
The hierarchy of programs that use WiFi is as follows:

```
+----------------+
|  Application   |
+----------------+
| Network Object |
+----------------+
|      lwip      |
+----------------+
|   WiFi Driver  |
+----------------+ 
```

Arduino Core for ESP8266 Ver3.0.2 officially supports SPI External Ethernet.   
__You don't need any libraries.__   
The hierarchy of programs that use Ethernet is as follows:

```
+----------------+
|  Application   |
+----------------+
| Network Object |
+----------------+
|      lwip      |
+----------------+
| Ethernet Driver|
+----------------+
```

# Hardware requiment
SPI Ethernet module using this chip.
- ENC28J60
- W5100
- W5500

# Software requiment
- Arduino core for ESP8266 __V3.0.2__ or later  
![](https://img.shields.io/badge/_IMPORTANT-important)  
You can check the Core version using ESP.getFullVersion() or ESP.getCoreVersion().   
This is mine:   
```
ESP.getFullVersion=SDK:2.2.2-dev(38a443e)/Core:3.0.2=30002000/lwIP:STABLE-2_1_2_RELEASE/glue:1.2-48-g7421258/BearSSL:6105635
ESP.getCoreVersion=3.0.2
```


# Wireing

|SPI PHY|---|ESP8266|
|:-:|:-:|:-:|
|SCK|---|GPIO14(D5)|
|MISO|---|GPIO12(D6)|
|MOSI|---|GPIO13(D7)|
|CS|---|GPIO16(D0)(*)|
|RST|---|RESET|
|3.3V|---|3.3V|
|GND|---|GND|

(*) You can change it to any GPIO.

# Selecting the Ethernet module

- Using W5500:
```
#include <W5500lwIP.h>
Wiznet5500lwIP eth(CSPIN);
```
![W5500](https://user-images.githubusercontent.com/6020549/83312712-524abd00-a24e-11ea-9c15-c5ad85022854.JPG)

- Using W5100:
```
#include <W5100lwIP.h>
Wiznet5100lwIP eth(CSPIN);
```
![W5100](https://user-images.githubusercontent.com/6020549/83312708-4f4fcc80-a24e-11ea-923d-409ddeeee855.JPG)

- Using ENC28J60:
```
#include <ENC28J60lwIP.h>
ENC28J60lwIP eth(CSPIN);
```
![ENC28J60](https://user-images.githubusercontent.com/6020549/83312722-57a80780-a24e-11ea-8ae5-f878071a8a3d.JPG)


# Using PlatformIO
```
$ git clone https://github.com/nopnop2002/esp8266_ethernet

$ cd esp8266_ethernet/connectEthernet/

$ pio init -b d1_mini

$ cp connectEthernet.ino src/

$ pio run -t upload && pio device monitor -b 115200

==================================== [SUCCESS] Took 25.50 seconds ====================================
--- Available filters and text transformations: colorize, debug, default, direct, esp8266_exception_decoder, hexlify, log2file, nocontrol, printable, send_on_enter, time
--- More details at https://bit.ly/pio-monitor-filters
--- Miniterm on /dev/ttyUSB0  115200,8,N,1 ---
--- Quit: Ctrl+C | Menu: Ctrl+T | Help: Ctrl+T followed by Ctrl+H ---

SDK:2.2.2-dev(38a443e)/Core:3.0.2=30002000/lwIP:STABLE-2_1_2_RELEASE/glue:1.2-48-g7421258/BearSSL:6105635
connecting ethernet..
ethernet ip address: 192.168.10.128
ethernet subnetMask: 255.255.255.0
ethernet gateway: 192.168.10.1
```

# How to convert from WiFi to Ethernet

- WiFi
```
#include <ESP8266WiFi.h>

const char* SSID = "ssid_of_your_router";
const char* PASSWORD = "password_of_your_router";

void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.print(ESP.getFullVersion());
  Serial.println();

  Serial.print("connecting wifi");
  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD);
  while (1) {
    uint8_t status = WiFi.status();
    Serial.print("status=");
    Serial.println(status);
    if (status == WL_CONNECTED) break;
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("WiFi ip address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Wifi subnetMask: ");
  Serial.println(WiFi.subnetMask());
  Serial.print("WiFi.gatewayIP: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("WiFi.dnsIP: ");
  Serial.println(WiFi.dnsIP());

}

void loop() {
}
```

- Ethernet
```
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <W5500lwIP.h>
//#include <W5100lwIP.h>
//#include <ENC28J60lwIP.h>

#define CSPIN 16

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
}
```


