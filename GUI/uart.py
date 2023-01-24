import serial
import serial.tools.list_ports
import time
from gui import CoolerScale, lightScale
from glob import T , L, Tnew, Lnew


def initComPort(port, baudrate = 9600, timeout = 1 ):
    # port = 'COM' + str(index)   //ttyUSB0
    serialInst.port = port
    serialInst.baudrate = 9600
    serialInst.open()


def findArduino(ports):
    commPort = 'None'
    for i in range(len(ports)):
        port = foundPorts[i]
        strPort = str(port)
        if 'Arduino' in strPort: 
            splitPort = strPort.split(' ')
            commPort = (splitPort[0])
    return commPort


def checkSerialPort():
    global T, L

    if serialInst.isOpen() and serialInst.in_waiting:
        recentPacket = serialInst.readline()
        recentPacketString = recentPacket.decode('utf').rstrip('\n')
        T, L = recentPacketString.split()
        T = T[1 : ]
        L = L[1 : ]  


def sendMessageTemp():
    serialInst.write("T")
    coolerPower = CoolerScale.get()
    serialInst.write(coolerPower.encode())


def sendMessageLight():
    serialInst.write("L")
    lightPower = lightScale.get()
    serialInst.write(lightPower.encode())


def checkStatus():
   global T, L, Tnew, Lnew

   if T != Tnew:
      T = Tnew
      sendMessageTemp()

   if L != Lnew:
    L = Lnew
    sendMessageLight()


foundPorts = serial.tools.list_ports.comports()        
connectPort = findArduino(foundPorts)

if connectPort != 'None':
    serialInst = serial.Serial()
    initComPort(connectPort)
    print('Connected to ' + connectPort)
else:
    serialInst = serial.Serial()
    print('Connection Issue!')


# serialInst.close()   


while True:
    checkSerialPort()