# esp8266_ethernet

Ethernet example using Ethernet drivers for lwIP on esp8266

Arduino core for ESP8266 uses [lwip](https://savannah.nongnu.org/projects/lwip/) for TCP/IP protocol stack.

You can check the version of lwip with the following function: Serial.print(ESP.getFullVersion());

SDK:2.2.2-dev(38a443e)/Core:2.7.1=20701000/lwIP:STABLE-2_1_2_RELEASE/glue:1.2-30-g92add50/BearSSL:5c771be

The hierarchy of programs that use WiFi is as follows:

```
+---------------+
|  Application  |
+---------------+
|  Network-API  |
+---------------+
|      lwip     |
+---------------+
|  WiFi Driver  |
+---------------+ 
```



[This](https://github.com/d-a-v/W5500lwIP) driver replaces the WiFi driver with an Ethernet driver.   
You can use Ethnernet with only a few changes.   

```
+---------------+
|  Application  |
+---------------+
|  Network-API  |
+---------------+
|      lwip     |
+---------------+
|Ethernet Driver|
+---------------+
```

This repository is a example of using Ethernet with ESP8266.

# Hardware requiment
SPI Ethernet module using this chip.
- ENC28J60
- W5100
- W5500

# Software requiment
- Latest of Arduino core for ESP8266.   
I tested 2.7.1.   

- Ethernet drivers for lwIP on esp8266.   
You can download from [here](https://github.com/d-a-v/W5500lwIP).

# Wireing

|SPI PHY|---|ESP8266|
|:-:|:-:|:-:|
|SCK|---|D5|
|MISO|---|D6|
|MOSI|---|D7|
|CS|---|D2(*)|

(*) You can change.

# Selecting the Ethernet module

By default the W5500 ethernet module is selected.

```
#include <W5500lwIP.h>
Wiznet5500lwIP eth(SPI, CSPIN);
```

Using W5100:

```
#include <W5100lwIP.h>
Wiznet5100lwIP eth(SPI, CSPIN);
```

Using ENC28J60:

```
#include <ENC28J60lwIP.h>
ENC28J60lwIP eth(SPI, CSPIN);
```

# Limitations
UDP receive is currently not working.   
This is probably not a driver issue, but a core library issue.   
