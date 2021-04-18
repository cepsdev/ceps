cflags :=  -O0 -g3 -Wall -fmessage-length=0 -std=c++11 -fPIC
TARGET :=
OBJDIR := bin$(TARGET)
objfiles_orig := ceps.tab.o symtab.o cepslexer.o cepsparserdriver.o cepsruntime.o ceps_ast.o ceps_interpreter.o \
 ceps_serialize.o ceps_interpreter_loop.o ceps_interpreter_nodeset.o pugixml.o ceps_interpreter_functions.o ceps_interpreter_macros.o
objfiles := $(patsubst %,$(OBJDIR)/%,$(objfiles_orig))
objfilesx86 := $(patsubst %,$(OBJDIR)/x86/%,$(objfiles_orig))

src := src
pugixml := ../../pugixml/src
inc := include
BISONGENDIR := src-gen
CEPSLIB := libcepscore.a
includes := -Iinclude -Iinclude/include_gen -I$(pugixml)
cepslib86 := bin/x86/$(CEPSLIB)
SRCGEN_EXISTS=$(shell [ -d src-gen ] && echo 1 || echo 0 )
ifeq ($(SRCGEN_EXISTS), 0)
 xy:=$(shell mkdir src-gen && echo 1 || echo 0)
endif

INCGEN_EXISTS=$(shell [ -d include/include_gen ] && echo 1 || echo 0 )
ifeq ($(INCGEN_EXISTS), 0)
 xyz:=$(shell mkdir include/include_gen && echo 1 || echo 0)
endif


BINGEN_EXISTS=$(shell [ -d bin ] && echo 1 || echo 0 )
ifeq ($(BINGEN_EXISTS), 0)
 xyz1:=$(shell mkdir bin && echo 1 || echo 0)
endif

all: bin/$(CEPSLIB) tools/cepsvalidate/cepsvalidate

bin/$(CEPSLIB): $(objfiles)
	rm -f bin/$(CEPSLIB);\
	$(AR) rcs $(OBJDIR)/$(CEPSLIB) $(objfiles)
	mkdir -p bin/x86
	rm -f $(cepslib86)
	rm -f $(objfilesx86)
	cp $(objfiles) bin/$(CEPSLIB) bin/x86
$(OBJDIR)/ceps.tab.o: $(BISONGENDIR)/ceps.tab.cpp include/include_gen/ceps.tab.hh
	$(CXX) -c $(cflags) $(BISONGENDIR)/ceps.tab.cpp $(includes) -o $(OBJDIR)/ceps.tab.o

$(OBJDIR)/symtab.o: $(src)/symtab.cpp include/include_gen/ceps.tab.hh
	$(CXX) -c $(cflags) $(includes) $(src)/symtab.cpp -o $(OBJDIR)/symtab.o  

$(OBJDIR)/cepslexer.o: $(src)/cepslexer.cpp include/ceps_ast.hh include/include_gen/ceps.tab.hh
	$(CXX) -c $(cflags) $(includes) $(src)/cepslexer.cpp -o $(OBJDIR)/cepslexer.o

$(OBJDIR)/cepsparserdriver.o: $(src)/cepsparserdriver.cpp include/ceps_ast.hh include/include_gen/ceps.tab.hh
	$(CXX) -c $(cflags) $(includes) $(src)/cepsparserdriver.cpp -o $(OBJDIR)/cepsparserdriver.o

$(OBJDIR)/cepsruntime.o: $(src)/cepsruntime.cpp $(inc)/cepsruntime.hh $(inc)/ceps_interpreter.hh include/ceps_ast.hh include/include_gen/ceps.tab.hh
	$(CXX) -c $(cflags) $(includes) $(src)/cepsruntime.cpp -o $(OBJDIR)/cepsruntime.o

$(OBJDIR)/ceps_ast.o: $(src)/ceps_ast.cpp include/ceps_ast.hh include/include_gen/ceps.tab.hh
	$(CXX) -c $(cflags) $(includes) $(src)/ceps_ast.cpp -o $(OBJDIR)/ceps_ast.o

$(OBJDIR)/ceps_interpreter.o: $(src)/ceps_interpreter.cpp include/ceps_interpreter.hh include/ceps_ast.hh include/include_gen/ceps.tab.hh
	$(CXX) -c $(cflags) $(includes) $(src)/ceps_interpreter.cpp -o $(OBJDIR)/ceps_interpreter.o

$(OBJDIR)/ceps_serialize.o: $(src)/ceps_serialize.cpp include/ceps_serialize.hh include/ceps_ast.hh include/include_gen/ceps.tab.hh
	$(CXX) -c $(cflags) $(includes) $(src)/ceps_serialize.cpp -o $(OBJDIR)/ceps_serialize.o

$(OBJDIR)/ceps_interpreter_loop.o: $(src)/ceps_interpreter_loop.cpp include/ceps_interpreter_loop.hh include/ceps_ast.hh include/include_gen/ceps.tab.hh
	$(CXX) -c $(cflags) $(includes) $(src)/ceps_interpreter_loop.cpp -o $(OBJDIR)/ceps_interpreter_loop.o

$(OBJDIR)/ceps_interpreter_nodeset.o: $(src)/ceps_interpreter_nodeset.cpp include/ceps_interpreter_nodeset.hh include/ceps_ast.hh include/include_gen/ceps.tab.hh
	$(CXX) -c $(cflags) $(includes) $(src)/ceps_interpreter_nodeset.cpp -o $(OBJDIR)/ceps_interpreter_nodeset.o
$(OBJDIR)/ceps_interpreter_functions.o: $(src)/ceps_interpreter_functions.cpp include/ceps_interpreter.hh include/ceps_ast.hh include/include_gen/ceps.tab.hh
	$(CXX) -c $(cflags) $(includes) $(src)/ceps_interpreter_functions.cpp -o $(OBJDIR)/ceps_interpreter_functions.o
$(OBJDIR)/ceps_interpreter_macros.o: $(src)/ceps_interpreter_macros.cpp include/ceps_interpreter.hh include/ceps_ast.hh include/include_gen/ceps.tab.hh
	$(CXX) -c $(cflags) $(includes) $(src)/ceps_interpreter_macros.cpp -o $(OBJDIR)/ceps_interpreter_macros.o
		
$(OBJDIR)/pugixml.o: $(pugixml)/pugixml.cpp $(pugixml)/pugixml.hpp
	$(CXX) -c $(cflags) $(includes)  $(pugixml)/pugixml.cpp -o $(OBJDIR)/pugixml.o

include/include_gen/ceps.tab.hh $(BISONGENDIR)/ceps.tab.cpp: src/grammar/ceps.y
	@echo "========= Running Bison =========";
	@cd $(BISONGENDIR);bison ../src/grammar/ceps.y;cd ..;
	@ls -al src-gen
	@mv $(BISONGENDIR)/location.hh include/include_gen
	@cp $(BISONGENDIR)/ceps.tab.h include/include_gen/ceps.tab.h
	@mv $(BISONGENDIR)/ceps.tab.h include/include_gen/ceps.tab.hh
	@mv $(BISONGENDIR)/ceps.tab.c $(BISONGENDIR)/ceps.tab.cpp
	@ls -al include/include_gen
	@ls -al src-gen

tools/cepsvalidate/cepsvalidate: bin/$(CEPSLIB)
	cd tools/cepsvalidate && $(MAKE)

clean_tests:
	rm -f test/ast_gen_1

clean:
	rm -f $(OBJDIR)/*o
	rm -f $(OBJDIR)/*a
	rm -f $(BISONGENDIR)/location.hh
	rm -f $(BISONGENDIR)/position.hh
	rm -f $(BISONGENDIR)/stack.hh
	rm -f $(BISONGENDIR)/ceps.output
	rm -f $(BISONGENDIR)/ceps.tab.cpp
	rm -f $(BISONGENDIR)/ceps.tab.h
	rm -f build.log
