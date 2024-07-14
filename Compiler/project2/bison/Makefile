GCC = @g++
LEX = @flex
YACC = @bison

main: main.cpp yacc.o
			$(GCC) main.cpp yacc.o -o main

yacc.o: yacc.c
			$(GCC) -c yacc.c -o yacc.o

yacc.c: bison.y lexer.c
			$(YACC) -o yacc.c -d bison.y

lexer.c: lexer.lex
			$(LEX) -o lexer.c lexer.lex

clean:
	                        @-rm -f *.o *~ lexer.c yacc.c yacc.h main
.PHONY: clean
