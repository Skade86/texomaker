DEPENDPATH = include ui sources
INCLUDEPATH = include

macx:{
INCLUDEPATH += /usr/local/include/poppler/qt5
INCLUDEPATH += /usr/local/include/quazip
LIBS += -L/usr/local/lib/ -lpoppler-qt5
LIBS += -L/usr/local/lib/ -lquazip
}

unix:!macx{
INCLUDEPATH += /usr/include/poppler/qt5
INCLUDEPATH += /usr/include/quazip
LIBS+= -lquazip-qt5 -lpoppler-qt5
}

win32:{
INCLUDEPATH += include/poppler/qt5
INCLUDEPATH += include/quazip
LIBS += "$$_PRO_FILE_PWD_/lib/libpoppler-qt5.dll"
LIBS += "$$_PRO_FILE_PWD_/lib/quazip.dll"
}

# Numéros de version
DEFINES += MAJOR=2
DEFINES += MINOR=2
!win32 {
build_nr.commands = $$_PRO_FILE_PWD_/setVersion
}
win32:build_nr.commands = $$_PRO_FILE_PWD_/setVersion.bat
build_nr.depends = FORCE
QMAKE_EXTRA_TARGETS += build_nr
PRE_TARGETDEPS += build_nr

# Fichiers sources
HEADERS =  include/xmldomhandler.h \
    include/version.h \
    include/tableview.h \
    include/propertydialog.h \
    include/preferences.h \
    include/pdflatexlogdialog.h \
    include/mpthread.h \
    include/mainwindow.h \
    include/importthread.h \
    include/importer.h \
    include/headerdialog.h \
    include/exosmodel.h \
    include/exosheet.h \
    include/documentwidget.h \
    include/dirprefdialog.h \
    include/affichedialog.h \
    include/zipthread.h \
    include/unzipthread.h \
    include/generalsettingsdialog.h \
    include/advancedfilter.h
SOURCES =  sources/xmldomhandler.cpp \
    sources/tableview.cpp \
    sources/propertydialog.cpp \
    sources/preferences.cpp \
    sources/pdflatexlogdialog.cpp \
    sources/mpthread.cpp \
    sources/mainwindow.cpp \
    sources/main.cpp \
    sources/importthread.cpp \
    sources/importer.cpp \
    sources/headerdialog.cpp \
    sources/exosmodel.cpp \
    sources/exosheet.cpp \
    sources/documentwidget.cpp \
    sources/dirprefdialog.cpp \
    sources/affichedialog.cpp \
    sources/zipthread.cpp \
    sources/unzipthread.cpp \
    sources/generalsettingsdialog.cpp \
    sources/advancedfilter.cpp
RESOURCES = ressources.qrc
TRANSLATIONS += translations/texomaker_fr.ts
FORMS += ui/dirprefdialog.ui \
    ui/affichedialog.ui \
    ui/pdflatexlogdialog.ui \
    ui/exosheet.ui \
    ui/headerdialog.ui \
    ui/propertydialog.ui \
    ui/generalsettingsdialog.ui \
    ui/advancedfilter.ui

# Configuration générale
QT += widgets xml network
DESTDIR = bin
TARGET = TeXoMaker
macx:ICON = TeXoMaker.icns
win32:RC_FILE += ressources.rc
TEMPLATE = app
CONFIG += app_bundle \
    thread x86_64

# Fichiers objets temporaires
MOC_DIR = tmp
OBJECTS_DIR = tmp
RCC_DIR = tmp
UI_DIR = $$_PRO_FILE_PWD_/ui/include

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

OTHER_FILES += \
    setVersion \
    setVersion.bat \
    build_number
