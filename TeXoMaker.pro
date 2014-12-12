DEPENDPATH = include ui sources
INCLUDEPATH = include
INCLUDEPATH += /usr/local/include/poppler/qt5
INCLUDEPATH += /usr/local/include/quazip
!win32 { LIBS += -L/usr/local/lib/ -lpoppler-qt5 }
!win32 { LIBS += -L/usr/local/lib/ -lquazip }
#!win32 { LIBS += -lz }
#macx { LIBS += "$$_PRO_FILE_PWD_/lib/libquazip.a" }
unix:!macx { LIBS+= -lquazip }
win32:INCLUDEPATH += include/poppler/qt4
win32:INCLUDEPATH += include/quazip
win32:LIBS += "$$_PRO_FILE_PWD_/lib/libpoppler-qt4.dll"
win32:LIBS += "$$_PRO_FILE_PWD_/lib/win32/quazip.dll"
win32:INCLUDEPATH += $$[QT_INSTALL_PREFIX]/src/3rdparty/zlib

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
DESTDIR = bin
TARGET = TeXoMaker
macx:ICON = TeXoMaker.icns
win32:RC_FILE += ressources.rc
TEMPLATE = app
CONFIG += app_bundle \
    thread x86_64
TRANSLATIONS += translations/texomaker_fr.ts
FORMS += ui/dirprefdialog.ui \
    ui/affichedialog.ui \
    ui/pdflatexlogdialog.ui \
    ui/exosheet.ui \
    ui/headerdialog.ui \
    ui/propertydialog.ui \
    ui/generalsettingsdialog.ui \
    ui/advancedfilter.ui
MOC_DIR = tmp
OBJECTS_DIR = tmp
RCC_DIR = tmp
UI_HEADERS_DIR = ui/include
QT += widgets xml network

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
