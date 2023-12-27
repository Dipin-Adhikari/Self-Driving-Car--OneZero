# Importing libraries
import io
import socket
import struct
import time
import picamera
import serial

# defining the port of arduino
port = serial.Serial("/dev/ttyUSB0", 9600, timeout=0.1)

# Establising connection with main computer
clientSocket = socket.socket()
clientSocket.connect(('192.168.1.2', 8000)) # IP adress of main computer
connection = clientSocket.makefile('wb')

# Function to send angle data to arduino received from compter.
def sendToArduino(data):
    port.write(data)

# Streaming live video to main computer.
try:
    with picamera.PiCamera() as camera:
        camera.resolution = (640, 480)
        camera.framerate = 30
        time.sleep(2)
        stream = io.BytesIO()
        # Use the video-port for captures.
        for foo in camera.capture_continuous(stream, 'jpeg',
                                             use_video_port=True):
            connection.write(struct.pack('<L', stream.tell()))
            connection.flush()
            stream.seek(0)
            connection.write(stream.read())

            stream.seek(0)
            stream.truncate()
            # Receiving the angle data from main computer and sending to arduino.
            command = clientSocket.recv(1024)

            if command is not None:
                sendToArduino(command)
            

    connection.write(struct.pack('<L', 0))
finally:
    connection.close()
    clientSocket.close()

