# esp8266_ethernet
Ethernet example using Ethernet drivers for lwIP on esp8266

Arduino core for ESP8266 uses [lwip](https://savannah.nongnu.org/projects/lwip/) for TCP/IP protocol stack.

You can check the version of lwip with the following function: Serial.print(ESP.getFullVersion());

SDK:2.2.2-dev(38a443e)/Core:2.7.1=20701000/lwIP:STABLE-2_1_2_RELEASE/glue:1.2-30-g92add50/BearSSL:5c771be

The hierarchy of programs that use WiFi is as follows:

+---------------+   
|  Appilication |   
+---------------+   
|      API      |   
+---------------+   
|      lwip     |   
+---------------+   
| WiFi  Driver  |   
+---------------+   



[This](https://github.com/d-a-v/W5500lwIP) driver replaces the WiFi driver with an Ethernet driver.

+---------------+   
|  Appilication |   
+---------------+   
|      API      |   
+---------------+   
|      lwip     |   
+---------------+   
|Ethernet Driver|   
+---------------+   


This repository is a example of using Ethernet with ESP8266.
