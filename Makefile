PROG = pixelcode
CC = clang++
CPPFLAGS=$(shell llvm-config-6.0 --cxxflags)
LDFLAGS=$(shell llvm-config-6.0 --ldflags --libs)

#imageParser: imageParser.o image.o
	#$(CC) -o $@ imageParser.o image.o



$(PROG): lex.yy.o parser.o imageParser.o image.o ast.o
	$(CC) $(LDFLAGS) -o $@ $^
ast.o: source/ast.cpp header/ast.hpp
	$(CC) $(CPPFLAGS) -c -o $@ $<
lex.yy.o: lex.yy.c parser.tab.hpp
	$(CC) $(CPPFLAGS) -Wno-deprecated -c -o $@ $<
lex.yy.c: source/lexer.lex
	flex $<
parser.o: parser.tab.cpp parser.tab.hpp
	$(CC) $(CPPFLAGS) -c -o $@ $<
parser.tab.cpp parser.tab.hpp: source/parser.ypp
	bison -v -d $<

imageParser.o: source/imageParser.cpp
	$(CC)  -c -o $@ $<

image.o: source/image.c
	$(CC) -c -o $@ $<

.PHONY: clean clear_tests

clean:
	rm -f *~ *tab* lex.yy.c $(PROG) *.o *.output tmpReprFile *.s *.ll *.out

clear_tests:
	rm -f *.s *.ll *.out
