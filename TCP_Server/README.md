# TCP Server
TCP Client using Python.   
```
#!/usr/bin/python
#-*- encoding: utf-8 -*-
import argparse
import sys
import socket
import time
import signal

def handler(signal, frame):
    global running
    print('handler')
    running = False

if __name__ == "__main__":
    signal.signal(signal.SIGINT, handler)
    running = True

    parser = argparse.ArgumentParser()
    parser.add_argument('--host', help="TCP Host", default="esp8266.local")
    parser.add_argument('--port', type=int, help="TCP Port", default=9876)
    args = parser.parse_args()
    print("args.host={}".format(args.host))
    print("args.port={}".format(args.port))

    while running:
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        try:
            client.connect((args.host, args.port))
        except:
            print("{} not found".format(args.host))
            sys.exit()
        request = "Hello World"
        #client.send(b'Hello World')
        client.send(request.encode('utf-8'))
        response = client.recv(1024)
        if (type(response) is bytes):
            response=response.decode('utf-8')
        print("{} --> {}".format(request, response))
        client.close()
        time.sleep(5)
```


# Screen Shot
```
SDK:2.2.2-dev(38a443e)/Core:3.1.2=30102000/lwIP:STABLE-2_1_3_RELEASE/glue:1.2-65-g06164fb/BearSSL:b024386
connecting ethernet...
ethernet ip address: 192.168.10.113
ethernet subnetMask: 255.255.255.0
ethernet gateway: 192.168.10.1
Server Start. Wait for Client
mDNS responder started
new client
TCP Server Receive Size=11
Hello World->hELLO wORLD
Client disonnected
new client
TCP Server Receive Size=11
Hello World->hELLO wORLD
Client disonnected
new client
TCP Server Receive Size=11
Hello World->hELLO wORLD
Client disonnected
```

