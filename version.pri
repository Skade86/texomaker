# Numéros de version
DEFINES += MAJOR=4
DEFINES += MINOR=0
DEFINES += SUBMINOR=0

equals(DEBPACKING,"No") {
!win32 {
build_nr.commands = $$_PRO_FILE_PWD_/setVersion
}
win32:build_nr.commands = $$_PRO_FILE_PWD_/setVersion.bat
build_nr.depends = FORCE
QMAKE_EXTRA_TARGETS += build_nr
PRE_TARGETDEPS += build_nr
}
