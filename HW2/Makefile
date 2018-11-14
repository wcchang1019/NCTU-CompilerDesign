OBJECT = lex.yy.c y.tab.h y.tab.c y.output parser

all: lex.yy.c y.tab.c
	gcc -g lex.yy.c y.tab.c -o parser -ly -ll 

y.tab.c: 0516094.y
	yacc -d -v 0516094.y

lex.yy.c: 0516094.l
	flex 0516094.l

clean:
	rm -f $(OBJECT)
