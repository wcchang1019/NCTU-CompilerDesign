#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"
#include "symtab.h"

extern int yyparse();
extern FILE* yyin;

extern struct SymTable *symbolTable;
extern struct PType *funcReturn;
char* fileName;

extern __BOOLEAN semError; 
FILE* fpOut;
FILE* fpFor;

int  main( int argc, char **argv )
{
	if( argc == 1 )
	{
		yyin = stdin;
	}
	else if( argc == 2 )
	{
		FILE *fp = fopen( argv[1], "r" );
		if( fp == NULL ) {
				fprintf( stderr, "Open file error\n" );
				exit(-1);
		}
		yyin = fp;
	}
	else
	{
	  	fprintf( stderr, "Usage: ./parser [filename]\n" );
   		exit(0);
 	} 

	symbolTable = (struct SymTable *)malloc(sizeof(struct SymTable));
	initSymTab( symbolTable );
	fileName = argv[1];
	char* tmp = strrchr(fileName, '/');
	if(tmp){
		fileName = tmp+1;
	}else{
		
	}
	
	fpOut = fopen("output.j", "w");
	// initial function return recoder

	yyparse();	/* primary procedure of parser */

	if(semError == __TRUE){	
		fprintf( stdout, "\n|--------------------------------|\n" );
		fprintf( stdout, "|  There is no syntactic error!  |\n" );
		fprintf( stdout, "|--------------------------------|\n" );
	}
	else{
		fprintf( stdout, "\n|-------------------------------------------|\n" );
		fprintf( stdout, "| There is no syntactic and semantic error! |\n" );
		fprintf( stdout, "|-------------------------------------------|\n" );
	}
	fclose(fpOut);
	//fclose(fpFor);
	exit(0);
}

