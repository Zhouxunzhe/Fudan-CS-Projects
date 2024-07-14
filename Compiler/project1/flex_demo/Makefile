GCC = @g++
LEX = @flex

lexer: lexer.c main.cpp
			$(GCC) lexer.c main.cpp -o lexer

lexer.c: lexer.lex
			$(LEX) -o lexer.c lexer.lex

clean:
				@-rm -f lexer.c lexer
.PHONY: clean