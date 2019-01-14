#include "generator.h"
#include <stdio.h>
#include "header.h"
#include "symtab.h"
#include <string.h>
char tmp[100][100];
int tmpCount = 0;
int tmptmp = 0;
void genProgramStart(char* filename){
	fprintf(fpOut, ".class public %s\n", filename);
	fprintf(fpOut, ".super java/lang/Object\n");
	fprintf(fpOut, ".field public static _sc Ljava/util/Scanner;\n");

}

void genFunctEnd(){
	fprintf(fpOut, ".end method\n");
}


void genGlobalVariables(const char *name, struct PType *type11){
	fprintf(fpOut, ".field public static %s ", name);
	genType(type11);
	//fprintf(fpOut, "\n");
}

void genType(struct PType *type11){
	switch(type11->type){
		case INTEGER_t:
		 	fprintf(fpOut, "I");
			break;
		case FLOAT_t:
		 	fprintf(fpOut, "F");
			break;
		case DOUBLE_t:
		 	fprintf(fpOut, "D");
			break;
		case BOOLEAN_t:
		 	fprintf(fpOut, "Z");
			break;
		case STRING_t:
			fprintf(fpOut, "Ljava/lang/String;");
			break;
		case VOID_t:
		 	fprintf(fpOut, "V");
			break;
	}
}
void genToTmpChar(struct ConstAttr *attr){
	switch(attr->category){
		case INTEGER_t:
			snprintf(tmp[tmpCount], sizeof(tmp[tmpCount]), "%d", attr->value.integerVal);
			break;
		case FLOAT_t:
			snprintf(tmp[tmpCount], sizeof(tmp[tmpCount]), "%lf", attr->value.floatVal);
			break;
		case DOUBLE_t:
			snprintf(tmp[tmpCount], sizeof(tmp[tmpCount]), "%lf", attr->value.doubleVal);
			break;
		case BOOLEAN_t:
			if(attr->value.booleanVal)
				snprintf(tmp[tmpCount], sizeof(tmp[tmpCount]), "%s", "1");
			else
				snprintf(tmp[tmpCount], sizeof(tmp[tmpCount]), "%s", "0");
			break;
		case STRING_t:
			if(!strcmp(attr->value.stringVal, "\n")){
				snprintf(tmp[tmpCount], sizeof(tmp[tmpCount]), "\"\\n%s\"", "");
			}else if(!strcmp(attr->value.stringVal, "\r")){
				snprintf(tmp[tmpCount], sizeof(tmp[tmpCount]), "\"\\r%s\"", "");
			}else if(!strcmp(attr->value.stringVal, "\t")){
				snprintf(tmp[tmpCount], sizeof(tmp[tmpCount]), "\"\\t%s\"", "");
			}else{
				snprintf(tmp[tmpCount], sizeof(tmp[tmpCount]), "\"%s\"", attr->value.stringVal);
			}
			break;
		case VOID_t:
			break;
	}
	tmpCount++;
}

void genLocalVarAssign(int localVarNum){
	fprintf(fpOut, "ldc %s\n", tmp[tmptmp]);
	tmptmp++;
	fprintf(fpOut, "istore %d\n", localVarNum);
}

void genGlobalVarAssign(){
	fprintf(fpOut, " = %s\n", tmp[tmptmp]);
	tmptmp++;
}

void genLocalVarLoad(int localVarNum, SEMTYPE tt){
	if(tt == FLOAT_t || tt == DOUBLE_t) fprintf(fpOut, "fload %d\n", localVarNum);
	else fprintf(fpOut, "iload %d\n", localVarNum);
}

void genGlobalVarLoad(const char *name, char* filename, struct PType *type11){
	fprintf(fpOut, "getstatic %s/%s ", filename, name);
	genType(type11);
	fprintf(fpOut, "\n");
}

void genNeg(SEMTYPE tt){
	if(tt == INTEGER_t){
		fprintf(fpOut, "ineg\n");
	}else{
		fprintf(fpOut, "fneg\n");
	}
}
void genOneVarOperateLoad(struct expr_sem *exprs, OPERATOR o, struct SymTable *table, int scope, char *fileName, int neg){
	if(exprs->isLiteralConst){
		fprintf(fpOut, "ldc %s\n", tmp[tmptmp]);
		tmptmp++;
	}else{
		struct SymNode *node = 0;
		node = lookupSymbol( table, exprs->varRef->id, scope, __FALSE );
		if(node->category == CONSTANT_t){
			genConstant(node);
		}else if(node->scope != 0){
			genLocalVarLoad(node->localVarNum, exprs->pType->type);
		}else{
			genGlobalVarLoad(exprs->varRef->id, fileName, exprs->pType);
		}
		if(exprs->beginningOp == SUB_t){
			genNeg(exprs->pType->type);
		}
	}
	if(exprs->pType->type == INTEGER_t){
		switch(o){
			case MUL_t:
				fprintf(fpOut, "imul\n");
				break;
			case DIV_t:
				fprintf(fpOut, "idiv\n");
				break;
			case MOD_t:
				fprintf(fpOut, "irem\n");
				break;
			case ADD_t:
				fprintf(fpOut, "iadd\n");
				break;
			case SUB_t:
				fprintf(fpOut, "isub\n");
				break;
		}
	}else{
		switch(o){
			case MUL_t:
				fprintf(fpOut, "fmul\n");
				break;
			case DIV_t:
				fprintf(fpOut, "fdiv\n");
				break;
			case ADD_t:
				fprintf(fpOut, "fadd\n");
				break;
			case SUB_t:
				fprintf(fpOut, "fsub\n");
				break;
		}
	}
	if(neg){
		genNeg(exprs->pType->type);
	}


}
void genTwoVarOperateLoad(struct expr_sem *exprs, struct expr_sem *exprs2, OPERATOR o, struct SymTable *table, int scope, char *fileName){
	if(exprs->isLiteralConst){
		fprintf(fpOut, "ldc %s\n", tmp[tmptmp]);
		tmptmp++;
	}else{
		struct SymNode *node = 0;
		node = lookupSymbol( table, exprs->varRef->id, scope, __FALSE );
		if(node->category == CONSTANT_t){
			genConstant(node);
		}else if(node->scope != 0){
			genLocalVarLoad(node->localVarNum, exprs->pType->type);
		}else{
			genGlobalVarLoad(exprs->varRef->id, fileName, exprs->pType);
		}
		if(exprs->beginningOp == SUB_t){
			genNeg(exprs->pType->type);
		}
	}
	if(exprs->pType->type == INTEGER_t && exprs2->pType->type != INTEGER_t){
		fprintf(fpOut, "i2f\n");
	}
	if(exprs2->isLiteralConst){
		fprintf(fpOut, "ldc %s\n", tmp[tmptmp]);
		tmptmp++;
	}else{
		struct SymNode *node = 0;
		node = lookupSymbol( table, exprs2->varRef->id, scope, __FALSE );
		if(node->category == CONSTANT_t){
			genConstant(node);
		}else if(node->scope != 0){
			genLocalVarLoad(node->localVarNum, exprs2->pType->type);
		}else{
			genGlobalVarLoad(exprs2->varRef->id, fileName, exprs2->pType);
		}
		if(exprs2->beginningOp == SUB_t){
			genNeg(exprs2->pType->type);
		}
	}
	if(exprs2->pType->type == INTEGER_t && exprs->pType->type != INTEGER_t){
		fprintf(fpOut, "i2f\n");
	}
	if(exprs->pType->type == INTEGER_t && exprs2->pType->type == INTEGER_t){
		switch(o){
			case MUL_t:
				fprintf(fpOut, "imul\n");
				break;
			case DIV_t:
				fprintf(fpOut, "idiv\n");
				break;
			case MOD_t:
				fprintf(fpOut, "irem\n");
				break;
			case ADD_t:
				fprintf(fpOut, "iadd\n");
				break;
			case SUB_t:
				fprintf(fpOut, "isub\n");
				break;
		}
	}else{
		switch(o){
			case MUL_t:
				fprintf(fpOut, "fmul\n");
				break;
			case DIV_t:
				fprintf(fpOut, "fdiv\n");
				break;
			case ADD_t:
				fprintf(fpOut, "fadd\n");
				break;
			case SUB_t:
				fprintf(fpOut, "fsub\n");
				break;
		}
	}
}
void genOneVarLoad(struct expr_sem *exprs, struct SymTable *table, int scope, char *fileName){
	if(!exprs->isCombine && !exprs->isFunctInvoke){
		if(exprs->isLiteralConst){
			{
				fprintf(fpOut, "ldc %s\n", tmp[tmptmp]);
			}
			tmptmp++;
		}else{
			struct SymNode *node = 0;
			node = lookupSymbol( table, exprs->varRef->id, scope, __FALSE );
			//printf("type:%d\n", node->category);
			if(node->category == CONSTANT_t){
				genConstant(node);
			}else if(node->scope != 0){
				genLocalVarLoad(node->localVarNum, exprs->pType->type);
			}else{
				genGlobalVarLoad(exprs->varRef->id, fileName, exprs->pType);
			}
			if(exprs->beginningOp == SUB_t){
				genNeg(exprs->pType->type);
			}
		}
	}

}
void genLocalVarStore(int localVarNum, SEMTYPE tt){
	if(tt == DOUBLE_t || tt == FLOAT_t)	fprintf(fpOut, "fstore %d\n", localVarNum);
	else fprintf(fpOut, "istore %d\n", localVarNum);

}

void genGlobalVarStore(const char *name, char* filename, struct PType *type11){
	fprintf(fpOut, "putstatic %s/%s ", filename, name);
	genType(type11);
	fprintf(fpOut, "\n");
}

void genVarStore(struct expr_sem *exprs, struct SymTable *table, int scope, char *fileName){

	struct SymNode *node = 0;
	node = lookupSymbol( table, exprs->varRef->id, scope, __FALSE );
	if(node->category == CONSTANT_t){
		genConstant(node);
	}else if(node->scope != 0){
		genLocalVarStore(node->localVarNum, exprs->pType->type);
	}else{
		genGlobalVarStore(exprs->varRef->id, fileName, exprs->pType);
	}

}

void genPrint1(){
	fprintf(fpOut, "getstatic java/lang/System/out Ljava/io/PrintStream;\n");
}

void genPrint2(struct expr_sem *exprs, struct SymTable *table, int scope, char *fileName, int condition){
	if(condition == 0){
		if(exprs->isLiteralConst){
			fprintf(fpOut, "ldc %s\n", tmp[tmptmp]);
			tmptmp++;
		}else{

			struct SymNode *node = 0;
			node = lookupSymbol( table, exprs->varRef->id, scope, __FALSE );	

			if(node->category == CONSTANT_t){
				genConstant(node);
			}else if(node->scope != 0){
				genLocalVarLoad(node->localVarNum, exprs->pType->type);
			}else{
				genGlobalVarLoad(exprs->varRef->id, fileName, exprs->pType);
			}
		}
	}
	fprintf(fpOut, "invokevirtual java/io/PrintStream/print(");
	genType(exprs->pType);
	fprintf(fpOut, ")V\n");
}
void genRead(struct expr_sem *exprs, struct SymTable *table, int scope, char *fileName){
	fprintf(fpOut, "getstatic %s/_sc Ljava/util/Scanner;\n", fileName);
	genReadType(exprs->pType);
	genVarStore(exprs, table, scope, fileName);
}

void genReadType(struct PType *type11){
	switch(type11->type){
		case INTEGER_t:
		 	fprintf(fpOut, "invokevirtual java/util/Scanner/nextInt()I\n");
			break;
		case FLOAT_t:
		 	fprintf(fpOut, "invokevirtual java/util/Scanner/nextFloat()F\n");
			break;
		case DOUBLE_t:
		 	fprintf(fpOut, "invokevirtual java/util/Scanner/nextDouble()D\n");
			break;
		case BOOLEAN_t:
		 	fprintf(fpOut, "invokevirtual java/util/Scanner/nextBoolean()Z\n");
			break;
	}
}
void genFunctBegin(const char *functName, struct param_sem *par, struct PType *ptype){
	fprintf(fpOut, ".method public static %s(", functName);
	if(!strcmp(functName, "main")){
		fprintf(fpOut, "[Ljava/lang/String;)V\n");
		fprintf(fpOut, "new java/util/Scanner\n");	
		fprintf(fpOut, "dup\n");	
		fprintf(fpOut, "getstatic java/lang/System/in Ljava/io/InputStream;\n");	
		fprintf(fpOut, "invokespecial java/util/Scanner/<init>(Ljava/io/InputStream;)V\n");	
		fprintf(fpOut, "putstatic output/_sc Ljava/util/Scanner;\n");	
	}else{
		struct param_sem *pars;
		pars = par;
		while(pars != NULL){
			genType(pars->pType);
			pars = pars->next;
		}
		fprintf(fpOut, ")");
		genType(ptype);
		fprintf(fpOut, "\n");
	}
	fprintf(fpOut, ".limit stack 100\n");
	fprintf(fpOut, ".limit locals 100\n");

}
void genFunctReturn(struct PType *funcReturn){
	switch(funcReturn->type){
		case INTEGER_t:
			fprintf(fpOut, "ireturn\n");
			break;
		case FLOAT_t:
			fprintf(fpOut, "freturn\n");
			break;
		case DOUBLE_t:
			fprintf(fpOut, "dreturn\n");
			break;
	}
}

void genFunctNoReturn(){
	fprintf(fpOut, "return\n");
}

void genRelationOp( struct expr_sem *op1, OPERATOR operator, struct expr_sem *op2, struct SymTable *table, int scope, char *fileName, int count){
	genOneVarLoad(op1, table, scope, fileName);
	genOneVarLoad(op2, table, scope, fileName);
	if(op1->pType->type == INTEGER_t && op2->pType->type == INTEGER_t){
		fprintf(fpOut, "isub\n");
	}else{
		fprintf(fpOut, "fcmpl\n");
	}
	switch(operator){
		case LT_t: 
			fprintf(fpOut, "iflt ");
			break;
		case LE_t: 
			fprintf(fpOut, "ifle ");
			break;
		case EQ_t:
			fprintf(fpOut, "ifeq ");
			break; 
		case GE_t:
			fprintf(fpOut, "ifge ");
			break; 
		case GT_t:
			fprintf(fpOut, "ifgt ");
			break; 
		case NE_t:
			fprintf(fpOut, "ifne ");
			break;
	}
	fprintf(fpOut, "Ltrue_%d\n", count);
	fprintf(fpOut, "iconst_0\n");
	fprintf(fpOut, "goto Lfalse_%d\n", count);
	fprintf(fpOut, "Ltrue_%d:\n", count);
	fprintf(fpOut, "iconst_1\n");
	fprintf(fpOut, "Lfalse_%d:\n", count);
}

void genAndOrOp( struct expr_sem *op1, OPERATOR operator, struct expr_sem *op2, struct SymTable *table, int scope, char *fileName){
	genOneVarLoad(op1, table, scope, fileName);
	genOneVarLoad(op2, table, scope, fileName);
	switch(operator){
		case AND_t: 
			fprintf(fpOut, "iand\n");
			break;
		case OR_t: 
			fprintf(fpOut, "ior\n");
			break;
	}


}

void genArithmeticOp( struct expr_sem *op1, OPERATOR operator, struct expr_sem *op2, struct SymTable *table, int scope, char *fileName){
	genOneVarLoad(op1, table, scope, fileName);
	if(op1->pType->type == INTEGER_t && op2->pType->type != INTEGER_t){
		fprintf(fpOut, "i2f\n");
	}
	genOneVarLoad(op2, table, scope, fileName);
	if(op1->pType->type != INTEGER_t && op2->pType->type == INTEGER_t){
		fprintf(fpOut, "i2f\n");
	}
	if(op1->pType->type == INTEGER_t && op2->pType->type == INTEGER_t){
		switch(operator){
			case MUL_t:
				fprintf(fpOut, "imul\n");
				break;
			case DIV_t:
				fprintf(fpOut, "idiv\n");
				break;
			case MOD_t:
				fprintf(fpOut, "irem\n");
				break;
			case ADD_t:
				fprintf(fpOut, "iadd\n");
				break;
			case SUB_t:
				fprintf(fpOut, "isub\n");
				break;
		}
	}else{
		switch(operator){
			case MUL_t:
				fprintf(fpOut, "fmul\n");
				break;
			case DIV_t:
				fprintf(fpOut, "fdiv\n");
				break;
			case ADD_t:
				fprintf(fpOut, "fadd\n");
				break;
			case SUB_t:
				fprintf(fpOut, "fsub\n");
				break;
		}
	}





}

void genUnaryNOT(struct expr_sem *op1, struct SymTable *table, int scope, char *fileName){
	genOneVarLoad(op1, table, scope, fileName);
	fprintf(fpOut, "ixor\n");
}

void genConstant(struct SymNode *node){
	switch(node->attribute->constVal->category){
		case INTEGER_t:
			fprintf(fpOut, "ldc %d\n", node->attribute->constVal->value.integerVal);
			break;
		case DOUBLE_t:
			fprintf(fpOut, "ldc %lf\n", node->attribute->constVal->value.doubleVal);
			break;
		case FLOAT_t:
			fprintf(fpOut, "ldc %lf\n", node->attribute->constVal->value.floatVal);
			break;
		 case BOOLEAN_t:
		 	if( node->attribute->constVal->value.booleanVal == __TRUE ) 
				fprintf(fpOut, "iconst_1\n");
			else
				fprintf(fpOut, "iconst_0\n");
			break;
		 case STRING_t:
		 	fprintf(fpOut, "ldc \"%s\"\n", node->attribute->constVal->value.stringVal);
			break;
	}
}

void genIfBegin(int relationCount){
	fprintf(fpOut, "ifeq Lelse_%d\n", relationCount);
}

void genOnlyIfEnd(int relationCount){
	fprintf(fpOut, "Lelse_%d:\n", relationCount);
}

void genWhileConditionBegin(int relationCount){
	fprintf(fpOut, "ifeq WhileC_%d\n", relationCount);
}

void genWhileConditionEnd(int relationCount){
	fprintf(fpOut, "WhileC_%d:\n", relationCount);
}

void genForConditionBegin(int relationCount){
	fprintf(fpOut, "ifeq ForC_%d\n", relationCount);
}

void genForConditionEnd(int relationCount){
	fprintf(fpOut, "ForC_%d:\n", relationCount);
}

void genElseBegin(int relationCount){
	fprintf(fpOut, "goto Lexit_%d\n", relationCount);
	fprintf(fpOut, "Lelse_%d:\n", relationCount);
}

void genElseEnd(int relationCount){
	fprintf(fpOut, "Lexit_%d:\n", relationCount);
}

void genWhileBegin(int whileCount){
	fprintf(fpOut, "wbegin_%d:\n", whileCount);
}

void genWhileEnd(int whileCount){
	fprintf(fpOut, "goto wbegin_%d\n", whileCount);

}

void genForBegin(int whileCount){
	fprintf(fpOut, "Fbegin_%d:\n", whileCount);
}

void genForEnd(int whileCount){
	fprintf(fpOut, "goto Fbegin_%d\n", whileCount);

}

void genFunctInvoke(const char *functName, struct PType *type11, struct PTypeList *listPtr){
	fprintf(fpOut, "invokestatic output/%s(", functName);
	struct PTypeList *list;
	list = listPtr;
	while(list!=NULL){
		genType(list->value);
		list = list->next;
	}
	fprintf(fpOut, ")");
	genType(type11);	
	fprintf(fpOut, "\n");
}