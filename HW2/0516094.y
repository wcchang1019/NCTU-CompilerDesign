%{
#include <stdio.h>
#include <stdlib.h>

extern int linenum;             /* declared in lex.l */
extern FILE *yyin;              /* declared by lex */
extern char *yytext;            /* declared by lex */
extern char buf[256];           /* declared in lex.l */
%}
%nonassoc INT BOOL FLOAT DOUBLE STRING CONST VOID
%nonassoc END
%nonassoc ONLYIF
%nonassoc ELSE

%nonassoc '(' ')'

%left OR
%left AND
%left '!'
%nonassoc SE NE GE EE '<' '>'

%left '+' '-'
%left '*' '/' '%'

%token SEMICOLON    /* ; */
%token ID           /* identifier */
%token INT          /* keyword */ 
%token WHILE DO IF FOR PRINT CONST READ BOOLEAN BOOL VOID FLOAT DOUBLE STRING CONTINUE BREAK RETURN INTEGERCONST FLOATCONST STRINGCONST BOOLCONST COMMA ASSIGN SE NE GE EE AND OR

%%

program : declaration_list funct_def decl_and_def_list
	;

decl_and_def_list : decl_and_def_list const_decl 
		  | decl_and_def_list var_decl 
		  | decl_and_def_list funct_decl
		  | decl_and_def_list funct_def
		  |
		  ;

declaration_list : declaration_list const_decl
		 | declaration_list var_decl
		 | declaration_list funct_decl
		 |
		 ;

var_decl : type var_list SEMICOLON
         ;

type : INT
     | FLOAT
     | DOUBLE
     | STRING
     | BOOLEAN
     | BOOL
     ; 

var_list : var_item
	 | var_list COMMA var_item 
	 ;

var_item : identifier
         | ID ASSIGN expr
	 | ID array ASSIGN '{' zero_or_more_expressions '}'
	 ;


identifier : ID
	   | ID array 	
	   ;	

array : single_array
      | single_array array
      ;

single_array : '[' INTEGERCONST ']'
             ;

const_decl : CONST type const_list SEMICOLON
	   ;

const_list : const_item
	   | const_item COMMA const_list 
	   ;

const_item : ID ASSIGN all_constant 
	   | ID ASSIGN '-' all_constant %prec '*'
	   ;

all_constant : INTEGERCONST
             | FLOATCONST
	     | STRINGCONST
	     | BOOLCONST
	     ;

funct_decl : type ID '(' args ')' SEMICOLON
	   | VOID ID '(' args ')' SEMICOLON
	   ;

funct_def : type ID '(' args ')' compound_statement
	  | VOID ID '(' args ')' compound_statement
	  ;


args : arg_list
     |
     ;

arg_list : single_arg COMMA arg_list 
         | single_arg
         ;

single_arg : type identifier
           | CONST type identifier
           ;



compound_statement : '{'local_decl_statement '}'
		  ;

local_decl_statement : local_decl_statement var_decl 
		     | local_decl_statement const_decl
		     | local_decl_statement statement
		     |
		     ;

statement : compound_statement
	  | simple_statement
	  | if_statement
     	  | while_statement
     	  | for_statement
     	  | jump_statement
     	  | function_invocation
	  ;

simple_statement : variable_reference ASSIGN expr SEMICOLON
		 | PRINT expr SEMICOLON
		 | READ variable_reference SEMICOLON
		 ;

if_statement : IF '(' expr ')' compound_statement %prec ONLYIF
	     | IF '(' expr ')' compound_statement ELSE compound_statement
	     ;

while_statement : WHILE '(' expr ')' compound_statement
    		| DO compound_statement WHILE '(' expr ')' SEMICOLON
		;

for_statement : FOR '(' for_expr SEMICOLON for_expr SEMICOLON for_expr ')' compound_statement
	      ;

for_expr : 
	 | expr
	 | more_for_expr
	 ;

more_for_expr : variable_reference ASSIGN expr
	      | more_for_expr COMMA variable_reference ASSIGN expr
	      ;

jump_statement : RETURN expr SEMICOLON
	       | BREAK SEMICOLON
	       | CONTINUE SEMICOLON
	       ;

variable_reference : ID
		   | ID array_reference
		   ;

array_reference : array_reference_item array_reference 
		| array_reference_item
		;

array_reference_item : '[' expr ']'
		     ;

expr : expr '+' expr
     | expr '-' expr
     | expr '*' expr
     | expr '/' expr
     | expr '%' expr
     | '-' expr %prec '*'
     | '(' expr ')'
     | expr '<' expr
     | expr SE expr
     | expr '>' expr
     | expr GE expr
     | expr NE expr
     | expr EE expr
     | expr AND expr
     | expr OR expr
     | '!' expr
     | all_constant
     | function_invocation
     | variable_reference
     ;

function_invocation : ID '('zero_or_more_expressions ')' SEMICOLON
		    ;

zero_or_more_expressions : expr_list
			 |
			 ;

expr_list : expr_list COMMA expr
	  | expr
	  ;
%%

int yyerror( char *msg )
{
  fprintf( stderr, "\n|--------------------------------------------------------------------------\n" );
	fprintf( stderr, "| Error found in Line #%d: %s\n", linenum, buf );
	fprintf( stderr, "|\n" );
	fprintf( stderr, "| Unmatched token: %s\n", yytext );
  fprintf( stderr, "|--------------------------------------------------------------------------\n" );
  exit(-1);
}

int  main( int argc, char **argv )
{
	if( argc != 2 ) {
		fprintf(  stdout,  "Usage:  ./parser  [filename]\n"  );
		exit(0);
	}

	FILE *fp = fopen( argv[1], "r" );
	
	if( fp == NULL )  {
		fprintf( stdout, "Open  file  error\n" );
		exit(-1);
	}
	
	yyin = fp;
	yyparse();

	fprintf( stdout, "\n" );
	fprintf( stdout, "|--------------------------------|\n" );
	fprintf( stdout, "|  There is no syntactic error!  |\n" );
	fprintf( stdout, "|--------------------------------|\n" );
	exit(0);
}
