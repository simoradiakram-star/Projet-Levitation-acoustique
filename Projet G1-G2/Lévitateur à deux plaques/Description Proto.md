# Prototype 2 : Lévitateur à Double Plaque

Ce prototype constitue la phase 3 du projet. Il utilise deux matrices en vis-à-vis pour créer des ondes stationnaires et augmenter la force de lévitation.

## Organisation des dossiers

### 📁 Electronique
Ce dossier détaille l'adaptation matérielle pour deux plaques :
* **Schémas** : Plans pour les deux matrices (haute et basse) et leurs drivers améliorés.
* **Câblage** : Schéma de connexion spécifique pour relier les deux systèmes ensemble.

### 📁 Informatique
Ce dossier contient les outils pour la gestion synchronisée :
* **Codes Arduino (Master/Slave)** : Deux programmes distincts sont fournis. L'un pour la plaque du bas (Maître), l'autre pour celle du haut (Esclave).
* **Génération de phases** : Scripts adaptés pour calculer les pièges sur l'ensemble des 128 transducteurs simultanément.

### 📁 Physique théorique
* **Comparaison** : Documents montrant les gains de stabilité obtenus grâce aux ondes stationnaires par rapport au premier prototype.

---
## Objectif
Obtenir une lévitation très stable permettant de manipuler des liquides ou plusieurs objets à la fois.
