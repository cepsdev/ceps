#-------------------------------------------------
#
# Project created by QtCreator 2015-11-24T15:21:39
#
#-------------------------------------------------

CONFIG   += c++11
CEPS_DIR = $$_PRO_FILE_PWD_/../..
INCLUDEPATH += $$CEPS_DIR/include
INCLUDEPATH += $$CEPS_DIR/include/include_gen
QT       -= core gui

TARGET = cepscore
TEMPLATE = lib
CONFIG += staticlib

SOURCES += cepscore.cpp \
 $$CEPS_DIR/src-gen/ceps.tab.cpp \
 $$CEPS_DIR/src/ceps_ast.cpp \
 $$CEPS_DIR/src/ceps_interpreter.cpp \
 $$CEPS_DIR/src/ceps_interpreter_loop.cpp \
 $$CEPS_DIR/src/ceps_interpreter_nodeset.cpp \
 $$CEPS_DIR/src/ceps_serialize.cpp \
 $$CEPS_DIR/src/cepslexer.cpp \
 $$CEPS_DIR/src/cepsparserdriver.cpp \
 $$CEPS_DIR/src/cepsruntime.cpp \
 $$CEPS_DIR/src/symtab.cpp

HEADERS += cepscore.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}
