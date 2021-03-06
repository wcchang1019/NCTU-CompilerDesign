#include <stdio.h>
#include "header.h"

extern FILE* fpOut;
extern FILE* fpFor;
extern FILE* fpMain;
void genProgramStart(char* filename);
void genGlobalVariables(const char *name, struct PType *type);
void genType(struct PType *type);
void genToTmpChar(struct ConstAttr *attr);
void genLocalVarAssign(int localVarNum);
void genGlobalVarAssign();
void genLocalVarLoad(int localVarNum, SEMTYPE tt);
void genGlobalVarLoad(const char *name, char* filename, struct PType *type11);
void genNeg(SEMTYPE tt);
void genOneVarOperateLoad(struct expr_sem *exprs, OPERATOR o, struct SymTable *table, int scope, char *fileName, int neg);
void genTwoVarOperateLoad(struct expr_sem *exprs, struct expr_sem *exprs2, OPERATOR o, struct SymTable *table, int scope, char *fileName);
void genLocalVarStore(int localVarNum, SEMTYPE tt);
void genGlobalVarStore(const char *name, char* filename, struct PType *type11);
void genVarStore(struct expr_sem *exprs, struct SymTable *table, int scope, char *fileName);
void genPrint1();
void genPrint2(struct expr_sem *exprs, struct SymTable *table, int scope, char *fileName,int condition);
void genOneVarLoad(struct expr_sem *exprs, struct SymTable *table, int scope, char *fileName);
void genRead(struct expr_sem *exprs, struct SymTable *table, int scope, char *fileName);
void genReadType(struct PType *type);
void genFunctBegin(const char *functName, struct param_sem *par, struct PType *ptype);
void genFunctReturn(struct PType *funcReturn);
void genFunctNoReturn();
void genFunctEnd();
void genRelationOp( struct expr_sem *op1, OPERATOR operator, struct expr_sem *op2, struct SymTable *table, int scope, char *fileName, int count);
void genAndOrOp( struct expr_sem *op1, OPERATOR operator, struct expr_sem *op2, struct SymTable *table, int scope, char *fileName);
void genArithmeticOp( struct expr_sem *op1, OPERATOR operator, struct expr_sem *op2, struct SymTable *table, int scope, char *fileName);
void genUnaryNOT(struct expr_sem *op1, struct SymTable *table, int scope, char *fileName);
void genConstant(struct SymNode *node);
void genIfBegin(int relationCount);
void genOnlyIfEnd(int relationCount);
void genWhileConditionBegin(int relationCount);
void genWhileConditionEnd(int relationCount);
void genForConditionBegin(int relationCount);
void genForConditionEnd(int relationCount);
void genElseBegin(int relationCount);
void genElseEnd(int relationCount);
void genWhileBegin(int whileCount);
void genWhileEnd(int whileCount);
void genForBegin(int whileCount);
void genForEnd(int whileCount);
void genFunctInvoke(const char *functName, struct PType *type11, struct PTypeList *listPtr);