#!/usr/bin/python           

import socket               
import sys
from time import sleep

if len(sys.argv) < 3:
	print "Usage: {} prefix keys_num".format(sys.argv[0])
	sys.exit(1)

s = socket.socket()        
host = socket.gethostname() 
port = 7777                

try:
	s.connect((host, port))
except socket.error:
	print "Connection refused"
	sys.exit(1)

for i in range(int(sys.argv[2])):
	s.send("PUT {0}{1} 1\n".format(sys.argv[1], str(i)))
	s.send("GET {0}{1}\n".format(sys.argv[1],str(i)))
	print s.recv(100)
	sleep(0.01)
s.close()
