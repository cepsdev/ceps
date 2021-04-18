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

all: bin/$(CEPSLIB) tools/cepsvalidate/cepsvalidate

tests: test/ast_gen_1 test/si_units_1 test/test_1

bin/$(CEPSLIB): $(objfiles)
	mkdir bin -p
	rm -f bin/$(CEPSLIB);\
	$(AR) rcs $(OBJDIR)/$(CEPSLIB) $(objfiles)
	mkdir -p bin/x86
	rm -f $(cepslib86)
	rm -f $(objfilesx86)
	cp $(objfiles) bin/$(CEPSLIB) bin/x86
	
$(OBJDIR)/ceps.tab.o: $(BISONGENDIR)/ceps.tab.cpp
	$(CXX) -c $(cflags) $(BISONGENDIR)/ceps.tab.cpp $(includes) -o $(OBJDIR)/ceps.tab.o

$(OBJDIR)/symtab.o: $(src)/symtab.cpp
	$(CXX) -c $(cflags) $(includes) $(src)/symtab.cpp -o $(OBJDIR)/symtab.o  

$(OBJDIR)/cepslexer.o: $(src)/cepslexer.cpp include/ceps_ast.hh
	$(CXX) -c $(cflags) $(includes) $(src)/cepslexer.cpp -o $(OBJDIR)/cepslexer.o

$(OBJDIR)/cepsparserdriver.o: $(src)/cepsparserdriver.cpp include/ceps_ast.hh
	$(CXX) -c $(cflags) $(includes) $(src)/cepsparserdriver.cpp -o $(OBJDIR)/cepsparserdriver.o

$(OBJDIR)/cepsruntime.o: $(src)/cepsruntime.cpp $(inc)/cepsruntime.hh $(inc)/ceps_interpreter.hh include/ceps_ast.hh
	$(CXX) -c $(cflags) $(includes) $(src)/cepsruntime.cpp -o $(OBJDIR)/cepsruntime.o

$(OBJDIR)/ceps_ast.o: $(src)/ceps_ast.cpp include/ceps_ast.hh
	$(CXX) -c $(cflags) $(includes) $(src)/ceps_ast.cpp -o $(OBJDIR)/ceps_ast.o

$(OBJDIR)/ceps_interpreter.o: $(src)/ceps_interpreter.cpp include/ceps_interpreter.hh include/ceps_ast.hh
	$(CXX) -c $(cflags) $(includes) $(src)/ceps_interpreter.cpp -o $(OBJDIR)/ceps_interpreter.o

$(OBJDIR)/ceps_serialize.o: $(src)/ceps_serialize.cpp include/ceps_serialize.hh include/ceps_ast.hh
	$(CXX) -c $(cflags) $(includes) $(src)/ceps_serialize.cpp -o $(OBJDIR)/ceps_serialize.o

$(OBJDIR)/ceps_interpreter_loop.o: $(src)/ceps_interpreter_loop.cpp include/ceps_interpreter_loop.hh include/ceps_ast.hh
	$(CXX) -c $(cflags) $(includes) $(src)/ceps_interpreter_loop.cpp -o $(OBJDIR)/ceps_interpreter_loop.o

$(OBJDIR)/ceps_interpreter_nodeset.o: $(src)/ceps_interpreter_nodeset.cpp include/ceps_interpreter_nodeset.hh include/ceps_ast.hh
	$(CXX) -c $(cflags) $(includes) $(src)/ceps_interpreter_nodeset.cpp -o $(OBJDIR)/ceps_interpreter_nodeset.o
$(OBJDIR)/ceps_interpreter_functions.o: $(src)/ceps_interpreter_functions.cpp include/ceps_interpreter.hh include/ceps_ast.hh
	$(CXX) -c $(cflags) $(includes) $(src)/ceps_interpreter_functions.cpp -o $(OBJDIR)/ceps_interpreter_functions.o
$(OBJDIR)/ceps_interpreter_macros.o: $(src)/ceps_interpreter_macros.cpp include/ceps_interpreter.hh include/ceps_ast.hh
	$(CXX) -c $(cflags) $(includes) $(src)/ceps_interpreter_macros.cpp -o $(OBJDIR)/ceps_interpreter_macros.o
		
$(OBJDIR)/pugixml.o: $(pugixml)/pugixml.cpp $(pugixml)/pugixml.hpp
	$(CXX) -c $(cflags) $(includes)  $(pugixml)/pugixml.cpp -o $(OBJDIR)/pugixml.o
	

src-gen/ceps.tab.cpp: src/grammar/ceps.y
	mkdir $(BISONGENDIR) -p
	cd $(BISONGENDIR);\
	bison ../src/grammar/ceps.y;\
	cd ..;\
	mv $(BISONGENDIR)/location.hh include/include_gen;\
	mv $(BISONGENDIR)/position.hh include/include_gen;\
	mv $(BISONGENDIR)/stack.hh include/include_gen;\
	mv $(BISONGENDIR)/ceps.tab.h include/include_gen;\
	cp include/include_gen/ceps.tab.h include/include_gen/ceps.tab.hh;\
	mv $(BISONGENDIR)/ceps.tab.c $(BISONGENDIR)/ceps.tab.cpp;\
	cat $(BISONGENDIR)/ceps.output >> build.log;\
	rm -f $(BISONGENDIR)/ceps.output

test/ast_gen_1: test/ast_gen_1.cpp $(OBJDIR)/libcepscore.a
	$(CXX) test/ast_gen_1.cpp $(cflags) $(includes) -L$(OBJDIR) -lcepscore -o test/ast_gen_1;\
	chmod +x test/ast_gen_1

test/si_units_1: test/si_units_1.cpp $(OBJDIR)/libcepscore.a
	$(CXX) test/si_units_1.cpp $(cflags) $(includes) -L$(OBJDIR) -lcepscore -o test/si_units_1;\
	chmod +x test/si_units_1

test/test_1: test/test_1.cpp $(OBJDIR)/libcepscore.a
	$(CXX) test/test_1.cpp $(cflags) $(includes) -L$(OBJDIR) -lcepscore -o test/test_1;\
	chmod +x test/test_1
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

