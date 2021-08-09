echo Running bison on input %1%2.y
bison %1%2.y
mkdir ..\..\include\include_gen
copy %2.tab.c ..\..\src\%2.tab.cpp
copy %2.tab.h ..\..\include\include_gen\%2.tab.hh
copy %2.tab.h ..\..\include\include_gen\%2.tab.h

copy location.hh ..\..\include\location.hh
copy position.hh ..\..\include\position.hh
copy stack.hh ..\..\include\stack.hh


del %2.tab.c
del %2.tab.h
del location.hh
del position.hh
del stack.hh