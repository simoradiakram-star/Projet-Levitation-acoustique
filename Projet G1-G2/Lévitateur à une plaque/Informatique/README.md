# Pilotage Informatique - Lévitateur à une plaque

Ce dossier contient l'ensemble des ressources logicielles nécessaires au fonctionnement du prototype de lévitation acoustique à une plaque. 

## Structure du dossier

* **`Codes arduino mega/`** : Contient les scripts `.ino` pour l'Arduino Mega ainsi que les documentations d'explication de ces codes ;
* 
**`Passage_Phases_python_pin.ipynb`** : Notebook Python permettant de convertir les phases théoriques calculées en données binaires (octets) compréhensibles par l'Arduino.



---

## 🛠️ Instructions d'utilisation

### 1. Calcul des phases et conversion

Avant de téléverser le code sur l'Arduino, vous devez définir la position du piège acoustique :

1. Utilisez les scripts de calcul (Optimizer ou Vortex dans la partie physique théorique de la palque seule) pour obtenir les **phases réelles** (en radians) pour les 64 transducteurs ;
2. Ouvrez le notebook **`Passage_Phases_python_pin.ipynb`** ;
3. Renseignez vos phases réelles dans le tableau `Phi_64` ;
4. Exécutez le script pour générer le **bit buffer** (liste d'octets) ;
5. Copiez ces octets pour l'étape suivante.

### 2. Configuration de l'Arduino Mega

Le code de pilotage est une version optimisée du `DriverMega` original, nommée `NewDriverMega`.

1. Ouvrez le sketch dans le dossier `Codes arduino mega`.
2. Remplacez les octets de la variable `patterns` (ligne 24 environ) par ceux générés par le notebook.

3. **Note sur les motifs (patterns)** : Le code est configuré pour 2 motifs.
* Pour un piège statique : copiez deux fois le même set de phases.
* Pour un déplacement : renseignez deux sets de phases proches l'un de l'autre.

4. Sélectionnez la carte **Arduino Mega** dans l'IDE et téléversez le programme.


---

## ⚠️ Précautions

* 
**Alimentation** : Ne jamais dépasser **29V** sur la carte d'amplification.


* 
**Connectique** : Vérifiez la continuité des câbles en nappe, car des fils trop fins peuvent causer des pertes de signal.


* 
**Synchronisation** : Si le signal n'est pas stable à l'oscilloscope, ajustez ou supprimez le nombre de `nop` dans la fonction `WAIT()`.
