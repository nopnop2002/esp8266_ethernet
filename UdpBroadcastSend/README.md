# UDP Broadcast Send
This project uses Limited broadcast address by default.   
Limited broadcast addresses can't cross routers.   
If you use a directed broadcast address, Uncomment this line.   
Directed broadcast addresses can cross routers.   
```
//#define DIRECT
```

Limited broadcast address is 255.255.255.255.   
Directed broadcast address is like 192.168.10.255, only the last octet is 255.   

__Notes on using staic ip__   
If you use a static IP, use Directed broadcast address instead of Limited broadcast address.   

# UDP Broadcast Receiver using python
```
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
    parser.add_argument('--directed', action='store_true', help="Use Directed broadcast address")
    args = parser.parse_args()
    print("args.port={}".format(args.port))
    print("args.directed={}".format(args.directed))

    if (args.directed):
        connect_interface = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        connect_interface.connect(("8.8.8.8", 80))
        local_address = connect_interface.getsockname()[0]
        #print(connect_interface.getsockname()[0])
        print("local_address={} {}".format(local_address, type(local_address)))
        connect_interface.close()
        local_address = local_address.split(".")
        print("local_address={} {}".format(local_address, type(local_address)))
        remote_address = "{}.{}.{}.255".format(local_address[0],local_address[1],local_address[2])
    else:
        remote_address = '<broadcast>'

    print("remote_address={}".format(remote_address))
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    #sock.bind(('<broadcast>', args.port))
    sock.bind((remote_address, args.port))
    sock.setblocking(False)

    while running:
        # With 1 second timeout
        result = select.select([sock],[],[], 1)
        #print("result[0]={}".format(result[0]))
        # Not timeout
        if (len(result[0]) != 0):
            msg = result[0][0].recv(1024)
            if (type(msg) is bytes):
                msg=msg.decode('utf-8')
            print(msg)

    sock.close()
```
