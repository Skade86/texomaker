# Numéros de version
DEFINES += MAJOR=3
DEFINES += MINOR=2
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
