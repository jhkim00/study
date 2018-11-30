from tkinter import *
from tkinter import ttk
import adbfunc
 
root = Tk()
root.title("Calculator")
root.geometry("200x200")

#버튼.
btnRadio = ttk.Button(root, text = "RADIO", command = lambda:adbfunc.broadcast("com.test.jhkim.avnstudy.action.KEY_EVT", {"type" : "RADIO"}))
btnRadio.grid(row = 0, column = 0)

btnMedia = ttk.Button(root, text = "MEDIA", command = lambda:adbfunc.broadcast("com.test.jhkim.avnstudy.action.KEY_EVT", {"type" : "MEDIA"}))
btnMedia.grid(row = 0, column = 1)

btnOsd = ttk.Button(root, text = "OSD", command = lambda:adbfunc.broadcast("com.test.jhkim.avnstudy.action.OSD_EVT"))
btnOsd.grid(row = 0, column = 2)

 
root.mainloop()