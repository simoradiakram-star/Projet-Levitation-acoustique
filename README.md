# Système de Lévitation Acoustique

**Projet (G1 / G2) — Centrale Lille**

Développement d'un dispositif expérimental de manipulation sans contact par pinces acoustiques.

---

### À propos

Ce projet vise à concevoir et réaliser un système de lévitation acoustique à **faible coût**. L'objectif principal est de permettre la manipulation sans contact d'objets millimétriques (tels que des billes de polystyrène ou de petits insectes) dans l'air.

Ce travail s'inscrit dans la continuité des recherches du groupe **AIMAN-FILMS (IEMN)**, qui développe des technologies de pinces acoustiques pour des applications biomédicales (déplacement de cellules en milieu stérile). Notre prototype adapte ces concepts pour une application macroscopique dans l'air.

---

### 1. Objectifs

* **Conception :** Réaliser un prototype fonctionnel intégrant l'électronique et la structure mécanique.
* **Manipulation :** Réussir la lévitation stable d'objets millimétriques.
* **Démonstration :** Prouver la faisabilité d'applications potentielles (ex: manipulation d'échantillons fragiles).

### 2. Technologies et Compétences

Ce projet mobilise des compétences pluridisciplinaires :

* **Hardware / Électronique :** Soudure de cartes électroniques, pilotage de transducteurs.
* **Système embarqué :** Programmation sur Arduino.
* **Mécanique :** Modélisation et Impression 3D de la structure (support des transducteurs).
* **Physique :** Principes des ondes stationnaires et de la pression de radiation acoustique.

### 3. Partenaires et Encadrement

Ce projet est réalisé en partenariat avec le groupe **AIMAN-FILMS** de l'**IEMN** (Institut d'Électronique, de Microélectronique et de Nanotechnologie).

### 4. Contacts académiques

* **Olivier Bou Matar-Lacaze** (Centrale Lille / IEMN)
* **Abdelkrim Talbi** (Centrale Lille / IEMN)

---

Voici l'architecture globale du GitHub :

```
└── Projet-Levitation-acoustique
    ├── Lévitateur à deux plaques
    │   ├── Electronique
    │   │   ├── Circuits
    │   │   └── Composants
    │   ├── Informatique
    │   │   └── Communication 2 Arduino
    │   ├── Notices
    │   ├── Physique Théorique
    │   │   ├── Calcul des phases
    │   │   └── Photos
    │   └── Résultats  <-- (Vidéos de lévitation, rapports de tests)
    │
    ├── Lévitateur à une plaque
    │   ├── Electronique
    │   │   ├── Circuits
    │   │   └── Composants
    │   ├── Informatique
    │   │   ├── Code Arduino Mega
    │   │   │   ├── DriverMega
    │   │   │   └── NewDriverMega
    │   │   └── Codes Arduino Mega
    │   │       └── DriverMega
    │   ├── Notices
    │   ├── Physique théorique
    │   │   ├── Calcul des phases
    │   │   └── Photos
    │   └── Résultats  <-- (Vidéos, mesures de précision)
    │
    └── TinyLev
        └── Résultats  <-- (Démonstration du prototype compact)
```
