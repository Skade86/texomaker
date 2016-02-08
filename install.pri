# Installation

# L'executable
target.path += /usr/bin/

# Le script ltx2pdf
ltx2pdf.path = /usr/bin/
ltx2pdf.files = bin/ltx2pdf

# Ubuntu desktop
icon.path = /usr/share/pixmaps/
icon.files = images/TeXoMaker.png
desktop.path = /usr/share/applications/
desktop.files = texomaker.desktop

INSTALLS += target ltx2pdf icon desktop

