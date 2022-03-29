# UDP Broadcast Receive
Transmitter using Python.   
Using Limited broadcast address.   
Limited broadcast addresses can't cross routers.   

If you use a direct broadcast address, you will need to change <broadcast>.   
Direct broadcast addresses can cross routers.   
```
import sys
import socket
import time
import signal
import datetime

def handler(signal, frame):
  global flag
  print('handler')
  flag = False

def main():
  global flag
  sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
  sock.setsockopt(socket.SOL_SOCKET, socket.SO_BROADCAST, 1)
  signal.signal(signal.SIGINT, handler)
  flag = True

  while flag:
    #print("sys.version_info={}".format(sys.version_info))
    dt_now = datetime.datetime.now()
    #print("type(dt_now)={}".format(type(dt_now)))
    message = dt_now.strftime('%Y/%m/%d/ %H:%M:%S')
    #print("type(message)={}".format(type(message)))
    print("message={}".format(message))
    if (sys.version_info >= (3, 0)):
      sock.sendto(message.encode('utf-8'), ('<broadcast>', 9876))
    else:
      sock.sendto(message, ('<broadcast>', 9876))
    time.sleep(5)

if __name__ == "__main__":
  main()
```
