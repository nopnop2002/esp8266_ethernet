# mqtt publish
mqtt subscribe using mosquitto_sub.   
```
$ sudo apt install mosquitto-clients

$ sudo apt install moreutils

$ mosquitto_sub -v -h broker.emqx.io -p 1883  -t "/topic/test" | ts "%y/%m/%d %H:%M:%S"
24/07/11 19:35:12 /topic/test hello world #1
24/07/11 19:35:13 /topic/test hello world #2
24/07/11 19:35:14 /topic/test hello world #3
24/07/11 19:35:15 /topic/test hello world #4
24/07/11 19:35:16 /topic/test hello world #5
24/07/11 19:35:17 /topic/test hello world #6
24/07/11 19:35:18 /topic/test hello world #7
24/07/11 19:35:19 /topic/test hello world #8
24/07/11 19:35:20 /topic/test hello world #9
```
