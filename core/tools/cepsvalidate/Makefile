#
# cepsvalidate - Makefile
#

includes :=  -I"include" -I"../../include" -I"../../include/include-gen"
cflags := -O0 -g3 -Wall -fmessage-length=0 -std=c++17
OBJDIR := bin
objfiles := $(patsubst %,$(OBJDIR)/%,$(objfiles))
CEPSLIB := ../../bin/libcepscore.a
tutorial_dir := tutorial
cepslibs := ../../bin

all: cepsvalidate 

cepsvalidate: cepsvalidate.o ../../bin/cepsparserdriver.o ../../bin/ceps_interpreter.o ../../bin/symtab.o ../../bin/cepslexer.o ../../bin/ceps.tab.o ../../bin/ceps_ast.o  \
 ../../bin/cepsruntime.o ../../bin/ceps_interpreter_loop.o ../../bin/ceps_interpreter_nodeset.o ../../bin/ceps_interpreter_functions.o ../../bin/ceps_interpreter_macros.o ../../bin/ceps_interpreter_eval_id.o \
  ../../bin/ceps_interpreter_generic.o ../../bin/ceps_interpreter_binop.o ../../bin/pugixml.o cepsvalidate.o
	g++   $(cflags) $(includes) -L"$(cepslibs)" ../../bin/cepsparserdriver.o ../../bin/ceps_interpreter.o ../../bin/symtab.o ../../bin/cepslexer.o ../../bin/ceps.tab.o ../../bin/ceps_ast.o \
	  ../../bin/cepsruntime.o ../../bin/ceps_interpreter_loop.o ../../bin/ceps_interpreter_nodeset.o ../../bin/ceps_interpreter_functions.o ../../bin/ceps_interpreter_macros.o \
	  ../../bin/ceps_interpreter_ifelse.o ../../bin/ceps_interpreter_eval_id.o ../../bin/ceps_interpreter_generic.o ../../bin/ceps_interpreter_binop.o  ../../bin/pugixml.o cepsvalidate.o -o cepsvalidate 

cepsvalidate.o: src/cepsvalidate.cpp
	g++ -c $(cflags) src/cepsvalidate.cpp $(includes) -o cepsvalidate.o

clean:
	rm -f *o
	rm -f cepsvalidate

