from tkinter.ttk import *
from tkinter import ttk
from tkinter import *
import tkinter
import tkinter as tk
import awesometkinter as atk
from glob import T , L, Tnew, Lnew


def showPower():  
   sel = "Power = " + str(CoolerScale.get())  
   CoolerLabel.config(text = sel)
   CoolerProgress.set(CoolerScale.get())
   Tnew = CoolerScale.get()


def showLight():  
   sel = "Light = " + str(lightScale.get())  
   lightLabel.config(text = sel)
   lightProgress.set(lightScale.get())
   Lnew = lightScale.get()


# Root 
root = tk.Tk()
root.config(background = atk.DEFAULT_COLOR)
root.geometry("700x440+400+250")
root.title("Manage Temperature and Light System")
s = ttk.Style()
s.theme_use("default")


# Frame f1 
f1 = atk.Frame3d(root)
f1.pack(side = 'left', expand = True, fill = 'both', padx = 3, pady = 3)

temp = tk.Label(f1, text = "Cooler", fg = "black", bg = '#0dbee1', font=("Helvetica", 14))
temp.pack(fill = 'x', padx = 5)

currentCoolerValue = T
CoolerProgress = atk.RadialProgressbar3d(f1, fg = '#0dbee1', size = 170)
CoolerProgress.pack(padx = 20, pady = 20)
CoolerProgress.set(currentCoolerValue)
CoolerProgress.stop()

CoolerValue = DoubleVar()  
CoolerScale = Scale(f1, variable = CoolerValue, from_ = 0, to = 100, orient = HORIZONTAL, length = 300, showvalue = 1)
CoolerScale.pack(anchor = CENTER, padx = 10, pady = 10)  
CoolerScale.set(currentCoolerValue)
  
CoolerBtn = atk.Button3d(f1, text = "Set", command = showPower)
CoolerBtn.pack(anchor = CENTER, pady = 10)

CoolerLabel = Label(f1)  
CoolerLabel.pack(pady = 5)  


# Frame f2
f2 = atk.Frame3d(root)
f2.pack(side = 'left', expand = True, fill = 'both', padx = 3, pady = 3)

temp = tk.Label(f2, text = "Light", bg = '#f2e05b', fg = "black", font=("Helvetica", 14))
temp.pack(fill = 'x', padx = 5)

currentLightValue = L
lightProgress = atk.RadialProgressbar3d(f2, fg = '#f2e05b', size = 170)
lightProgress.pack(padx = 20, pady = 20)
lightProgress.set(currentLightValue)
lightProgress.stop()

lightValue = DoubleVar()
lightScale = Scale(f2, variable = lightValue, from_ = 0, to = 100, orient = HORIZONTAL, length= 300, showvalue = 1)
lightScale.pack(anchor = CENTER, padx = 10, pady = 10)  
lightScale.set(currentLightValue)

lightBtn = atk.Button3d(f2, text = "Set", command = showLight)
lightBtn.pack(anchor = CENTER, pady = 10)

lightLabel = Label(f2)  
lightLabel.pack(pady = 5) 

root.update() 
root.mainloop()