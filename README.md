# Hunting_Town
Prototypage d'un système embarqué pour Hunting-town

Objectif : Mettre en place un système couvrant les nombreux besoins d'une session d'escape game dans la ville.

Overview
  Modules nécéssaires :
    - [Appel] Récupération du son des joueurs
    - [GPS] Récupération de la position des joueurs
    - [Vidéo] Depuis le PC du maitre du jeu, lancer une vidéo sur l'écran des joueurs (son compris)
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

Communication entre maître du jeu et joueurs
  Cette communication est assurée par internet, le maître du jeu est instalé sur sont PC, du coté joueur la STM32 est équipée d'un module
4G : le "MIKROE-2527" de MikroElektronika, ce module embarque le composant "LARA-R211".
Le composant communique par UART à l'aide de commandeAT. Plus d'information sur les commandeAT dans le Rapport
Module GPS
  Pour la mise en place du GPS nos
