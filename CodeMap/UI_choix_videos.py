from tkinter import *

class Interface(Frame):

    def __init__(self, fenetre, **kwargs):
        Frame.__init__(self, fenetre, width=768, height=576, **kwargs)
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
        print("Envoi de la vidéo {}".format(self.choix.get()))


fenetre_principale = Tk()

interface = Interface(fenetre_principale)

# Affichage 
interface.mainloop()