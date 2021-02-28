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

The latest Arduino core supports SPI external Ethernet as standard.   
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
- Latest of Arduino core for ESP8266.   
I tested Version 3.0.0.   
![](https://img.shields.io/badge/_IMPORTANT-important)  
You cannot install this version using Borad Manager.   
You need to install it using git according to [this](https://arduino-esp8266.readthedocs.io/en/latest/installing.html#using-git-version) page.   

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
UDP receive is currently not working.   
This is probably not a driver issue, but a core library issue.   
