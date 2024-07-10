# UDP Broadcast Send
Receiver using python.   
Using Limited broadcast address.   
Limited broadcast addresses can't cross routers.   
__Note:__   
If you use a direct broadcast address, you will need to change <broadcast>.   
Direct broadcast addresses can cross routers.   
```
#!/usr/bin/python
#-*- encoding: utf-8 -*-
import argparse
import select
import socket
import signal

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
    sock.bind(('<broadcast>', args.port))
    sock.setblocking(0)

    while running:
        result = select.select([sock],[],[])
        msg = result[0][0].recv(1024)
        if (type(msg) is bytes):
            msg=msg.decode('utf-8')
        print(msg)

    sock.close()
```
