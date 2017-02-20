# Variable indiquant un empaquetage .deb ou pas : Yes ou No

DEBPACKING = No
QMAKE_MAC_SDK = macosx10.12
# Configuration générale
QT += widgets xml network
DESTDIR = bin
TARGET = TeXoMaker
macx:ICON = TeXoMaker.icns
win32:RC_FILE += ressources.rc
TEMPLATE = app
CONFIG += app_bundle thread x86_64

# Librairies et Headers

INCLUDEPATH = include

macx:{
INCLUDEPATH += /usr/local/include/poppler/qt5
LIBS +=  -L/usr/local/lib/ -lpoppler-qt5
}

unix:!macx{
INCLUDEPATH += /usr/include/poppler/qt5
LIBS+= -lpoppler-qt5
}

win32:{
INCLUDEPATH += include/poppler/qt5
LIBS += "$$_PRO_FILE_PWD_/lib/libpoppler-qt5.lib"
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
    include/generalsettingsdialog.h \
    include/advancedfilter.h \
    include/viewer.h \
    include/newexodialog.h \
    include/editwindow.h \
    include/highlighter.h
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
    sources/generalsettingsdialog.cpp \
    sources/advancedfilter.cpp \
    sources/viewer.cpp \
    sources/newexodialog.cpp \
    sources/editwindow.cpp \
    sources/highlighter.cpp
RESOURCES = ressources.qrc
TRANSLATIONS += translations/texomaker_fr.ts
FORMS += ui/dirprefdialog.ui \
    ui/pdflatexlogdialog.ui \
    ui/exosheet.ui \
    ui/headerdialog.ui \
    ui/propertydialog.ui \
    ui/generalsettingsdialog.ui \
    ui/advancedfilter.ui \
    ui/viewer.ui \
    ui/newexodialog.ui \
    ui/editwindow.ui

unix:!macx{
include(install.pri)
}

include(others.pri)

unix:!macx{
DISTFILES += \
    texomaker.desktop
}
