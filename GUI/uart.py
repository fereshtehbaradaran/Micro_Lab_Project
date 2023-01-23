import serial
import serial.tools.list_ports
import time
import serial
import tkinter as tk


# ports = serial.tools.list_ports.comports()
# serialInst = serial.Serial()

# portsList = []

# for onePort in ports:
#     portsList.append(str(onePort))
#     print(str(onePort))

# val = input("Select Port: COM")
# portVar = ''
# for x in range(len(portsList)):
#     if portsList[x].startswith("COM" + str(val)):
#         portVar = "COM" + str(val)
#         print(portVar)

# serialInst.baudrate = 9600
# serialInst.port = portVar
# serialInst.open()

# while True:
#  if serialInst.in_waiting:
#   packet = serialInst.readline()
#   print(packet.decode('utf').rstrip('\n'))



import serial.tools.list_ports
import functools
from tkinter import *
import serial

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

# dataFrame = Frame(dataCanvas, bg="white")
# dataCanvas.create_window((10,0),window=dataFrame,anchor='nw')

# def checkSerialPort():
#     if serialObj.isOpen() and serialObj.in_waiting:
#         recentPacket = serialObj.readline()
#         recentPacketString = recentPacket.decode('utf').rstrip('\n')
#         Label(dataFrame, text=recentPacketString)

# while True:
#     root.update()
#     checkSerialPort()
#     dataCanvas.config(scrollregion=dataCanvas.bbox("all"))


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


def send_message():
    message = entry.get()
    ser.write(message.encode())
    response = ser.readline()
    label.config(text=response)


# serialInst.write(b'Hello Arduino')        
# response = serialInst.readline()          
# print(response)                   
# serialInst.close()   


while True:
    checkSerialPort()


               







root = tk.Tk()
root.title("Arduino UART")

entry = tk.Entry(root)
entry.pack()

send_button = tk.Button(root, text="Send", command=send_message)
send_button.pack()

label = tk.Label(root, text="No response yet.")
label.pack()

root.mainloop()