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
|SCK|---|GPIO14|
|MISO|---|GPIO12|
|MOSI|---|GPIO13|
|CS|---|GPIO16(*)|
|RST|---|RESET|

(*) You can change.

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

# Limitations
NTP Client is currently not working.   
This is probably not a driver issue, but a core library issue.   


# Using platformIO
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

