@ECHO OFF

SET /p var=<build_number
SET /a var=%var%+1
ECHO %var%>build_number
SET version=#define BUILD %var%
ECHO %version%>include\version.h


