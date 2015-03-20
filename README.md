TeXoMaker
=========

Gestion de listes d'exercices sous LaTeX

Installation sous Linux
-----------------------

### Systèmes basés sur Debian (Ubuntu, ...)
```
apt-get install libpoppler-qt5-dev qt5-default libquazip0-dev
```

Dans le répertoire de texomaker :

```
qmake
make
```

L'exécutable est dans le répertoire bin

### Sous Fedora
```
yum install poppler-qt5-devel qt5-qtbase quazip-devel
```

Dans le répertoire de texomaker :

```
qmake-qt5
make
```

L'exécutable est dans le répertoire bin

### Sous Mageia

```
urpmi poppler-qt5-devel quazip-devel lib64qt5network-devel lib64qt5widgets-devel 
```

puis dans le répertoire des sources de texomaker: 

```
/usr/lib64/qt5/bin/qmake 
make
```


