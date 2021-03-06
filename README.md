# Hunting_Town
## Prototypage d'un système embarqué pour Hunting-Town


__Objectif : Mettre en place un système couvrant les nombreux besoins d'une session d'escape game dans la ville.__


__Overview__

 1. Modules nécessaires :
 
    * {Comm} Communication entre les joueurs et le maitre du jeu
    * {GPS} Récupération de la position des joueurs
    * {Vidéo} Depuis le PC du maitre du jeu, lancer une vidéo sur l'écran des joueurs (son compris)
    * {Appel} Récupération du son des joueurs
  
 2. Besoins :
 
    * Alimentation
    * Cartes compactes
    * Gérer plusieurs équipes en même temps
  
  
__Notre système__

 * Le prototype que nous avons développé est composé de deux cartes:
    * Une carte STM32F4 Discovery associée au "Discovery Shield" de MikroElektronika
    * Une BeagleBone Back wireless
  La carte STM32 est la carte principale hébergant toutes les communications et faisant office de relai, de son coté le mini-ordinateur 
BeagleBone devrait être fixé sur le bras d'un joueur et ne possède qu'une tâche : Afficher les vidéos à l'aide d'un écran.


__{Comm} Communication entre maître du jeu et joueurs__

Cette communication est assurée par internet, le maître du jeu est instalé sur sont PC, du coté joueur la STM32 est équipée d'un module 4G :

  * Le "MIKROE-2527" de MikroElektronika, ce module embarque le composant "LARA-R211". Le composant communique par UART à l'aide de commandeAT. Plus d'informations sur les commandes AT dans "Rapport_Gibaud_Nicolas.pdf". 
  Le driver se découpe en deux parties, la première se trouve dans les fichiers AT_command.h et AT_command.c, la seconde dans le main.c au niveau du callBack de l'UART dans le premier "if".
  
 * Nous avons développé deux versions de la communication : 
    * Communication avec une l'API REST d'Hologram, notre founisseur d'internet au début du projet
    * Communication grâce à un serveur ftp


__{GPS} Module GPS__

  L'objectif de cette partie est de récupérer la possition GPS des joueurs et de la transmettre à maitre du jeu.
  Pour la mise en place du GPS nous avons choisis le module "NANO GPS click" de MikroElektronika, ce module embarque le composant "Nano Hornet". Plus d'information sur la mise en place du driver de ce composant dans "Rapport_Venier_Antoine.pdf".
  L'implémentation est située également dans le main.c au niveau du callBack de l'UART, cette fois dans le "else if". On trouve également la fonction d'initialisation dans le main.c juste après la fonction main.
  On retrouve enfin le code pour l'interface graphique codé maitre du jeu dans le dossier "cote map". 
  
  En l'état le module GPS montre des difficultés à récupérer avec assez de présision la position des joueurs dans les rues étroites où se détroule le jeu, il faut songer à utiliser un autre moyen de géolocalisation. 
   Nous avons testé le module "3G SARA click" de MikroElektronika embarquant le composant "SARA U-201" qui possède une technologie prometteuse nommée "cellLocate" qui utilise à la fois le GPS et les antennes 3G pour se localiser, après examen de la documentation, le module de MikroElektronika n'embarque PAS de composant GPS rendant cette fonctionalité inutilisable.


 __{Vidéo} Module Vidéo__
 
  L'objectif de cette partie est de créer une chaîne de communication débutant coté maitre du jeu, passant par internet jusqu'à la STM32 puis passant par Bluetooth jusqu'à la BeagleBone où la vidéo indiquée soit être lu.
  
  L'affichage est gérer par l'écran et la cape "GEN4-4DCAPE-43T" achetés ici : 
  <!-- -->
       https://www.digikey.fr/product-detail/fr/4d-systems-pty-ltd/GEN4-4DCAPE-43T/1613-1386-ND/7360959 
  <!-- -->

  Pour le son, nous n'avons pas reçu notre commande afin de tester, nous avions pensé utiliser un connecteur USB mâle vers jack femelle sur lequel nous aurions pu branche la petit enceinte.
  
  
__/!\ Branche extra_demo /!\\__

  La communication par internet est gérée par un serveur ftp publique (assez peu sûr) sur lequel est posté la valeur de la vidéo a jouer depuis l'interface graphique {NICO}.
  Sur la STM32 le fichier est récupéré à l'aide de commandes AT dans la fonction getVideo_ftp déclarée dans AT_commande.h, plus d'information dans "Rapport_Gibaud_Nicolas.pdf". 
  La partie Bluetooth est développée sur le module "BLE P click" de MikroElektronika qui embarque le composant "nRF8001". Le driver est basé sur le ce gitHub :
  
  <!-- -->
       https://github.com/sgreg/avr-nrf8001 
 <!-- --> 
 
  Mais il est très grandement modifié. Le code est situé dans les fichiers nrf.h et nrf.c.
  La partie BeagleBone consiste en deux étapes : 
  * L'automatisation de la partie bluetooth dans un script en langage "expect" que l'on trouve dans "script_blue.sh"
  * La partie de lancement de la video qui est un script bash : "lancerVideo.sh". Dans le même dossier on trouve également deux fichiers utiles à la personnalisation du bureau debian pour lancer le script simple en deux cliques sur l'écran.
  
__{Appel} Module Appel__

La récupération du son ambiant des joueurs se fait via un appel passé depuis le maître du jeu au module 4G placé sur les joueurs. Suite à quelques problèmes, on n'a pas eu la possibilité de tester les configurations implémentées du module 4G pour le microphone et la gestion des appels. Une configuration de base a été implémenté sur la branche dédié aux appels.
Toutes les commandes AT utiles à la configuration du module 4G pour la gestion des appels est détaillé sur le rapport "Rapport_Gibaud_Nicolas.pdf". 
  
# Conclusion

  Il reste encore à faire est très importantes parties de conception de carte pour rendre compacte la carte STM32 et ses modules. 
  Coté implémentation, la partie GPS est potentiellemnt à refaire en fonction de futurs choix. 
  La partie Vidéo fonctionne mais louer un serveur ftp privé semble judicieux pour une utilisation sur le long terme. De son coté la communication 4G très flexible peut permettre de nombreuses choses sans être modifiée. Enfin pour les appels {NICO}
