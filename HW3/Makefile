TARGET = parser
OBJECT = lex.yy.c y.tab.c y.tab.o lex.yy.o y.output parser y.tab.h main.o
CC = gcc -g
CPP = g++ -g
LEX = flex
YACC = yacc -v
YACCFLAG = -d
LIBS = -lfl

all: $(TARGET)

parser: y.tab.o lex.yy.o main.o
	$(CPP) -o $(TARGET) y.tab.o lex.yy.o main.o $(LIBS)

y.tab.o: y.tab.c
	$(CPP) -c y.tab.c

y.tab.c: parser.y
	$(YACC) $(YACCFLAG) parser.y

lex.yy.o: lex.yy.c
	$(CPP) -c lex.yy.c

lex.yy.c: lex.l
	$(LEX) lex.l

main.o: main.c
	$(CPP) -c main.c

clean:
	rm -f $(TARGET) $(OBJECT)

