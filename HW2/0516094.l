%{
#include <string.h>
#include "y.tab.h"
#define LIST                { strncat(buf, yytext, sizeof(buf) - strlen(buf) - 1); }
#define token(t)            { LIST; if (Opt_Token) printf("<%s>\n", #t); }
#define tokenString(t, s)   { LIST; if (Opt_Token) printf("<%s:%s>\n", t, s); }
#define MAX_LINE_LENGTH 257
#define MAX_ID_LENGTH 257
int Opt_Source = 1;
int Opt_Token = 1;
int Opt_Statistic = 1;
int linenum = 1;
char stringBuf[MAX_LINE_LENGTH];
char buf[MAX_LINE_LENGTH];
int id_count = 0;
int freq[10000]={0}, i = 0;
char id_name[10000][MAX_ID_LENGTH];
int check = 0;

%}

letter  [A-Za-z_]
digit [0-9]
ID ({letter})({digit}|{letter})*
integer 0|[1-9][0-9]*
float {digit}+"."{digit}+
scientificNotation (0*{integer}|{float})[Ee][+-]?{digit}+
%x C_COMMENT
%x CPP_COMMENT
%x STRING_HANDLE
%x PRAGMA
%x PRAGMA_SOURCE
%x PRAGMA_STAT
%x PRAGMA_TOKEN
%x PRAGMA_ON_OFF
%%

"("	{
		tokenString("delim", "(");
		return '(';
	}
")"	{
		tokenString("delim", ")");
		return ')';
	}
","	{
		tokenString("delim", ",");
		return COMMA;	
}
";"	{
		tokenString("delim", ";");
		return SEMICOLON;
	}
"["	{
		tokenString("delim", "[");
		return '[';
	}
"]"	{
		tokenString("delim", "]");
		return ']';
	}
"{"	{
		tokenString("delim", "{");
		return '{';
	}
"}"	{
		tokenString("delim", "}");
		return '}';
	}
"+"	{
		token("+");
		return '+';
	}
"-"	{
		token("-");
		return '-';
	}
"*"	{
		token("*");
		return '*';
	}
"/"	{
		token("/");
		return '/';
	}
"%"	{
		token("%");
		return '%';
	}
"="	{
		token("=");
		return ASSIGN;
	}
"<"	{
		token("<");
		return '<';
	}
"<="	{
		token("<=");
		return SE;
	}
"!="	{
		token("!=");
		return NE;
	}
">="	{
		token(">=");
		return GE;
	}
">"	{
		token(">");
		return '>';
	}
"=="	{
		token("==");
		return EE;
	}
"&&"	{
		token("&&");
		return AND;
	}
"||"	{
		token("||");
		return OR;
	}
"!"	{
		token("!");
		return '!';
	}

"while"	{tokenString("KW", "while");
		return WHILE;
}
"do"	{tokenString("KW", "do");
		return DO;
}
"if"	{tokenString("KW", "if");
		return IF;
}
"else"	{tokenString("KW", "else");
		return ELSE;
}
"true"	{tokenString("KW", "true");
		return BOOLCONST;
}
"false" {tokenString("KW", "false");
		return BOOLCONST;
} 
"for"	{tokenString("KW", "for");
		return FOR;
}
"int"	{tokenString("KW", "int");
		return INT;
}
"print"	{tokenString("KW", "print");
		return PRINT;
}
"const"	{tokenString("KW", "const");
		return CONST;
}
"read"	{tokenString("KW", "read");
		return READ;
}
"boolean" {tokenString("KW", "boolean");
		return BOOLEAN;
}
"bool"	{tokenString("KW", "bool");
		return BOOL;
}
"void"	{tokenString("KW", "void");
		return VOID;
}
"float"	{tokenString("KW", "float");
		return FLOAT;
}
"double" {tokenString("KW", "double");
		return DOUBLE;
}
"string" {tokenString("KW", "string");
		return STRING;
}
"continue" {tokenString("KW", "continue");
		return CONTINUE;
}
"break"	{tokenString("KW", "break");
		return BREAK;
}
"return" {tokenString("KW", "return");
		return RETURN;
}

[ \t]+ {LIST;}
{ID}	{
	tokenString("id", yytext);
	return ID;
	//printf("%s", yytext);
	if(Opt_Statistic){
		for(i=1; i<=id_count; i++){
			if(strcmp(id_name[i], yytext) == 0){
				freq[i]++;
				check = 1;
				break;
			}	    
		}
		if(check == 0){
			id_count++;
			freq[id_count]++;
			strcpy(id_name[id_count], yytext);
		}
		check = 0;
	}
}

{scientificNotation} {tokenString("scientific", yytext);return FLOATCONST;}
{float} {tokenString("float", yytext);return FLOATCONST;}
{integer} {tokenString("integer", yytext);return INTEGERCONST;}
[\"]			{
	LIST;
	BEGIN(STRING_HANDLE);
}
<STRING_HANDLE>[\"]	{
	//LIST;
	tokenString("string", stringBuf);
	memset(stringBuf, 0, sizeof stringBuf);
	BEGIN(INITIAL);
	return STRINGCONST;
}
<STRING_HANDLE>[\\][\"]  {
	LIST;
	strncat(stringBuf, "\"", sizeof(stringBuf) - strlen(stringBuf) - 1);
	
}
<STRING_HANDLE>[\\]{2}  {
	LIST;
	strncat(stringBuf, "\\", sizeof(stringBuf) - strlen(stringBuf) - 1);
	
}

<STRING_HANDLE>\n { // string can't contain newline
    fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
    exit(1);
}
<STRING_HANDLE>.	{
	LIST;
	strncat(stringBuf, yytext, sizeof(stringBuf) - strlen(stringBuf) - 1);
}


"/*"		{
		LIST; 
		BEGIN(C_COMMENT); 
}
<C_COMMENT>"*/" {
		LIST; 
		BEGIN(INITIAL); 
}
<C_COMMENT>\n  	{ 
	    	LIST;
	    	if (Opt_Source)
			printf("%d:%s\n", linenum, buf);
	    	++linenum;
	    	buf[0] = '\0';
}
<C_COMMENT>.    {LIST;}

"//"		{
		LIST;
		BEGIN(CPP_COMMENT);
}
<CPP_COMMENT>\n  { 
	    	LIST;
	    	if (Opt_Source)
			printf("%d:%s\n", linenum, buf);
	    	++linenum;
	    	buf[0] = '\0';
		BEGIN(INITIAL);
}
<CPP_COMMENT>.    {LIST;}

^"#pragma"[\t ]+	{
		LIST;
		BEGIN(PRAGMA);
}

<PRAGMA>"source"[\t ]+ {
		LIST;
		BEGIN(PRAGMA_SOURCE);
}
<PRAGMA>"token"[\t ]+ {
		LIST;
		BEGIN(PRAGMA_TOKEN);
}
<PRAGMA>"statistic"[\t ]+ {
		LIST;
		BEGIN(PRAGMA_STAT);
}
<PRAGMA>\n {
    fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
    exit(1);
}
<PRAGMA>. {
    fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
    exit(1);
}

<PRAGMA_SOURCE>"on"[\t ]* {
	LIST;
	Opt_Source = 1;
	BEGIN(PRAGMA_ON_OFF);
}
<PRAGMA_SOURCE>"off"[\t ]* {
	LIST;
	Opt_Source = 0;
	BEGIN(PRAGMA_ON_OFF);
}
<PRAGMA_SOURCE>\n  { 
    fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
    exit(1);
}
<PRAGMA_SOURCE>. { // Unrecognized character
    fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
    exit(1);
}


<PRAGMA_TOKEN>"on"[\t ]* {
	LIST;
	Opt_Token = 1;
	BEGIN(PRAGMA_ON_OFF);
}
<PRAGMA_TOKEN>"off"[\t ]* {
	LIST;
	Opt_Token = 0;
	BEGIN(PRAGMA_ON_OFF);
}
<PRAGMA_TOKEN>\n  { 
	fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
	exit(1);
}
<PRAGMA_TOKEN>. { // Unrecognized character
	fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
	exit(1);
}


<PRAGMA_STAT>"on"[\t ]* {
	LIST;
	Opt_Statistic = 1;
	BEGIN(PRAGMA_ON_OFF);
}
<PRAGMA_STAT>"off"[\t ]* {
	LIST;
	Opt_Statistic = 0;
	BEGIN(PRAGMA_ON_OFF);
}
<PRAGMA_STAT>\n  { 
	fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
	exit(1);
}
<PRAGMA_STAT>. { // Unrecognized character
	fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
	exit(1);
}

<PRAGMA_ON_OFF>"/*"  {
	LIST; 
	BEGIN(C_COMMENT); 
}
<PRAGMA_ON_OFF>"//" {
	LIST;
	BEGIN(CPP_COMMENT);
}
<PRAGMA_ON_OFF>\n  { 
	LIST;
	if (Opt_Source)
	printf("%d:%s\n", linenum, buf);
	++linenum;
	buf[0] = '\0';
	BEGIN(INITIAL);
}
<PRAGMA_ON_OFF>.  { 
	fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
	exit(1);
}
\n {
    LIST;
    if (Opt_Source)
        printf("%d:%s\n", linenum, buf);
    ++linenum;
    buf[0] = '\0';
}

. { // Unrecognized character
    fprintf(stderr, "Error at line %d: %s\n", linenum, yytext);
    exit(1);
}


%%
/*
int main( int argc, char **argv )
{
    if (argc >= 2)
    {
        yyin = fopen( argv[1], "r" );
        if ( NULL == yyin ) {
            fprintf( stderr, "Cannot open: %s\n", argv[1] );
            exit(-1);
        }
    }
    else
    {
        yyin = stdin;
    }

    yylex();
    strncat(buf, "\n", sizeof(buf) - strlen(buf) - 1); 
    if (Opt_Source)
        printf("%d:%s\n", linenum, buf);
    ++linenum;
    if (Opt_Statistic)
    {
        // Print frequency table
	printf("frequencies of identifiers:\n");
	for(i=1;i<=id_count;i++){
		printf("%s	%d\n", id_name[i], freq[i]);	
	}
    }

    exit(0);
}

*/