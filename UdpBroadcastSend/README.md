# UDP Broadcast Send
Receiver using python.   
Using Limited broadcast address.   
Limited broadcast addresses can't cross routers.   
__Note:__   
If you use a direct broadcast address, you will need to change <broadcast>.   
Direct broadcast addresses can cross routers.   
```
import select
import socket
import signal

def handler(signal, frame):
  global flag
  print('handler')
  flag = False

def main():
  global flag
  s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  s.bind(('<broadcast>', 9876))
  s.setblocking(0)
  flag = True

  while flag:
    result = select.select([s],[],[])
    msg = result[0][0].recv(1024)
    if (type(msg) is bytes):
      msg=msg.decode('utf-8')
    print(msg)

if __name__ == "__main__":
  main()
```
