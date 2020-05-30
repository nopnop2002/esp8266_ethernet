#include <SPI.h>
#include <ESP8266WiFi.h>
#include <W5500lwIP.h> // https://github.com/d-a-v/W5500lwIP
#include <time.h>

#define CSPIN D2

Wiznet5500lwIP eth(SPI, CSPIN);
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x04};

#define TIME_ZONE "JST-9"

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

  configTzTime(TIME_ZONE, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp");   // 2.7.0 or later
}

void loop() {
  time_t t;
  struct tm *tm;
  static const char *wd[7] = {"Sun","Mon","Tue","Wed","Thr","Fri","Sat"};

  t = time(NULL);
  tm = localtime(&t);
  Serial.printf("ESP8266/Arduino ver%s :  %04d/%02d/%02d(%s) %02d:%02d:%02d\n",
        __STR(ARDUINO_ESP8266_GIT_DESC),
        tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday,
        wd[tm->tm_wday],
        tm->tm_hour, tm->tm_min, tm->tm_sec);
  delay(1000);
}
