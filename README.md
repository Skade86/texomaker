Installation
============

Dans tout les cas, noter que TeXoMaker est fait pour être utilisé avec LaTeX et qu'une distribution LaTeX doit être présente.

Nous recommandons fortement l'utilisation de TeXLive pour Linux et Mac.
Pour Windows, MikTeX est recommandé (mais TeXlive existe également).

Installation sous Windows
-----------------------

Téléchargez l'installateur, et double-cliquez dessus !

Installation sous Mac
-----------------------

Téléchargez l'image disque au format dmg.

Ouvrez-là et déplacez l'application dans le répertoire Applications.

Installation sous Linux
-----------------------
### Sous Ubuntu (16.04)

Il existe désormais un ppa qui permet l'installation de TeXoMaker avec apt-get, aptitude ou votre application de gestion des logiciels habituelle.

Pour ajouter le ppa à vos sources : 

Sois en ligne de commande avec :

```
sudo add-apt-repository ppa:gwenael-m/freebyte
sudo apt-get update
```

puis 

```
sudo apt-get install texomaker
```

Sois en mode graphique en lançant "Logiciels et mises à jour".

Choisir "Autres logiciels" puis le bouton "Ajouter...", et entrer
 
```
ppa:gwenael-m/freebyte
```


Vérifier que le dépôt a bien été ajouté.

Après une mise à jour de la liste des logiciels, TeXoMaker est prêt à être installé.


### Compilation à partir des sources

Commencer par télécharger l'archive des sources sur la page des [téléchargements](https://github.com/Domlol/texomaker/releases).

Décompresser l'archive. La suite dépend de la distribution : 

#### Systèmes basés sur Debian (Ubuntu, ...)
```
apt-get install libpoppler-qt5-dev qt5-default g++
```

Dans le répertoire de texomaker :

```
qmake
make
```

L'exécutable est dans le répertoire bin

#### Sous Fedora
```
yum install poppler-qt5-devel qt5-qtbase
```

Dans le répertoire de texomaker :

```
qmake-qt5
make
```

L'exécutable est dans le répertoire bin

#### Sous Mageia

```
urpmi poppler-qt5-devel lib64qt5network-devel lib64qt5widgets-devel 
```

puis dans le répertoire des sources de texomaker: 

```
/usr/lib64/qt5/bin/qmake 
make
```