# Variable indiquant un empaquetage .deb ou pas : Yes ou No

DEBPACKING = No

# Configuration générale

QT += core gui widgets xml network webenginewidgets

DESTDIR = bin
TARGET = texomaker
macx:ICON = TeXoMaker.icns
win32:RC_FILE += ressources.rc
TEMPLATE = app
CONFIG += sdk_no_version_check
CONFIG += warn_off
# Librairies et Headers

INCLUDEPATH = include

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
