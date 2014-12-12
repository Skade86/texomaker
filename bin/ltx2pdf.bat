@echo off
echo Assignation des variables
Set texfile=%1%.tex
Set dvifile=%1%.dvi
Set psfile=%1%.ps
Set pdffile=%1%.pdf
Set latexbin=%2%
Set dvipsbin=%3%
Set ps2pdfbin=%4%
Set compileOptions=%5%

echo Compilation latex
%latexbin% %compileOptions% %texfile%

echo Compilation dvips
%dvipsbin% %dvifile% -o %psfile%

echo Compilation ps2pdf
%ps2pdfbin% -sPAPERSIZE#a4 %psfile% %pdffile%

echo Suppression des fichiers temporaires
del %dvifile%
del %psfile%
del %1%.aux
del %1%.log

