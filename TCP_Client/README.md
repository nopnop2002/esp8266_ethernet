# TCP Client
TCP Server using Python & zeroconf.   
You need to install zeroconf library.   
```
$ python3 -m pip install zeroconf
```



```
#!/usr/bin/python
#-*- encoding: utf-8 -*-
import argparse
import socket
import netifaces
import signal

from zeroconf import IPVersion, ServiceInfo, Zeroconf

def handler(signal, frame):
    global running
    print('handler')
    running = False

if __name__ == "__main__":
    signal.signal(signal.SIGINT, handler)
    running = True

    parser = argparse.ArgumentParser()
    parser.add_argument('--device', help="Ethernet device", default="eth0")
    parser.add_argument('--port', type=int, help="TCP Port", default=9876)
    parser.add_argument("--debug", action="store_true")
    args = parser.parse_args()
    print("args.device={}".format(args.device))
    print("args.port={}".format(args.port))

    if args.debug:
        logging.getLogger("zeroconf").setLevel(logging.DEBUG)

    ip_addr = netifaces.ifaddresses(args.device)[netifaces.AF_INET][0]['addr']
    print("ip_addr={}".format(ip_addr))

    info = ServiceInfo(
        "_http._tcp.local.",
        "TCP Server._http._tcp.local.",
        addresses=[socket.inet_aton(ip_addr)],
        port=args.port,
        server="esp8266-server.local.",
    )

    ip_version = IPVersion.V4Only
    zeroconf = Zeroconf(ip_version=ip_version)
    print("Registration of a service, press Ctrl-C to exit...")
    zeroconf.register_service(info)

    tcp_server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    tcp_server.bind(("", args.port))
    tcp_server.listen(5)

    while running:
        client,address = tcp_server.accept()
        print("Connected!! [ Source : {}]".format(address))

        rmsg = client.recv(1024)
        if (type(rmsg) is bytes):
            rmsg=rmsg.decode('utf-8')
        #print("Received Data : {}".format(rmsg))
        smsg = rmsg.swapcase()
        client.send(smsg.encode('utf-8'))
        print("{} --> {}".format(rmsg, smsg))

        client.close()

    print("Unregistering...")
    zeroconf.unregister_service(info)
    zeroconf.close()
```


# Screen Shot
```
SDK:2.2.2-dev(38a443e)/Core:3.1.2=30102000/lwIP:STABLE-2_1_3_RELEASE/glue:1.2-65-g06164fb/BearSSL:b024386
connecting ethernet...
ethernet ip address: 192.168.10.114
ethernet subnetMask: 255.255.255.0
ethernet gateway: 192.168.10.1
Sending mDNS query
mDNS query done
1 service(s) found
1: esp8266-server.local (192.168.10.46:9876)
remoteIp=192.168.10.46
Client connect to 192.168.10.46 .....ok
Server response....ok
TCP Client Receive Size=23
data from ESP8266 00000-->DATA FROM esp8266 00000
Client disconnect
Client connect to 192.168.10.46 .....ok
Server response....ok
TCP Client Receive Size=23
data from ESP8266 00001-->DATA FROM esp8266 00001
Client disconnect
Client connect to 192.168.10.46 .....ok
Server response....ok
TCP Client Receive Size=23
data from ESP8266 00002-->DATA FROM esp8266 00002
Client disconnect
Client connect to 192.168.10.46 .....ok
Server response....ok
TCP Client Receive Size=23
data from ESP8266 00003-->DATA FROM esp8266 00003
Client disconnect
Client connect to 192.168.10.46 .....ok
Server response....ok
TCP Client Receive Size=23
data from ESP8266 00004-->DATA FROM esp8266 00004
Client disconnect
```

