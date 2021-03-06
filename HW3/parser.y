%{
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <string.h>
 

using namespace std;
extern "C"
{
	int yyerror(char *msg);
	extern int yylex(void);
}



struct row{
	string name;
	string kind;
	int level;
	string type;
	string attribute;
};

vector<vector<row> > symbolTable;
row tmp;
vector<row> tmpTable;
vector<row> argTable;

extern int linenum;
extern FILE *yyin;
extern char *yytext;
extern char buf[256];
extern int yylex();
extern int level;
extern int Opt_Symbol;
int yyerror( char *msg );
void printTable(vector<row> table);
bool functionCheck(row tmp, vector<row> table);
void insertRow(row tmp,int level,int control);
void elseConditionPrint();
bool errorDection(row tmp,int level);
%}



%union {
	char* text;
}
%type<text> literal_const scalar_type dim parameter_list
%token<text>  ID
%token<text>  INT_CONST
%token<text>  FLOAT_CONST
%token<text>  SCIENTIFIC
%token  STR_CONST

%token  LE_OP
%token  NE_OP
%token  GE_OP
%token  EQ_OP
%token  AND_OP
%token  OR_OP

%token  READ
%token  BOOLEAN
%token  WHILE
%token  DO
%token  IF
%token  ELSE
%token  TRUE
%token  FALSE
%token  FOR
%token<text>  INT
%token<text>  PRINT
%token<text>  BOOL
%token<text>  VOID
%token<text>  FLOAT
%token<text>  DOUBLE
%token<text>  STRING
%token  CONTINUE
%token  BREAK
%token  RETURN
%token<text>  CONST

%token  L_PAREN
%token  R_PAREN
%token  COMMA
%token  SEMICOLON
%token<text>  ML_BRACE
%token<text>  MR_BRACE
%token  L_BRACE
%token  R_BRACE
%token  ADD_OP
%token  SUB_OP
%token  MUL_OP
%token  DIV_OP
%token  MOD_OP
%token  ASSIGN_OP
%token  LT_OP
%token  GT_OP
%token  NOT_OP


/*  Program 
    Function 
    Array 
    Const 
    IF 
    ELSE 
    RETURN 
    FOR 
    WHILE
*/
%start program
%%

program : decl_list funct_def decl_and_def_list{
		for(int i=int(symbolTable.size())-1;i>=0;i--)
			if(Opt_Symbol) printTable(symbolTable[i]);
	}
        ;

decl_list : decl_list var_decl
          | decl_list const_decl
          | decl_list funct_decl
          |
          ;


decl_and_def_list : decl_and_def_list var_decl
                  | decl_and_def_list const_decl
                  | decl_and_def_list funct_decl
                  | decl_and_def_list funct_def
                  | 
                  ;

funct_def : scalar_type ID L_PAREN R_PAREN compound_statement{
		string str($2);
		tmp.name = str;
		string str2($1);
		tmp.type = str2;
		tmp.kind = "function";
		tmp.level = level;
		if(functionCheck(tmp, symbolTable[level])) insertRow(tmp, level, 1);
		tmp = (row){"", "", 0, "", ""};
	  }
          | scalar_type ID L_PAREN parameter_list R_PAREN compound_statement{
		for(int i=int(argTable.size())-1;i>=0;i--){
			insertRow(argTable[i], level+1, 0);
		}
		argTable.clear();
		string str($2);
		tmp.name = str;
		string str2($1);
		tmp.type = str2;
		tmp.kind = "function";
		tmp.level = level;
		string str3($4);
		tmp.attribute = str3;
		if(functionCheck(tmp, symbolTable[level])) insertRow(tmp, level, 1);
		tmp = (row){"", "", 0, "", ""};
	  }
          | VOID ID L_PAREN R_PAREN compound_statement{
		string str($2);
		tmp.name = str;
		tmp.type = "void";
		tmp.kind = "function";
		tmp.level = level;
		if(functionCheck(tmp, symbolTable[level])) insertRow(tmp, level, 1);
		tmp = (row){"", "", 0, "", ""};
	  }
          | VOID ID L_PAREN parameter_list R_PAREN compound_statement{
		for(int i=int(argTable.size())-1;i>=0;i--){
			insertRow(argTable[i], level+1, 0);
		}
		argTable.clear();
		string str($2);
		tmp.name = str;
		tmp.type = "void";
		tmp.kind = "function";
		tmp.level = level;
		string str2($4);
		tmp.attribute = str2;
		if(functionCheck(tmp, symbolTable[level])) insertRow(tmp, level, 1);
		tmp = (row){"", "", 0, "", ""};
	  }
          ;

funct_decl : scalar_type ID L_PAREN R_PAREN SEMICOLON{
		string str($2);
		tmp.name = str;
		string str2($1);
		tmp.type = str2;
		tmp.kind = "function";
		insertRow(tmp, level, 1);
		tmp = (row){"", "", 0, "", ""};
	   }
           | scalar_type ID L_PAREN parameter_list R_PAREN SEMICOLON{
		string str($2);
		tmp.name = str;
		string str2($1);
		tmp.type = str2;
		tmp.kind = "function";
		string str3($4);
		tmp.attribute = str3;
		insertRow(tmp, level, 1);
		tmp = (row){"", "", 0, "", ""};
		argTable.clear();
	   }
           | VOID ID L_PAREN R_PAREN SEMICOLON{
		string str($2);
		tmp.name = str;
		tmp.type = "void";
		tmp.kind = "function";
		insertRow(tmp, level, 1);
		tmp = (row){"", "", 0, "", ""};
	   }
           | VOID ID L_PAREN parameter_list R_PAREN SEMICOLON{
		string str($2);
		tmp.name = str;
		tmp.type = "void";
		tmp.kind = "function";
		string str2($4);
		tmp.attribute = str2;
		insertRow(tmp, level, 1);
		tmp = (row){"", "", 0, "", ""};
		argTable.clear();
	   }
           ;

parameter_list : parameter_list COMMA scalar_type ID {
			string str($3);
			str = ","+str;
			strcat($$, str.c_str());

			row arg;
			str.erase(str.begin()+0);
			arg.type = str;
			string str2($4);
			arg.name = str2;
			arg.level = level+1;
			arg.kind = "parameter";
			argTable.push_back(arg);
	       }
               | parameter_list COMMA scalar_type ID dim{
			string str($3);
			str = ","+str;
			strcat($$, str.c_str());
			string str2($5);
			strcat($$, str2.c_str());

			row arg;
			str.erase(str.begin()+0);
			arg.type = str;
			string str3($4);
			arg.name = str3;
			arg.level = level+1;
			arg.kind = "parameter";
			argTable.push_back(arg);
	       }
               | scalar_type ID dim{
			$$ = $1;
			strcat($$, $3);

			row arg;
			string str($2);
			arg.name = str;
			string str2($$);
			arg.type = str2;
			arg.level = level+1;
			arg.kind = "parameter";
			argTable.push_back(arg);
	       }
               | scalar_type ID {
			$$ = $1;

			row arg;
			string str($2);
			arg.name = str;
			string str2($1);
			arg.type = str2;
			arg.level = level+1;
			arg.kind = "parameter";
			argTable.push_back(arg);
	       }
               ;




var_decl : scalar_type identifier_list SEMICOLON{
		for(int i=0;i<tmpTable.size();i++){
			tmpTable[i].kind = "variable";
			tmpTable[i].level = level;
			string str($1);
			tmpTable[i].type = str + tmpTable[i].type;
			insertRow(tmpTable[i], level, 1);
		}
		tmpTable.clear();
		tmp = (row){"", "", 0, "", ""};
       	 }
         ;

identifier_list : identifier_list COMMA ID{
			string str($3);
			tmp.name= str;	
			tmpTable.push_back(tmp);
			tmp = (row){"", "", 0, "", ""};	
		}
                | identifier_list COMMA ID ASSIGN_OP logical_expression{
			string str($3);
			tmp.name= str;	
			tmpTable.push_back(tmp);
			tmp = (row){"", "", 0, "", ""};
		}
                | identifier_list COMMA ID dim ASSIGN_OP initial_array{
			string str($3);
			tmp.name= str;
			string str2($4);
			tmp.type = $4;
			tmpTable.push_back(tmp);
			tmp = (row){"", "", 0, "", ""};
		}
                | identifier_list COMMA ID dim{
			string str($3);
			tmp.name= str;
			string str2($4);
			tmp.type = $4;
			tmpTable.push_back(tmp);
			tmp = (row){"", "", 0, "", ""};
		}
                | ID dim ASSIGN_OP initial_array{
			string str($1);
			tmp.name= str;
			string str2($2);
			tmp.type = $2;
			tmpTable.push_back(tmp);
			tmp = (row){"", "", 0, "", ""};
		}
                | ID dim{
			string str($1);
			tmp.name= str;
			string str2($2);
			tmp.type = $2;
			tmpTable.push_back(tmp);
			tmp = (row){"", "", 0, "", ""};
		}
                | ID ASSIGN_OP logical_expression{
			string str($1);
			tmp.name= str;
			tmpTable.push_back(tmp);
			tmp = (row){"", "", 0, "", ""};
		}
                | ID{
			string str($1);
			tmp.name= str;
			tmpTable.push_back(tmp);
			tmp = (row){"", "", 0, "", ""};
		}
                ;

initial_array : L_BRACE literal_list R_BRACE
              ;

literal_list : literal_list COMMA logical_expression
             | logical_expression
             | 
             ;

const_decl : CONST scalar_type const_list SEMICOLON{
		for(int i=0;i<tmpTable.size();i++){
			string str($2);
			tmpTable[i].type = str;
			insertRow(tmpTable[i], level, 1);
		}
		tmpTable.clear();
		tmp = (row){"", "", 0, "", ""};
	   }
	   ;

const_list : const_list COMMA ID ASSIGN_OP literal_const{
		string str($3);
		tmp.name = str;		
		tmp.kind = "constant";
		tmp.level = level;
		string str2($5);
		tmp.attribute = str2;
		tmpTable.push_back(tmp);
	   }
           | ID ASSIGN_OP literal_const{
		string str($1);
		tmp.name = str;		
		tmp.kind = "constant";
		tmp.level = level;
		string str2($3);
		tmp.attribute = str2;
		tmpTable.push_back(tmp);
	   }
           ;

//array_decl : ID dim;

dim : dim ML_BRACE INT_CONST MR_BRACE{
	string str($2);
	str += ($3);
	string str2($4);
	str += str2;
	string str3($1);
	str = str3 + str;
	strcpy($$, str.c_str());
    }
    | ML_BRACE INT_CONST MR_BRACE{
	string str($1);
	str += ($2);
	string str2($3);
	str += str2;
	strcpy($$, str.c_str());
    }
    ;

compound_statement : L_BRACE var_const_stmt_list R_BRACE{

		   }
                   ;

var_const_stmt_list : var_const_stmt_list statement{

		    }
                    | var_const_stmt_list var_decl{

		    }
                    | var_const_stmt_list const_decl{

		    }
                    |
		    {
			insertRow(tmp, level, 1);
		    }
                    ;

statement : compound_statement
          | simple_statement{

	  }
          | conditional_statement
          | while_statement
          | for_statement
          | function_invoke_statement{

	  }
          | jump_statement{

	  }
          ;     

simple_statement : variable_reference ASSIGN_OP logical_expression SEMICOLON
                 | PRINT logical_expression SEMICOLON
                 | READ variable_reference SEMICOLON
                 ;

conditional_statement : IF L_PAREN logical_expression R_PAREN L_BRACE var_const_stmt_list R_BRACE{
				if(Opt_Symbol) printTable(symbolTable[int(symbolTable.size())-1]);
				symbolTable.pop_back();
		      }
                      | IF L_PAREN logical_expression R_PAREN 
                            L_BRACE var_const_stmt_list R_BRACE
                        ELSE
                            L_BRACE var_const_stmt_list R_BRACE{
				if(Opt_Symbol) printTable(symbolTable[int(symbolTable.size())-1]);
				symbolTable.pop_back();
		      }
                      ;
while_statement : WHILE L_PAREN logical_expression R_PAREN
                    L_BRACE var_const_stmt_list R_BRACE{
				if(Opt_Symbol) printTable(symbolTable[int(symbolTable.size())-1]);
				symbolTable.pop_back();
		      }
                | DO L_BRACE
                    var_const_stmt_list
                  R_BRACE WHILE L_PAREN logical_expression R_PAREN SEMICOLON{
				if(Opt_Symbol) printTable(symbolTable[int(symbolTable.size())-1]);
				symbolTable.pop_back();
		      }
                ;


for_statement : FOR L_PAREN initial_expression_list SEMICOLON control_expression_list SEMICOLON increment_expression_list R_PAREN 
                    L_BRACE var_const_stmt_list R_BRACE{
			if(Opt_Symbol) printTable(symbolTable[int(symbolTable.size())-1]);
			symbolTable.pop_back();
	      }
              ;

initial_expression_list : initial_expression
                        |
                        ;

initial_expression : initial_expression COMMA variable_reference ASSIGN_OP logical_expression
                   | initial_expression COMMA logical_expression
                   | logical_expression
                   | variable_reference ASSIGN_OP logical_expression
		   ;

control_expression_list : control_expression
                        |
                        ;

control_expression : control_expression COMMA variable_reference ASSIGN_OP logical_expression
                   | control_expression COMMA logical_expression
                   | logical_expression
                   | variable_reference ASSIGN_OP logical_expression
                   ;

increment_expression_list : increment_expression 
                          |
                          ;

increment_expression : increment_expression COMMA variable_reference ASSIGN_OP logical_expression
                     | increment_expression COMMA logical_expression
                     | logical_expression
                     | variable_reference ASSIGN_OP logical_expression
                     ;

function_invoke_statement : ID L_PAREN logical_expression_list R_PAREN SEMICOLON
                          | ID L_PAREN R_PAREN SEMICOLON
                          ;

jump_statement : CONTINUE SEMICOLON
               | BREAK SEMICOLON
               | RETURN logical_expression SEMICOLON
               ;

variable_reference : array_list
                   | ID
                   ;


logical_expression : logical_expression OR_OP logical_term
                   | logical_term
                   ;

logical_term : logical_term AND_OP logical_factor
             | logical_factor
             ;

logical_factor : NOT_OP logical_factor
               | relation_expression
               ;

relation_expression : relation_expression relation_operator arithmetic_expression
                    | arithmetic_expression
                    ;

relation_operator : LT_OP
                  | LE_OP
                  | EQ_OP
                  | GE_OP
                  | GT_OP
                  | NE_OP
                  ;

arithmetic_expression : arithmetic_expression ADD_OP term
                      | arithmetic_expression SUB_OP term
                      | term
                      ;

term : term MUL_OP factor
     | term DIV_OP factor
     | term MOD_OP factor
     | factor
     ;

factor : SUB_OP factor
       | literal_const
       | variable_reference
       | L_PAREN logical_expression R_PAREN
       | ID L_PAREN logical_expression_list R_PAREN
       | ID L_PAREN R_PAREN
       ;

logical_expression_list : logical_expression_list COMMA logical_expression
                        | logical_expression
                        ;

array_list : ID dimension
           ;

dimension : dimension ML_BRACE logical_expression MR_BRACE         
          | ML_BRACE logical_expression MR_BRACE
          ;



scalar_type : INT {
		$$ = $1;
	    }
            | DOUBLE{
		$$ = $1;
	    }
            | STRING{
		$$ = $1;
	    }
            | BOOL {
		$$ = $1;
	    }
            | FLOAT {
		$$ = $1;
            }
            ;
 
literal_const : INT_CONST{
		$$ = $1;
	      }
              | FLOAT_CONST{
		$$ = $1;
	      }
              | SCIENTIFIC{
		$$ = $1;
	      }
              | STR_CONST{

	      }
              | TRUE{

	      }
              | FALSE{
		
	      }
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
    //  fprintf( stderr, "%s\t%d\t%s\t%s\n", "Error found in Line ", linenum, "next token: ", yytext );
}

void printTable(vector<row> table){
	printf("=======================================================================================\n"); 
	printf("Name");
	printf("%*c", 29, ' ');
	printf("Kind");
	printf("%*c", 7, ' ');
	printf("Level"); 
	printf("%*c", 7, ' ');
	printf("Type");
	printf("%*c", 15, ' ');
	printf("Attribute");
	printf("%*c", 15, ' ');
	printf("\n");
	printf("---------------------------------------------------------------------------------------\n");
	for(int i=0;i<int(table.size());i++){
		if(table[i].name != ""){
			stringstream ss;
			ss << table[i].level;
			string str = ss.str();

			cout << setiosflags( ios::left ) 
			<< setw(33) << table[i].name 
			<< setw(11) << table[i].kind 
			<< setw(12) << str + (table[i].level==0?"(global)":"(local)") 
			<< setw(19) << table[i].type 
			<< setw(15) << table[i].attribute;
			printf("\n");
		}
	}
	printf("=======================================================================================\n");
}

bool functionCheck(row tmp,vector<row> table){
	for(int i=0;i<table.size();i++){
		if(tmp.name == table[i].name && tmp.kind == table[i].kind && tmp.level == table[i].level && tmp.type == table[i].type && tmp.attribute == table[i].attribute)
			return false;

	}
	return true;
}

void insertRow(row tmp,int level,int control){
	// control 0 : insert front
	// control 1 : insert back
	vector<row> singleTable;
	while(level > int(symbolTable.size())-1){
		symbolTable.push_back(singleTable);	
	}
  	if(level < int(symbolTable.size())-1){
		//cout << level << endl;
		if(Opt_Symbol) printTable(symbolTable[level+1]);
		symbolTable.pop_back();
	}
	if(errorDection(tmp, level)){
		if(!control){
			symbolTable[level].insert(symbolTable[level].begin(), tmp);
		}else{
			symbolTable[level].push_back(tmp);
			//cout << symbolTable[level][int(symbolTable[level].size())-1].name << endl;
		}
	}
}	
void elseConditionPrint(){
	if(Opt_Symbol) printTable(symbolTable[int(symbolTable.size())-1]);
	symbolTable.pop_back();
}
bool errorDection(row tmp,int level){
	for(int i=0;i<(symbolTable[level].size());i++){
		if(tmp.name == symbolTable[level][i].name && tmp.name != ""){
			cout << "##########Error at Line " << linenum << ": " << tmp.name << " redeclared##########" << endl;
			return false;
		}
	}
	return true;
}
