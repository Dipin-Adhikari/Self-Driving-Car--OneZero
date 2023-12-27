import io
import socket
import struct
from PIL import Image
import cv2
import numpy as np
import time
import os
import math
from scipy import stats


serverSocket = socket.socket()
serverSocket.bind(('192.168.1.2', 8000))
serverSocket.listen(5)
c, addr = serverSocket.accept()
connection = c.makefile('rb')
cTime, pTime = 0, 0 


def findAngle(x1, y1, x2, y2):
    if round(x2-x1) != 0:
        m = float(y2-y1)/float(x2-x1)
        theta = (math.atan(m))*(180/np.pi)  
        if theta < 0: 
            return theta+180
        else:
            return theta
    else:
        return 90


def processImage(img):
    grayImg = cv2.cvtColor(img, cv2.COLOR_RGB2GRAY)
    histImg = cv2.equalizeHist(grayImg)
    blurImg = cv2.GaussianBlur(histImg, (9, 9), 0)
    ret, thresh = cv2.threshold(blurImg, 235, 255, cv2.THRESH_BINARY)
    imgCanny = cv2.Canny(thresh, 100, 300)
    lines = cv2.HoughLinesP(imgCanny, cv2.HOUGH_PROBABILISTIC, np.pi/180, 20, minLineLength=10, maxLineGap=40)
    cv2.imshow("Image Threshold", thresh)
    cv2.imshow("Image Canny", imgCanny)
    return lines




try:
    img = None
    while True:
        imageLen = struct.unpack('<L', connection.read(struct.calcsize('<L')))[0]
        if not imageLen:
            break
        imageStream = io.BytesIO()
        imageStream.write(connection.read(imageLen))
        imageStream.seek(0)
        img = np.array(Image.open(imageStream))
        img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        imgO = np.copy(img)
        
        lines = processImage(img)
        angleArray = np.array([])
    
        if lines is not None:
            for line in lines:
                for x1, y1, x2, y2 in line:
                    angle = findAngle(x1, y1, x2, y2)
                    angleArray = np.append(angleArray, np.array(angle))
                    cv2.line(imgO, (x1, y1+200), (x2, y2+200), (0, 255, 0), 2, cv2.LINE_AA)

            meanAngle = int(stats.trim_mean(angleArray, 0.15))
            print(meanAngle)
            c.send(meanAngle.to_bytes(1, 'big'))

        cTime = time.time()
        fps = int(1 / (cTime - pTime))
        pTime = cTime
        cv2.putText(imgO, str(fps), (100, 50), cv2.FONT_HERSHEY_SIMPLEX, 0.7, (0, 0, 255), 2)

        cv2.imshow("Original Image", imgO)
        cv2.imshow("Image", img)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

finally:
    connection.close()
    serverSocket.close()

cv2.destroyAllWindows()