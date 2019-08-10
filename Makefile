PROG = pixelcode
CC = clang++
CPPFLAGS=$(shell llvm-config-6.0 --cxxflags)
LDFLAGS=$(shell llvm-config-6.0 --ldflags --libs)

$(PROG): lex.yy.o parser.o
	$(CC) $(LDFLAGS) -o $@ $^
lex.yy.o: lex.yy.c parser.tab.hpp
	$(CC) $(CPPFLAGS) -Wno-deprecated -c -o $@ $<
lex.yy.c: source/lexer.lex
	flex $<
parser.o: parser.tab.cpp parser.tab.hpp
	$(CC) $(CPPFLAGS) -c -o $@ $<
parser.tab.cpp parser.tab.hpp: source/parser.ypp
	bison -v -d $<

.PHONY: clean

clean:
	rm -f *~ *tab* lex.yy.c $(PROG) *.o *.output
