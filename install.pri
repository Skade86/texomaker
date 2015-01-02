# Installation
macx {
target.path += /Applications
ltx2pdf.path = /Applications/TeXoMaker.app/Contents/Resources
ltx2pdf.files = bin/ltx2pdf
ltx2pdf.command = cd /Applications/TeXoMaker.app/Contents/Resources && chmod +x ltx2pdf
}

unix:!macx {
target.path += /opt/TeXoMaker/bin
ltx2pdf.path = /opt/TeXoMaker/bin
ltx2pdf.files = bin/ltx2pdf
lt2pdf.extra = ln -s /opt/TeXoMaker/bin/TeXoMaker /usr/local/bin/TeXoMaker
}

!win32:INSTALLS += target ltx2pdf
