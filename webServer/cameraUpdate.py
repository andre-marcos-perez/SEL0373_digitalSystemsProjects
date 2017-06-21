#!/usr/bin/env python

from time import sleep
import requests

class Camera(object):

#Attributes

    PROTOCOL   = 'http://'
    IP_ADDRESS = '143.107.235.49:8085'
    USER       = 'user=sel373'
    PASSWORD   = 'pwd=sel373'
    SERVICE_SNAPSHOT  = '/snapshot.cgi?'
    SERVICE_CONTROL   = '/decoder_control.cgi?'
    CONTROL_MAP = {'up'   : '2', 
                   'down' : '0',
		   'left' : '6',
		   'right': '4'}
#Methods

    def __init__(self):
    
        self.protocol  = Camera.PROTOCOL
        self.ipAddress = Camera.IP_ADDRESS
        self.user      = Camera.USER
        self.password  = Camera.PASSWORD
        self.serviceSnapshot  = Camera.SERVICE_SNAPSHOT

    def get_frame(self):
        
        self.download_frame()
	return open('frame.jpg', 'rb').read()

    def download_frame(self):

	url = self.protocol + self.ipAddress + self.serviceSnapshot + self.user + '&' + self.password
	try:

            response = requests.get(url)
            if response.status_code == 200:
                with open('static/images/frame.jpg', 'wb') as file:
                    for chunk in response.iter_content():
                        file.write(chunk)

	except requests.exceptions.RequestException as error:

	    print error 

    @staticmethod
    def move_camera(direction):

	url = Camera.PROTOCOL + Camera.IP_ADDRESS + Camera.SERVICE_CONTROL + 'command=' + Camera.CONTROL_MAP[direction] + '&' + 'onestep=1' + '&' + Camera.USER + '&' + Camera.PASSWORD
        try:

            response = requests.get(url)

        except requests.exceptions.RequestException as error:

            print error

cam = Camera()
while(1):
    cam.download_frame()
