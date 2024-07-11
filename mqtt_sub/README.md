# mqtt subscribe
mqtt publish using mosquitto_sub.   
```
$ sudo apt install mosquitto-clients

$ cat mqtt_pub.sh
#!/bin/bash
#set -x
count=1
while :
do
    payload=`date "+%Y/%m/%d %H:%M:%S"`
    echo ${payload}
    mosquitto_pub -h broker.emqx.io -p 1883 -t "/topic/test" -m "$payload"
    count=$((++count))
    sleep 1
done

$ ./mqtt_pub.sh
```
