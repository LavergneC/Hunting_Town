from tkinter import *
import tkinter.messagebox
from ftplib import FTP
import time
from threading import Thread

class Interface(Frame):
    
    def __init__(self, session_ftp, fenetre, **kwargs):
        self.ftp = session_ftp
        
        Frame.__init__(self, fenetre, width=10000, height=5000, **kwargs)
        self.pack(fill = BOTH)
        
        self.message = Label(self, text = "Choix de la vidéo à lire ")
        self.message.pack()

        # Bouton envoi
        self.bouton_envoi = Button(self, text="Lire vidéo", command=self.envoi_video)
        self.bouton_envoi.pack(side="bottom")

        # Liste vidéos
        self.choix = StringVar()

        self.case1 = Radiobutton(self, text="Vidéo 1", variable=self.choix, value="v1")
        self.case1.pack()

        self.case2 = Radiobutton(self, text="Vidéo 2", variable=self.choix, value="v2")
        self.case2.pack()

        self.case3 = Radiobutton(self, text="Vidéo 3", variable=self.choix, value="v3")
        self.case3.pack()

        self.case4 = Radiobutton(self, text="Vidéo 4", variable=self.choix, value="v4")
        self.case4.pack()

        self.case5 = Radiobutton(self, text="Vidéo 5", variable=self.choix, value="v5")
        self.case5.pack()

        self.case6 = Radiobutton(self, text="Vidéo 6", variable=self.choix, value="v6")
        self.case6.pack()
    
    def envoi_video(self):
        tkinter.messagebox.showinfo("Info", "Vidéo lancée !")
        
        with open("envoi_videos", 'w', encoding="utf-8") as fic_ordre:
            fic_ordre.write("{}".format(self.choix.get()))

        with open("envoi_videos", 'rb') as fic_ordre: 
            ftp.storbinary("STOR ordre_video", fic_ordre)

        print("Envoi de la vidéo {}".format(self.choix.get()))
        
# FTP
ftp = FTP("demo.wftpserver.com")
ftp.login(user="demo-user", passwd="demo-user")
ftp.cwd("upload")

fenetre_principale = Tk()
UI = Interface(ftp, fenetre_principale)

def get_coor_gps():
    ftp.retrbinary("RETR coor_gps", open("coor_gps", "wb").write)
    fenetre_principale.after(7000, get_coor_gps)

fenetre_principale.after(7000, get_coor_gps)

UI.mainloop()