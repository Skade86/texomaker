# Installation

QMAKE_STRIP = echo

unix:!macx {
target.path += /usr/local/bin
ltx2pdf.path = /usr/local/bin
ltx2pdf.files = bin/ltx2pdf
}

!win32:INSTALLS += target ltx2pdf

