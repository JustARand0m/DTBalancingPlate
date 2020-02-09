# !/usr/bin/env python
# -*- coding: utf-8 -*-
 
import os
import sys
import argparse
import socket
import base64
import time
 
# Presets / Variables
host = "172.16.36.108"
port = 1337
delay = 0.1 # (in seconds)
x = 0.000
y = 50.000
flagX = 1
flagY = 0
degreesteps = 1#0.25
maxangle = 50
 
# address for UDP Socket
addr = (host, port)
 
# code for sending continuous messages
# Send X and Y going down by degreesteps
# comment out the while loop and use code belowif you want to send data manually
while 1:
    UDPSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
 
    if flagX == 1:
        x = x + degreesteps
        if x >= maxangle:
            flagX = 0
    elif flagX == 0:
        x = x - degreesteps
        if x <= -maxangle:
            flagX = 1
    if flagY == 1:
        y = y + degreesteps
        if y >= maxangle:
            flagY = 0
    elif flagY == 0:
        y = y - degreesteps
        if y <= -maxangle:
            flagY = 1
     
    cdata = str(x) + ";" + str(y) + ";"
    print >> sys.stderr, "Sending: " + cdata
    UDPSock.sendto(cdata, addr)
    UDPSock.close()
    time.sleep(delay)
 
# code for sending a single message
UDPSock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
cdata = str(x) + ";" + str(y) + ";"
print >>sys.stderr, "Sending: " + cdata
UDPSock.sendto(cdata, addr)
UDPSock.close()
exit(0)