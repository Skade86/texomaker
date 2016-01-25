# Installation

QMAKE_STRIP = echo

macx {
target.path += /Applications
ltx2pdf.path = /Applications/TeXoMaker.app/Contents/Resources
ltx2pdf.files = bin/ltx2pdf
ltx2pdf.command = cd /Applications/TeXoMaker.app/Contents/Resources && chmod +x ltx2pdf
}

unix:!macx {
target.path += /usr/local/bin
ltx2pdf.path = /usr/local/bin
ltx2pdf.files = bin/ltx2pdf
}

!win32:INSTALLS += target ltx2pdf

