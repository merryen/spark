#!/usr/bin/python           # This is client.py file
import codecs
import socket               # Import socket module

s = socket.socket()         # Create a socket object
host = '192.168.1.100' # Get local machine name
port = 23                # Reserve a port for your service.

s.connect((host, port))
file_name = raw_input('Name of your file: ')
print 'File created'
while (1):
    file = open('/Users/Annamaria/Desktop/'+file_name, 'a')
    reading = s.recv(513)
    print reading
    file.write(str(reading))
    file.close()
s.close                     # Close the socket when done