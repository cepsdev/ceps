

includes := -Iinclude -Iinclude/include_gen
cflags :=  -O0 -g3 -Wall -fmessage-length=0 -std=c++11
OBJDIR := bin
objfiles := ceps.tab.o symtab.o cepslexer.o cepsparserdriver.o cepsruntime.o ceps_ast.o ceps_interpreter.o ceps_serialize.o
objfiles := $(patsubst %,$(OBJDIR)/%,$(objfiles))
src := src
inc := include
BISONGENDIR := src-gen
CEPSLIB := libcepscore.a

all: bin/$(CEPSLIB)

tests: test/ast_gen_1 test/si_units_1 test/test_1

bin/$(CEPSLIB): $(objfiles)
	rm -f bin/$(CEPSLIB);\
	ar rcs $(OBJDIR)/$(CEPSLIB) $(objfiles)

$(OBJDIR)/ceps.tab.o: $(BISONGENDIR)/ceps.tab.cpp
	g++ -c $(cflags) $(BISONGENDIR)/ceps.tab.cpp $(includes) -o bin/ceps.tab.o

$(OBJDIR)/symtab.o: $(src)/symtab.cpp
	g++ -c $(cflags) $(includes) $(src)/symtab.cpp -o $(OBJDIR)/symtab.o  

$(OBJDIR)/cepslexer.o: $(src)/cepslexer.cpp
	g++ -c $(cflags) $(includes) $(src)/cepslexer.cpp -o $(OBJDIR)/cepslexer.o

$(OBJDIR)/cepsparserdriver.o: $(src)/cepsparserdriver.cpp
	g++ -c $(cflags) $(includes) $(src)/cepsparserdriver.cpp -o $(OBJDIR)/cepsparserdriver.o

$(OBJDIR)/cepsruntime.o: $(src)/cepsruntime.cpp $(inc)/cepsruntime.hh $(inc)/ceps_interpreter.hh
	g++ -c $(cflags) $(includes) $(src)/cepsruntime.cpp -o $(OBJDIR)/cepsruntime.o

$(OBJDIR)/ceps_ast.o: $(src)/ceps_ast.cpp include/ceps_ast.hh
	g++ -c $(cflags) $(includes) $(src)/ceps_ast.cpp -o $(OBJDIR)/ceps_ast.o

$(OBJDIR)/ceps_interpreter.o: $(src)/ceps_interpreter.cpp include/ceps_interpreter.hh
	g++ -c $(cflags) $(includes) $(src)/ceps_interpreter.cpp -o $(OBJDIR)/ceps_interpreter.o

$(OBJDIR)/ceps_serialize.o: $(src)/ceps_serialize.cpp include/ceps_serialize.hh
	g++ -c $(cflags) $(includes) $(src)/ceps_serialize.cpp -o $(OBJDIR)/ceps_serialize.o


src-gen/ceps.tab.cpp: src/grammar/ceps.y
	cd $(BISONGENDIR);\
	~/local/bin/bison ../src/grammar/ceps.y;\
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
	g++ test/ast_gen_1.cpp $(cflags) $(includes) -L$(OBJDIR) -lcepscore -o test/ast_gen_1;\
	chmod +x test/ast_gen_1

test/si_units_1: test/si_units_1.cpp $(OBJDIR)/libcepscore.a
	g++ test/si_units_1.cpp $(cflags) $(includes) -L$(OBJDIR) -lcepscore -o test/si_units_1;\
	chmod +x test/si_units_1

test/test_1: test/test_1.cpp $(OBJDIR)/libcepscore.a
	g++ test/test_1.cpp $(cflags) $(includes) -L$(OBJDIR) -lcepscore -o test/test_1;\
	chmod +x test/test_1

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

