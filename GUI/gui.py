from tkinter.ttk import *
from tkinter import *
from tkinter import _flatten, _join, _stringify, _splitdict
import tkinter
import ttk


from tkinter import *
import tkinter as tk
from tkinter import ttk
import awesometkinter as atk


def showPower():  
   sel = "Temp = " + str(CoolerValue.get())  
   CoolerLabel.config(text = sel)  

def showLight():  
   sel = "Light = " + str(lightValue.get())  
   lightLabel.config(text = sel)  


root = tk.Tk()
root.config(background = atk.DEFAULT_COLOR)
root.geometry("700x400+400+250")
root.title("Manage Temperature and Light System")
s = ttk.Style()
s.theme_use("default")


###### f1
f1 = atk.Frame3d(root)
f1.pack(side = 'left', expand = True, fill = 'both', padx = 3, pady = 3)

currentCoolerValue = 20
CoolerProgress = atk.RadialProgressbar3d(f1, fg = 'purple', size = 170)
CoolerProgress.pack(padx = 20, pady = 20)
CoolerProgress.set(currentCoolerValue)
CoolerProgress.stop()

CoolerValue = DoubleVar()  
CoolerScale = Scale(f1, variable = CoolerValue, from_ = 0, to = 100, orient = HORIZONTAL, length = 300, showvalue = 1)
CoolerScale.pack(padx = 15, anchor = CENTER)  
CoolerScale.set(currentCoolerValue)
  
CoolerBtn = atk.Button3d(f1, text = "Set", command = showPower)
CoolerBtn.pack(anchor = CENTER, pady = 10)

CoolerLabel = Label(f1)  
CoolerLabel.pack(pady = 5)  


####### f2
f2 = atk.Frame3d(root)
f2.pack(side = 'left', expand = True, fill = 'both', padx = 3, pady = 3)

currentLightValue = 40
lightProgress = atk.RadialProgressbar3d(f2, fg = 'yellow', size = 170)
lightProgress.pack(padx = 20, pady = 20)
lightProgress.set(currentLightValue)
lightProgress.stop()

lightValue = DoubleVar() 
light = Scale(f2, variable = lightValue, from_ = 0, to = 100, orient = HORIZONTAL, length= 300, showvalue = 1)
light.pack(anchor = CENTER)  
light.set(currentLightValue)

atk.Button3d(f2, text = "Set", command = showLight).pack(anchor = CENTER, pady = 10)


lightLabel = Label(f2)  
lightLabel.pack(pady = 5) 
root.mainloop()




