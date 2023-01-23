import serial
import serial.tools.list_ports
import time
import serial
import tkinter as tk
import gui
from gui import showPower, showLight
import functools
from tkinter import *
from glob import T , L, Tnew, Lnew


# ports = serial.tools.list_ports.comports()
# serialObj = serial.Serial()

# root = Tk()
# root.config(bg='grey')

# def initComPort(index):
#     currentPort = str(ports[index])
#     comPortVar = str(currentPort.split(' ')[0])
#     print(comPortVar)
#     serialObj.port = comPortVar
#     serialObj.baudrate = 9600
#     serialObj.open()

# for onePort in ports:
#     comButton = Button(root, text=onePort, font=('Calibri', '13'), height=1, width=45, command = functools.partial(initComPort, index = ports.index(onePort)))
#     comButton.grid(row=ports.index(onePort), column=0)

# dataCanvas = Canvas(root, width=600, height=400, bg='white')
# dataCanvas.grid(row=0, column=1, rowspan=100)

# vsb = Scrollbar(root, orient='vertical', command=dataCanvas.yview)
# vsb.grid(row=0, column=2, rowspan=100, sticky='ns')

# dataCanvas.config(yscrollcommand = vsb.set)



def findArduino(ports):
    commPort = 'None'
    for i in range(len(ports)):
        port = foundPorts[i]
        strPort = str(port)
        if 'Arduino' in strPort: 
            splitPort = strPort.split(' ')
            commPort = (splitPort[0])
    return commPort


def initComPort(port, baudrate = 9600, timeout = 1 ):
    # port = 'COM' + str(index)
    serialInst.port = port
    serialInst.baudrate = 9600
    serialInst.open()


def checkSerialPort():
    if serialInst.isOpen() and serialInst.in_waiting:
        recentPacket = serialInst.readline()
        recentPacketString = recentPacket.decode('utf').rstrip('\n')
        T, L = recentPacketString.split()
        T = T[1 : ]
        L = L[1 : ]  

foundPorts = serial.tools.list_ports.comports()        
connectPort = findArduino(foundPorts)

if connectPort != 'None':
    serialInst = serial.Serial()
    initComPort(connectPort)
    print('Connected to ' + connectPort)
else:
    serialInst = serial.Serial()
    print('Connection Issue!')


def sendMessageTemp():
    serialInst.write("T")
    coolerPower = showPower()
    serialInst.write(coolerPower.encode())


def sendMessageLight():
    serialInst.write("L")
    lightPower = showLight()
    serialInst.write(lightPower.encode())


def checkStatus():
   if T != Tnew:
      sendMessageTemp()

   if L != Lnew:
      sendMessageLight()


# root.update()            
# serialInst.close()   


while True:
    checkSerialPort()


               