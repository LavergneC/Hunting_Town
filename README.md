# Hunting_Town
Prototypage d'un système embarqué pour Hunting-town

Objectif : Mettre en place un système couvrant les nombreux besoins d'une session d'escape game dans la ville.

Overview
  Modules nécéssaires :
    - <Comm> Communication entre les joueurs et le maitre du jeu
    - <GPS> Récupération de la position des joueurs
    - <Vidéo> Depuis le PC du maitre du jeu, lancer une vidéo sur l'écran des joueurs (son compris)
    - <Appel> Récupération du son des joueurs


  Besoins :
    - Alimentation
    - Cartes compactes
    - Gérer plusieurs équipes en même temps
Notre système
  Le prototype que nous avons développé est composé de deux cartes:
    - Une carte STM32F4 Discovery assicoée au "Discovery Shield" de MikroElektronika
    - Une BeagleBone Back wireless
  La carte STM32 est la carte principale, hébergant toutes les communications et faisant office de relai, de sont coté le mini-ordinateur 
BeagleBone devrait être fixé sur le bras d'un joueur et ne possède qu'une tâche : Afficher les vidéos à l'aide d'un écran.

<Comm>unication entre maître du jeu et joueurs
  Cette communication est assurée par internet, le maître du jeu est instalé sur sont PC, du coté joueur la STM32 est équipée d'un module 4G : le "MIKROE-2527" de MikroElektronika, ce module embarque le composant "LARA-R211". Le composant communique par UART à l'aide de commandeAT. Plus d'information sur les commandes AT dans "Rapport_Gibaud_Nicolas.pdf". 
  Le driver se découpe en deux parties, la première se trouve dans les fichiers AT_command.h et AT_command.c, la seconde dans le main.c au niveau du callBack de l'UART dans le premier "if".

Module <GPS>
  L'objectif de cette partie est de récupérer la possition GPS des joueurs et de la transmettre à maitre du jeu.
  Pour la mise en place du GPS nous avons choisis le module "NANO GPS click" de MikroElektronika, ce module embarque le composant "Nano Hornet". Plus d'information sur la mise en place du driver de ce composant dans "Rapport_Venier_Antoine.pdf". L'implémentation es situe également dans le main.c au niveau du callBack de l'UART, cette fois dans le "else if". On trouve également la fonction d'initialisation dans le main.c juste après la fonction main.
  En l'état le module GPS montre des difficultés à récupérer avec assez de présision la position des joueurs dans les rues étroites où se détroule le jeu, il faut songer à utiliser un autre moyen de géolocalisation. 
   Nous avons testé le module "3G SARA click" de MikroElektronika embarquant le composant "SARA U-201" qui possède une technologie promèteuse nommée "cellLocate" qui utilise à la fois le GPS et les antennes 3G pour se localiser, après examen de la documentation, le module de MikroElektronika n'embarque PAS de composant GPS rendant cette fonctionalité inutilisable.

 Module <Vidéo>
 Module <Appel>
