# UDP Broadcast Receive
Transmitter using Python.   
Using Limited broadcast address.   
Limited broadcast addresses can't cross routers.   
__Note:__   
If you use a direct broadcast address, you will need to change <broadcast>.   
Direct broadcast addresses can cross routers.   
```
#!/usr/bin/python
#-*- encoding: utf-8 -*-
import argparse
import sys
import socket
import time
import signal
import datetime

def handler(signal, frame):
    global running
    print('handler')
    running = False

if __name__ == "__main__":
    signal.signal(signal.SIGINT, handler)
    running = True

    parser = argparse.ArgumentParser()
    parser.add_argument('--port', type=int, help="UDP Port", default=9876)
    args = parser.parse_args()
    print("args.port={}".format(args.port))

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)

    while running:
        #print("sys.version_info={}".format(sys.version_info))
        dt_now = datetime.datetime.now()
        #print("type(dt_now)={}".format(type(dt_now)))
        message = dt_now.strftime('%Y/%m/%d/ %H:%M:%S')
        #print("type(message)={}".format(type(message)))
        print("message={}".format(message))
        if (sys.version_info >= (3, 0)):
            sock.sendto(message.encode('utf-8'), ('<broadcast>', args.port))
        else:
            sock.sendto(message, ('<broadcast>', args.port))
        time.sleep(5)

    #print("socket close")
    sock.close()
```


# Screen Shot
```
SDK:2.2.2-dev(38a443e)/Core:3.0.2=30002000/lwIP:STABLE-2_1_2_RELEASE/glue:1.2-48-g7421258/BearSSL:6105635
connecting ethernet..
ethernet ip address: 192.168.10.127
ethernet subnetMask: 255.255.255.0
ethernet gateway: 192.168.10.1
Now listening at IP:192.168.10.127 UDP port:9876

Received 20 bytes from 192.168.10.45, port 57811
UDP packet from 192.168.10.45 57811
UDP packet contents: 2022/03/30/ 08:14:08

Received 20 bytes from 192.168.10.45, port 57811
UDP packet from 192.168.10.45 57811
UDP packet contents: 2022/03/30/ 08:14:13

Received 20 bytes from 192.168.10.45, port 57811
UDP packet from 192.168.10.45 57811
UDP packet contents: 2022/03/30/ 08:14:18
```

