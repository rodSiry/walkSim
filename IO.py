import socket
import struct
import array
import sys

class IO:
    def __init__(self):
        self.s=socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
        self.s.bind("socket")
        self.s.listen(1)
        print("Attente du client")
        self.client, self.adress=self.s.accept()
    def getMsg(self):
        l=self.client.recv(4)
        l=struct.unpack('i', l)[0]
        fit=self.client.recv(l)
        fit=array.array('d', fit)
        return fit
    def sendMsg(self, ms):
        self.client.send(struct.pack('i', len(ms)))
        self.client.send(array.array("d", ms))
    def close(self):    
        self.client.close()
        self.s.close()

   
