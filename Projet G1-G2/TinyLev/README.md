# Prototype 1 : TinyLev

Le **TinyLev** constitue la première étape concrète du projet. Il s'agit d'un lévitateur acoustique à ondes stationnaires dont la géométrie est fixe, permettant de valider les concepts physiques fondamentaux avant de passer à des systèmes dynamiques.

## Raison d'être

L'objectif principal de ce prototype était de :

* Confirmer la faisabilité de la lévitation d'objets millimétriques (billes de polystyrène) ;
* Se familiariser avec la manipulation des transducteurs ultrasonores à 40 kHz.

## Fonctionnalités

* **Focalisation géométrique** : La courbure de la structure imprimée en 3D assure la convergence des faisceaux ultrasonores vers un point focal central ;
* **Ondes stationnaires** : Création de nœuds de pression stables par interférence d'ondes permettant de piéger des particules à des positions fixes ;
* **Simplicité matérielle** : Pilotage via un Arduino Nano et une alimentation directe, sans besoin de calcul de phase complexe en temps réel.

## Limites

* **Position fixe** : Les pièges acoustiques sont statiques : il est impossible de déplacer la particule une fois lévitée ;
* **Absence de contrôle logiciel** : Le déphasage est induit par la position physique des transducteurs et non par le code.

## Évolution : Vers le Prototype 2 (Monoplaque)

Le succès du TinyLev a démontré que notre électronique de puissance était fiable. Cependant, pour répondre aux objectifs de manipulation dynamique, nous avons dû abandonner la géométrie fixe au profit d'une **matrice plane (8x8)** dont nous contrôlons chacun des transducteurs individuellement.

Le passage au prototype suivant permet d'introduire le **contrôle logiciel des phases**, offrant la flexibilité nécessaire pour déplacer les pièges acoustiques et manipuler les particules dans l'espace.
