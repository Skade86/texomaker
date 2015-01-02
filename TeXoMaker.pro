INCLUDEPATH = include

# Configuration générale
QT += widgets xml network
DESTDIR = bin
TARGET = TeXoMaker
macx:ICON = TeXoMaker.icns
win32:RC_FILE += ressources.rc
TEMPLATE = app
CONFIG += app_bundle thread x86_64

# Librairies et Headers
macx:{
INCLUDEPATH += /usr/local/include/poppler/qt5
INCLUDEPATH += /usr/local/include/quazip
LIBS +=  -L/usr/local/lib/ -lpoppler-qt5
LIBS +=  -L/usr/local/lib/ -lquazip
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

include(version.pri)

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
    include/dirprefdialog.h \
    include/zipthread.h \
    include/unzipthread.h \
    include/generalsettingsdialog.h \
    include/advancedfilter.h \
    include/viewer.h
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
    sources/dirprefdialog.cpp \
    sources/zipthread.cpp \
    sources/unzipthread.cpp \
    sources/generalsettingsdialog.cpp \
    sources/advancedfilter.cpp \
    sources/viewer.cpp
RESOURCES = ressources.qrc
TRANSLATIONS += translations/texomaker_fr.ts
FORMS += ui/dirprefdialog.ui \
    ui/pdflatexlogdialog.ui \
    ui/exosheet.ui \
    ui/headerdialog.ui \
    ui/propertydialog.ui \
    ui/generalsettingsdialog.ui \
    ui/advancedfilter.ui \
    ui/viewer.ui


include(install.pri)
include(others.pri)
