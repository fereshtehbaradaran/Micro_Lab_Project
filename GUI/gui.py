from tkinter.ttk import *
from tkinter import *
from tkinter import _flatten, _join, _stringify, _splitdict
import tkinter
import ttk

# class Widget(tkinter.Widget):
#     """Base class for Tk themed widgets."""

#     def __init__(self, master, widgetname, kw=None):
#         """Constructs a Ttk Widget with the parent master.

#         STANDARD OPTIONS

#             class, cursor, takefocus, style

#         SCROLLABLE WIDGET OPTIONS

#             xscrollcommand, yscrollcommand

#         LABEL WIDGET OPTIONS

#             text, textvariable, underline, image, compound, width

#         WIDGET STATES

#             active, disabled, focus, pressed, selected, background,
#             readonly, alternate, invalid
#         """
#         master = setup_master(master)
#         tkinter.Widget.__init__(self, master, widgetname, kw=kw)


#     def identify(self, x, y):
#         """Returns the name of the element at position x, y, or the empty
#         string if the point does not lie within any element.

#         x and y are pixel coordinates relative to the widget."""
#         return self.tk.call(self._w, "identify", x, y)


#     def instate(self, statespec, callback=None, *args, **kw):
#         """Test the widget's state.

#         If callback is not specified, returns True if the widget state
#         matches statespec and False otherwise. If callback is specified,
#         then it will be invoked with *args, **kw if the widget state
#         matches statespec. statespec is expected to be a sequence."""
#         ret = self.tk.getboolean(
#                 self.tk.call(self._w, "instate", ' '.join(statespec)))
#         if ret and callback is not None:
#             return callback(*args, **kw)

#         return ret


#     def state(self, statespec=None):
#         """Modify or inquire widget state.

#         Widget state is returned if statespec is None, otherwise it is
#         set according to the statespec flags and then a new state spec
#         is returned indicating which flags were changed. statespec is
#         expected to be a sequence."""
#         if statespec is not None:
#             statespec = ' '.join(statespec)

#         return self.tk.splitlist(str(self.tk.call(self._w, "state", statespec)))

# class Scale(Widget, tkinter.Scale):
#     """Ttk Scale widget is typically used to control the numeric value of
#     a linked variable that varies uniformly over some range."""

#     def __init__(self, master=None, **kw):
#         """Construct a Ttk Scale with parent master.

#         STANDARD OPTIONS

#             class, cursor, style, takefocus

#         WIDGET-SPECIFIC OPTIONS

#             command, from, length, orient, to, value, variable
#         """
#         Widget.__init__(self, master, "ttk::scale", kw)


#     def configure(self, cnf=None, **kw):
#         """Modify or query scale options.

#         Setting a value for any of the "from", "from_" or "to" options
#         generates a <<RangeChanged>> event."""
#         retval = Widget.configure(self, cnf, **kw)
#         if not isinstance(cnf, (type(None), str)):
#             kw.update(cnf)
#         if any(['from' in kw, 'from_' in kw, 'to' in kw]):
#             self.event_generate('<<RangeChanged>>')
#         return retval


#     def get(self, x=None, y=None):
#         """Get the current value of the value option, or the value
#         corresponding to the coordinates x, y if they are specified.

#         x and y are pixel coordinates relative to the scale widget
#         origin."""
#         return self.tk.call(self._w, 'get', x, y)


# length_input.grid(row = 2, column = 1, pady=40, padx = 40)



from tkinter import *
import tkinter as tk
from tkinter import ttk
import awesometkinter as atk


def showTemp():  
   sel = "Temp = " + str(tempValue.get())  
   tempLabel.config(text = sel)  

def showLight():  
   sel = "Light = " + str(lightValue.get())  
   lightLabel.config(text = sel)  


root = tk.Tk()
root.config(background=atk.DEFAULT_COLOR)
root.geometry("700x400+400+250")
root.title("Manage Temperature and Light System")
s = ttk.Style()
s.theme_use('default')


###### f1
f1 = atk.Frame3d(root)
f1.pack(side='left', expand=True, fill='both', padx=3, pady=3)

tempProgress = atk.RadialProgressbar3d(f1, fg='purple', size=170)
tempProgress.pack(padx=20, pady=20)
tempProgress.set(50)
tempProgress.stop()

tempValue = DoubleVar()  
temp = Scale(f1, variable = tempValue, from_ = 25, to = 100, orient = HORIZONTAL, length= 300, showvalue = 1)
temp.pack(padx=15, anchor=CENTER)  
temp.set(50)
  
tempBtn = atk.Button3d(f1, text="Temperature", command=showTemp)
tempBtn.pack(anchor=CENTER, pady=10)

tempLabel = Label(f1)  
tempLabel.pack(pady=5)  


####### f2
f2 = atk.Frame3d(root)
f2.pack(side='left', expand=True, fill='both', padx=3, pady=3)

lightProgress = atk.RadialProgressbar3d(f2, fg='yellow', size=170)
lightProgress.pack(padx=20, pady=20)
lightProgress.set(50)
lightProgress.stop()

lightValue = DoubleVar() 
light = Scale(f2, variable = lightValue, from_ = 25, to = 100, orient = HORIZONTAL, length= 300, showvalue = 1)
light.pack(anchor=CENTER)  
light.set(40)

atk.Button3d(f2, text="Brightness", command=showLight).pack(anchor=CENTER, pady=10)


lightLabel = Label(f2)  
lightLabel.pack(pady=5) 
root.mainloop()




