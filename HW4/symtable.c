#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include"symtable.h"
#include <vector>
#include <string>
#include <iostream>
using namespace std;
extern int linenum;
extern int error;
int initSymTableList(struct SymTableList *list)
{
	list->head = NULL;
	list->tail = NULL;
	list->global = NULL;
	list->reference = 1;
	return 0;
}
int destroySymTableList(struct SymTableList *list)
{
	list->reference -= 1;//derefence
	if(list->reference>0)return -1;
	while(list->head!=NULL)
	{
		//kill head node
		list->head = deleteSymTable(list->head);//return new head
	}
	return 0;
}
struct SymTable* deleteSymTable(struct SymTable* target)
{
	struct SymTable *next;
	if(target==NULL)
		next = NULL;
	else
	{
		target->reference -= 1;//dereference
		if(target->reference>0)
			return NULL;
		next = target->next;
		while(target->head!=NULL)
		{
			target->head = deleteTableNode(target->head);
		}
	}
	if(next!=NULL)next->prev = NULL;
	return next;
}
int AddSymTable(struct SymTableList* list)//enter a new scope
{
	if(list->head == NULL)
	{
		struct SymTable *newTable = (struct SymTable*)malloc(sizeof(struct SymTable));
		newTable->head = NULL;
		newTable->tail = NULL;
		newTable->next = NULL;
		newTable->prev = NULL;
		list->head = newTable;
		list->tail = list->head;
		list->global = list->head;
		newTable->reference = 1;
	}
	else
	{
		struct SymTable *newTable = (struct SymTable*)malloc(sizeof(struct SymTable));
		newTable->head = NULL;
		newTable->tail = NULL;
		newTable->next = NULL;
		newTable->prev = list->tail;
		list->tail->next = newTable;
		list->tail = newTable;
		newTable->reference = 1;
	}
	return 0;
}
int deleteLastSymTable(struct SymTableList* list)//leave scope
{
	struct SymTable *temp = list->tail;
	if(temp==NULL)
		return -1;
	temp->reference -= 1;//derefence
	if(temp->reference>0)
		return -1;
	if(list->head!=list->tail)
	{
		temp->prev->next = NULL;
		list->tail = temp->prev;
	}
	else
	{
		list->tail = NULL;
		list->head = NULL;
	}
	deleteSymTable(temp);
	return 0;
}
int insertTableNode(struct SymTable *table,struct SymTableNode* newNode)
{
	if(table->tail==NULL)
	{
		table->head = newNode;
		table->tail = newNode;
	}
	else
	{
		table->tail->next = newNode;
		table->tail = newNode;
	}
	newNode->reference += 1;
	return 0;
}
struct SymTableNode* deleteTableNode(struct SymTableNode* target)//return next node
{
	struct SymTableNode *next;
	if(target==NULL)
		next = NULL;
	else
	{
		target->reference -= 1;//defreference
		if(target->reference>0)
			return NULL;
		next = target->next;
		killExtType(target->type);
		if(target->attr!=NULL)
			killAttribute(target->attr);
		free(target);
	}
	return next;
}



struct SymTableNode* createVariableNode(const char* name,int level,struct ExtType* type)
{
	struct SymTableNode *newNode = (struct SymTableNode*)malloc(sizeof(struct SymTableNode));
	//set node
	strncpy(newNode->name,name,32);
	newNode->kind = VARIABLE_t;
	newNode->level = level;
	/**/
	newNode->type = type;
	newNode->type->reference += 1;
	/**/
	newNode->attr = NULL;
	newNode->next = NULL;
	newNode->reference = 0;
	return newNode;
}
struct SymTableNode* createFunctionNode(const char* name,int level,struct ExtType* type,struct Attribute* attr)
{
	struct SymTableNode *newNode = (struct SymTableNode*)malloc(sizeof(struct SymTableNode));
	//set node
	strncpy(newNode->name,name,32);
	newNode->kind = FUNCTION_t;
	newNode->level = level;
	/**/
	newNode->type = type;
	newNode->type->reference += 1;
	/**/
	newNode->attr = attr;
	if(attr!=NULL)
		newNode->attr->reference += 1;
	/**/
	newNode->next = NULL;
	newNode->reference = 0;
	return newNode;

}
struct SymTableNode* createConstNode(const char* name,int level,struct ExtType* type,struct Attribute* attr)
{
	struct SymTableNode *newNode = (struct SymTableNode*)malloc(sizeof(struct SymTableNode));
	//set node
	strncpy(newNode->name,name,32);
	newNode->kind = CONSTANT_t;
	newNode->level = level;
	/**/
	newNode->type = type;
	newNode->type->reference += 1;
	/**/
	newNode->attr = attr;
	if(attr!=NULL)
		newNode->attr->reference += 1;
	/**/
	newNode->next = NULL;
	newNode->reference = 0;
	return newNode;
}
struct SymTableNode* createParameterNode(const char* name,int level,struct ExtType* type)
{
	struct SymTableNode *newNode = (struct SymTableNode*)malloc(sizeof(struct SymTableNode));
	//set node
	strncpy(newNode->name,name,32);
	newNode->kind = PARAMETER_t;
	newNode->level = level;
	/**/
	newNode->type = type;
	newNode->type->reference+=1;
	/**/
	newNode->attr = NULL;
	newNode->next = NULL;
	newNode->reference = 0;
	return newNode;
}
struct Attribute* createFunctionAttribute(struct FuncAttrNode* list)
{
	int num = 0;
	struct Attribute *newAttr = (struct Attribute*)malloc(sizeof(struct Attribute));
	newAttr->constVal = NULL;
	newAttr->funcParam = (struct FuncAttr*)malloc(sizeof(struct FuncAttr));
	newAttr->funcParam->reference = 1;
	/**/
	newAttr->funcParam->head = list;
	newAttr->funcParam->head->reference += 1;
	/**/
	while(list!=NULL)
	{
		list = list->next;
		++num;
	}
	newAttr->funcParam->paramNum = num;
	newAttr->reference = 0;
	return newAttr;
}
struct Attribute* createConstantAttribute(BTYPE type,void* value)
{
	struct Attribute *newAttr = (struct Attribute*)malloc(sizeof(struct Attribute));
	struct ConstAttr *newConstAttr = (struct ConstAttr*)malloc(sizeof(struct ConstAttr));
	newAttr->funcParam = NULL;
	newAttr->constVal = newConstAttr;
	newConstAttr->reference = 1;
	newConstAttr->hasMinus = false;
	newConstAttr->type = type;
	//printf("%d", type);
	switch(type)
	{
		case INT_t:
			newConstAttr->value.integerVal = *(int*)value;
			if(*(int*)value < 0)
				newConstAttr->hasMinus = true;
			break;
		case FLOAT_t:
			newConstAttr->value.floatVal = *(float*)value;
			if(*(float*)value < 0.0)
				newConstAttr->hasMinus = true;
			break;
		case DOUBLE_t:
			newConstAttr->value.doubleVal = *(double*)value;
			if(*(double*)value < 0.0)
				newConstAttr->hasMinus = true;
			break;
		case BOOL_t:
			newConstAttr->value.boolVal = *(bool*)value;
			break;
		case STRING_t:
			newConstAttr->value.stringVal = strdup((char*)value);
			break;
		default:
			break;
	}
	newAttr->reference = 0;
	return newAttr;
}
struct FuncAttrNode* deleteFuncAttrNode(struct FuncAttrNode* target)
{
	struct FuncAttrNode *next;
	if(target==NULL)
		next=NULL;
	else
	{
		target->reference -= 1;
		if(target->reference>0)
			return NULL;
		next = target->next;
		killExtType(target->value);
		free(target->name);
		free(target);
	}
	return next;
}
int killAttribute(struct Attribute* target)
{
	target->reference -= 1;
	if(target->reference>0)
		return -1;
	if(target->constVal!=NULL)
	{
		target->constVal->reference -= 1;
		if(target->constVal->reference>0)
			return -1;
		if(target->constVal->type==STRING_t)
			free(target->constVal->value.stringVal);
		free(target->constVal);
	}
	if(target->funcParam!=NULL)
	{
		target->funcParam->reference -= 1;
		if(target->funcParam->reference>0)
			return -1;
		target->funcParam->head = deleteFuncAttrNode(target->funcParam->head);
		free(target->funcParam);
	}
	free(target);
	return 0;
}
struct FuncAttrNode* createFuncAttrNode(struct ExtType* type,const char* name)
{
	struct FuncAttrNode *newNode = (struct FuncAttrNode*)malloc(sizeof(struct FuncAttrNode));
	/*reference*/
	newNode->value = type;
	type->reference += 1;
	/*         */
	newNode->name = strdup(name);
	newNode->next = NULL;
	newNode->reference = 0;
	return newNode;
}
int connectFuncAttrNode(struct FuncAttrNode* head, struct FuncAttrNode* newNode)//connect node to tail of function attribute list
{
	if(head==NULL || newNode==NULL || head==newNode)
		return -1;
	struct FuncAttrNode *temp = head;
	while(temp->next!=NULL)
	{
		temp = temp->next;
	}
	temp->next = newNode;
	newNode->reference += 1;
	return 0;
}
struct ExtType* createExtType(BTYPE baseType,bool isArray,struct ArrayDimNode* dimArray)
{
	int dimNum = 0;
	struct ArrayDimNode *temp = dimArray;
	struct ExtType *newExtType = (struct ExtType*)malloc(sizeof(struct ExtType));
	newExtType->baseType = baseType;
	newExtType->isArray = isArray;
	/**/
	newExtType->dimArray = dimArray;
	if(dimArray!=NULL)
		dimArray->reference += 1;
	/**/
	newExtType->reference = 0;
	while(temp!=NULL)
	{
		temp = temp->next;
		++dimNum;
	}
	newExtType->dim = dimNum;
	return newExtType;
}
int killExtType(struct ExtType* target)
{
	if(target==NULL)
		return -1;
	target->reference -= 1;
	if(target->reference>0)
		return -1;
	if(target->isArray)
	{
		while(target->dimArray!=NULL)
		{
			target->dimArray = deleteArrayDimNode(target->dimArray);
		}
	}
	return 0;
}
struct ArrayDimNode* createArrayDimNode(int size)
{
	struct ArrayDimNode *newNode = (struct ArrayDimNode*)malloc(sizeof(struct ArrayDimNode));
	if(size <= 0){
		error = 1;
		printf("##########Error at Line %d: ", linenum);
		printf("Array dimension must be greater than zero.");
		printf("##########\n");

	}
	newNode->size = size;
	newNode->next = NULL;
	newNode->reference = 0;
	return newNode;
}
int connectArrayDimNode(struct ArrayDimNode* head,struct ArrayDimNode* newNode)//connect dimension node to tail of list
{
	if(head==NULL || newNode==NULL || head==newNode)
		return -1;
	struct ArrayDimNode *temp = head;
	while(temp->next!=NULL)
	{
		temp = temp->next;
	}
	/**/
	temp->next = newNode;
	newNode->reference += 1;
	/**/
	return 0;
}
struct ArrayDimNode* deleteArrayDimNode(struct ArrayDimNode* target)
{
	struct ArrayDimNode *next;
	if(target==NULL)
		next = NULL;
	else
	{
		target->reference -= 1;
		if(target->reference>0)
			return NULL;
		next = target->next;
		free(target);
	}
	return next;
}


struct SymTableNode* findFuncDeclaration(struct SymTable* table,const char* name)
{
	struct SymTableNode *temp = table->head;
	while(temp!=NULL)
	{
		if(temp->kind == FUNCTION_t)
		{
			if(strncmp(temp->name,name,32)==0)
				return temp;
		}
		temp = temp->next;
	}
	return NULL;
}
int printSymTable(struct SymTable* table)
{
	struct SymTableNode *entry;
	struct ArrayDimNode *dimNode;
	struct Attribute *attr;
	struct FuncAttrNode *funcAttrNode;
	char strBuffer[32];
	if(table==NULL)return -1;
	if(table->head==NULL)return 1;//no entry to output
	printf("=======================================================================================\n");
	// Name [29 blanks] Kind [7 blanks] Level [7 blank] Type [15 blanks] Attribute [15 blanks]
	printf("Name                             Kind       Level       Type               Attribute               \n");
	printf("---------------------------------------------------------------------------------------\n");
	entry = table->head;
	while(entry!=NULL)
	{
		//name
		printf("%-32s ",entry->name);
		//kind
		switch(entry->kind)
		{
			case VARIABLE_t:
				printf("%-11s","variable");
				break;
			case CONSTANT_t:
				printf("%-11s","constant");
				break;
			case FUNCTION_t:
				printf("%-11s","function");
				break;
			case PARAMETER_t:
				printf("%-11s","parameter");
				break;
			default:
				printf("%-11s","unknown");
				break;
		}
		//level
		if(entry->level==0)
			printf("%-12s","0(global) ");
		else
		{
			sprintf(strBuffer,"%d(local)  ",entry->level);
			printf("%-12s",strBuffer);
		}
		//type
		printType(entry->type);
		//attribute
		attr = entry->attr;
		if(attr!=NULL)
		{
			if(attr->constVal!=NULL)
			{
				printConstAttribute(attr->constVal);
			}
			if(attr->funcParam!=NULL)
			{
				printParamAttribute(attr->funcParam);
			}
		}
		entry = entry->next;
		printf("\n");
	}
	printf("======================================================================================\n");
}
int printType(struct ExtType* extType)
{
	struct ArrayDimNode *dimNode;
	char strBuffer[20];
	char strTemp[20];
	if(extType == NULL)
		return -1;
	memset(strBuffer,0,20*sizeof(char));
	switch(extType->baseType)
	{
		case INT_t:
			strncpy(strBuffer,"int",3);
			break;
		case FLOAT_t:
			strncpy(strBuffer,"float",5);
			break;
		case DOUBLE_t:
			strncpy(strBuffer,"double",6);
			break;
		case BOOL_t:
			strncpy(strBuffer,"bool",4);
			break;
		case STRING_t:
			strncpy(strBuffer,"string",6);
			break;
		case VOID_t:
			strncpy(strBuffer,"void",4);
			break;
		default:
			strncpy(strBuffer,"unknown",7);
			break;
	}
	if(extType->isArray)
	{
		dimNode = extType->dimArray;
		while(dimNode!=NULL)
		{
			memset(strTemp,0,20*sizeof(char));
			sprintf(strTemp,"[%d]",dimNode->size);
			if(strlen(strBuffer)+strlen(strTemp)<20)
				strcat(strBuffer,strTemp);
			else
			{
				strBuffer[16]='.';
				strBuffer[17]='.';
				strBuffer[18]='.';
			}
			dimNode = dimNode->next;
		}
	}
	printf("%-19s",strBuffer);
	return 0;
}
int printConstAttribute(struct ConstAttr* constAttr)
{
	switch(constAttr->type)
	{
		case INT_t:
			printf("%d",constAttr->value.integerVal);
			break;
		case FLOAT_t:
			printf("%f",constAttr->value.floatVal);
			break;
		case DOUBLE_t:
			printf("%lf",constAttr->value.doubleVal);
			break;
		case BOOL_t:
			if(constAttr->value.boolVal)
				printf("true");
			else
				printf("false");
			break;
		case STRING_t:
			printf("%s",constAttr->value.stringVal);
			break;
		default:
			printf("__ERROR__");
			break;
	}
	return 0;
}
int printParamAttribute(struct FuncAttr* funcAttr)
{
	struct FuncAttrNode* attrNode = funcAttr->head;
	struct ArrayDimNode* dimNode;
	if(attrNode!=NULL)
	{
		switch(attrNode->value->baseType)
		{
			case INT_t:
				printf("int");
				break;
			case FLOAT_t:
				printf("float");
				break;
			case DOUBLE_t:
				printf("double");
				break;
			case BOOL_t:
				printf("bool");
				break;
			case STRING_t:
				printf("string");
				break;
			case VOID_t:
				printf("void");
				break;
			default:
				printf("unknown");
				break;
		}
		if(attrNode->value->isArray)
		{
			dimNode = attrNode->value->dimArray;
			while(dimNode!=NULL)
			{
				printf("[%d]",dimNode->size);
				dimNode = dimNode->next;
			}
		}
		attrNode = attrNode->next;
		while(attrNode!=NULL)
		{
			switch(attrNode->value->baseType)
			{
				case INT_t:
					printf(",int");
					break;
				case FLOAT_t:
					printf(",float");
					break;
				case DOUBLE_t:
					printf(",double");
					break;
				case BOOL_t:
					printf(",bool");
					break;
				case STRING_t:
					printf(",string");
					break;
				case VOID_t:
					printf(",void");
					break;
				default:
					printf(",unknown");
					break;
			}
			if(attrNode->value->isArray)
			{
				dimNode = attrNode->value->dimArray;
				while(dimNode!=NULL)
				{
					printf("[%d]",dimNode->size);
					dimNode = dimNode->next;
				}
			}
			attrNode = attrNode->next;
		}
	}
	return 0;
}
struct VariableList* createVariableList(struct Variable* head)
{	
	struct VariableList *list;
	if(head==NULL)
		list = NULL;
	else
	{
		list = (struct VariableList*)malloc(sizeof(struct VariableList));
		struct Variable* temp = head;
		while(temp->next!=NULL)
		{
			temp = temp->next;
		}
		/**/
		list->head = head;
		head->reference += 1;
		/**/
		list->tail = temp;
		if(head!=temp)
			temp->reference += 1;
		/**/
		list->reference = 0;
	}
	return list;
}
int deleteVariableList(struct VariableList* list)
{
	list->reference -= 1;
	if(list->reference>0)
		return -1;
	if(list->head!=NULL)
	{
		/**/
		//list->head = NULL
		list->head->reference -= 1;
		/**/
		if(list->head!=list->tail)
		{
			//list->tail = NULL
			list->tail->reference -= 1;
		}
		/**/
		while(list->head!=NULL)
		{
			list->head=deleteVariable(list->head);
		}
	}
	return 0;
}
int connectVariableList(struct VariableList* list,struct Variable* node)
{
	if(list==NULL||node==NULL)
		return -1;
	if(node->next!=NULL)
		return -2;
	if(list->tail!=list->head)
		list->tail->reference -= 1;
	/**/
	list->tail->next = node;
	list->tail->next->reference += 1;
	list->tail = node;
	list->tail->reference += 1;
	/**/
	return 0;
}
struct Variable* createVariable(const char* name,struct ExtType* type)
{
	struct Variable* variable = (struct Variable*)malloc(sizeof(struct Variable));
	variable->name = strdup(name);
	/**/
	variable->type = type;
	type->reference += 1;
	/**/
	variable->next = NULL;
	variable->reference = 0;
	return variable;
}
struct Variable* deleteVariable(struct Variable* target)
{
	struct Variable* next;
	if(target == NULL)
		next = NULL;
	else
	{
		target->reference -= 1;
		if(target->reference>0)
			return NULL;
		free(target->name);
		killExtType(target->type);
		next = target->next;
		free(target);
	}
	return next;
}
void printBTYPE(BTYPE t1)
{
	switch(t1)
	{
		case INT_t:
			printf("int");
			break;
		case FLOAT_t:
			printf("float");
			break;
		case DOUBLE_t:
			printf("double");
			break;
		case BOOL_t:
			printf("bool");
			break;
		case STRING_t:
			printf("string");
			break;
		case VOID_t:
			printf("void");
			break;
		case array:
			printf("array");
			break;
		default:
			printf("unknown");
			break;
	}
}
BTYPE arithmeticTypeCheck(BTYPE t1, BTYPE t2)
{
	if(t1 == BOOL_t || t1 == STRING_t || t1 == VOID_t || t2 == BOOL_t || t2 == STRING_t || t2 == VOID_t || t1 == array || t2 == array){
		printf("##########Error at Line %d: ", linenum);
		printBTYPE(t1);
		printf(" and ");
		printBTYPE(t2);
		printf(" do arithmetic operator.##########\n");
		error = 1;
		return unknown;
	}
	if(t1 == t2){
		return t1;
	}else if(t1 == DOUBLE_t || t2 == DOUBLE_t){
		return DOUBLE_t;
	}else if(t1 == FLOAT_t || t2 == FLOAT_t){
		return FLOAT_t;
	}else if(t1 == INT_t || t2 == INT_t){
		return INT_t;
	}		
}
BTYPE modTypeCheck(BTYPE t1, BTYPE t2)
{
	if(t1 == INT_t && t2 == INT_t){
		return INT_t;
	}else{
		printf("##########Error at Line %d: ", linenum);
		printBTYPE(t1);
		printf(" and ");
		printBTYPE(t2);
		printf(" do mod operator.##########\n");
		error = 1;
		return unknown;
	}		
}

BTYPE boolTypeCheck(BTYPE t1, BTYPE t2)
{
	if(t1 == BOOL_t && t2 == BOOL_t){
		return BOOL_t;	
	}
	printf("##########Error at Line %d: ", linenum);
	printBTYPE(t1);
	printf(" and ");
	printBTYPE(t2);
	printf(" do bool operator.##########\n");
	error = 1;
	return 	unknown;

}
BTYPE checkIDdect(struct SymTable* table, const char* name)
{
	bool check = false;
	do{
		struct SymTableNode *entry;
		entry = table->head;	
		while(entry!=NULL)
		{
			if(strcmp(entry->name, name) == 0){			
				check = true;
				
				if(!entry->type->isArray){return entry->type->baseType;}
				else {return array; }					
			}
			entry = entry->next;
		}
		table = table -> prev;
	}while(table != NULL);
	if(!check)
	{
		error = 1;
		printf("##########Error at Line %d: ", linenum);
		printf("%s ", name);
		printf("is not declared in this scope.##########\n");	
		return unknown;
	}
}
void assignTypeCheck(BTYPE t1, BTYPE t2)
{
	bool a = false;
	if(t1 == t2)
	{
		if(t1 == array && t2 == array) a = true;
		if(t1 == unknown && t2 == unknown) a = true;

	}else if(t1 == DOUBLE_t)
	{
		if(t2 == DOUBLE_t || t2 == FLOAT_t || t2 == INT_t){
		
		}else{
			a = true;	
		}	

	}else if(t1 == FLOAT_t)
	{
		if(t2 == FLOAT_t || t2 == INT_t){
		
		}else{
			a = true;	
		}	

	}else{
		a = true;	
	}
	if(a){
		error = 1;
		printf("##########Error at Line %d: ", linenum);
		printf("type ");
		printBTYPE(t2);
		printf(" can't assign to type ");
		printBTYPE(t1);
		printf(".##########\n");
	}

}
void declAssignTypeCheck(BTYPE t1, BTYPE t2)
{
	bool a = false;
	if(t1 == t2 || t2 == unknown)
	{
		if(t1 == array && t2 == array) a = true;
		
	}else if(t1 == DOUBLE_t)
	{
		if(t2 == DOUBLE_t || t2 == FLOAT_t || t2 == INT_t){
		
		}else{
			a = true;	
		}	

	}else if(t1 == FLOAT_t)
	{
		if(t2 == FLOAT_t || t2 == INT_t){
		
		}else{
			a = true;	
		}	

	}else{
		a = true;	
	}
	if(a){
		error = 1;
		printf("##########Error at Line %d: ", linenum);
		printf("type ");
		printBTYPE(t2);
		printf(" can't assign to type ");
		printBTYPE(t1);
		printf(".##########\n");
	}
}
bool arrayDimCheck(BTYPE t1){
	if(t1 == INT_t)
	{
		return true;
	}else
	{
		error = 1;
		printf("##########Error at Line %d: ", linenum);
		printf("type ");
		printBTYPE(t1);
		printf(" can't be array dimension.");
		printf("##########\n");
		return false;
	}

}
bool redeclareCheck(struct SymTable* table, const char* name)
{
	struct SymTableNode *entry;
	entry = table->head;
	while(entry!=NULL)
	{
		if(strcmp(entry->name, name) == 0){		
			error = 1;	
			printf("##########Error at Line %d: ", linenum);
			printf("%s ", name);
			printf("is redeclared in this scope.##########\n");
			return true;
			break;							
		}
		entry = entry->next;
	}
	return false;

}
void conditionalStatementBoolCheck(BTYPE t1, int logical_expression_num){
	if( t1 != BOOL_t){
		error = 1;
		printf("##########Error at Line %d: ", logical_expression_num);
		printf("conditional expression ");
		printf("must be bool type.");
		printf("##########\n");

	}


}
void breakContinueCheck(int loop)
{
	if(!loop){
		error = 1;
		printf("##########Error at Line %d: ", linenum);
		printf("break and continue can only appear in loop statements.");
		printf("##########\n");

	}
}
void functDeclDefParamCheck(struct FuncAttrNode *attr1, struct FuncAttrNode *attr2)
{
	bool check = true;
	struct ArrayDimNode* dim1;
	struct ArrayDimNode* dim2;
	if(attr1 != NULL && attr2 != NULL)
	{
		
		do
		{
			if(attr1->value->baseType == attr2->value->baseType)
			{
				
			}else{
				check = false;
				break;			
			}
			if(attr1->value->isArray && attr2->value->isArray)
			{
				dim1 = attr1->value->dimArray;
				dim2 = attr2->value->dimArray;
				while(dim1 != NULL && dim2 != NULL)
				{
					if(dim1->size == dim2->size)
					{
					}else
					{
						check = false;
						break;					
					}
					//printf("[%d]",dimNode->size);
					dim1 = dim1->next;
					dim2 = dim2->next;
				}
				if(dim1 != NULL || dim2 != NULL) check = false;
			}else if(attr1->value->isArray != attr2->value->isArray){
				check = false;
			}
			if(!check)
			{
				break;		
			}
			attr1 = attr1->next;
			attr2 = attr2->next;
		}while(attr1 != NULL && attr2 != NULL);
		if(attr1 != NULL || attr2 != NULL) check = false;
	}else if(attr1 == NULL && attr2 == NULL)
	{
		
	}else
	{
		check = false;	
	}
	if(!check)
	{
		error = 1;
		printf("##########Error at Line %d: ", linenum);
		printf("Declaration and definition's parameter are not matched.");
		printf("##########\n");
	}
}
void functDeclDefNoParamError(const char* name)
{
	error = 1;
	printf("##########Error at Line %d: ", linenum);
	printf("function %s need some parameter.", name);
	printf("##########\n");

}
void functDeclDefParamError(const char* name)
{
	error = 1;
	printf("##########Error at Line %d: ", linenum);
	printf("function %s shouldn't have any parameter.", name);
	printf("##########\n");

}
void returnTypeCheck(BTYPE t1, BTYPE returnType[1000], int return_linenum[1000], int return_count)
{
	for(int i=0;i<return_count;i++){
		if(t1 != returnType[i])
		{
			if(t1 == DOUBLE_t && (returnType[i] == INT_t || returnType[i] == FLOAT_t)){}
			else if(t1 == FLOAT_t && returnType[i] == INT_t){}
			else{
				error = 1;
				printf("##########Error at Line %d: ", return_linenum[i]);
				printf("Return type should be ");
				printBTYPE(t1);
				printf(", but got ");
				printBTYPE(returnType[i]);
				printf(" type");
				printf(".##########\n");
			}
		}
	}
	if(return_count == 0 && t1 != VOID_t){
		error = 1;
		printf("##########Error at Line %d: ", linenum);
		printf("Return type should be ");
		printBTYPE(t1);
		printf(", but got no return");
		printf(".##########\n");
	}

}
void arrayDeclareCheck(vector<BTYPE> v1, struct ArrayDimNode* dimNode, BTYPE t1)
{
	int count = 1;
	while(dimNode!=NULL)
	{
		count *= dimNode->size;
		dimNode = dimNode->next;
	}
	if(v1.size() > count)
	{
		error = 1;
		printf("##########Error at Line %d: ", linenum);
		printf("Array size should be %d, but got %d.",count, int(v1.size()) );
		printf("##########\n");
	}
	for(int i=0;i<v1.size();i++)
	{
		BTYPE t2 = v1[i];
		bool c = false;
		if(t1 == BOOL_t || t1 == STRING_t || t1 == VOID_t){
			if(t2 != t1) c = true;
		}
		if(t1 == t2){
		}else if(t1 == DOUBLE_t){
			if(t2 != DOUBLE_t && t2 != FLOAT_t && t2 != INT_t) c = true;
		}else if(t1 == FLOAT_t){
			if(t2 != FLOAT_t && t2 != INT_t) c = true;
		}else if(t1 == INT_t){
			if(t2 != INT_t) c = true;
		}
		if(c){
			error = 1;
			printf("##########Error at Line %d: ", linenum);
			printf("Array type should be ");
			printBTYPE(t1);
			printf(", but got ");
			printBTYPE(v1[i]);
			printf(".##########\n");

		}
	}

}
bool arrayIndexCheck(BTYPE t1)
{
	if(t1 != INT_t){
		error = 1;
		printf("##########Error at Line %d: ", linenum);
		printf("Array index type should be int, but got ");
		printBTYPE(t1);
		printf(".##########\n");
		return false;	
	}
	return true;
}
BTYPE arrayNameCheck(struct SymTable* table, const char* name, vector< BTYPE > arrayCheck){	
	bool check = false;
	int count = 0;
	struct SymTableNode *entry;
	struct ArrayDimNode* dimNode;
	do{
		entry = table->head;	
		while(entry!=NULL)
		{
			if(strcmp(entry->name, name) == 0 && entry->type->isArray){			
				check = true;
				dimNode = entry->type->dimArray;
				//printf("c:%d %d\n", entry->type->dim, int(arrayCheck.size() ));
				//printf("%d\n", entry->type->baseType);
				if(arrayCheck.size() == entry->type->dim) return entry->type->baseType;
				else if(arrayCheck.size() > entry->type->dim){
					error = 1;
					printf("##########Error at Line %d: ", linenum);
					printf("%s ", name);
					printf("dimension should be %d, but got %d.##########\n", entry->type->dim, int(arrayCheck.size()));	
					return unknown;	
				}else return array;
				break;
			}else if(strcmp(entry->name, name) == 0 && !entry->type->isArray){
				error = 1;
				printf("##########Error at Line %d: ", linenum);
				printf("%s is not array.", name);
				printf("##########\n");
				return unknown;	
			}
			entry = entry->next;
		}
		table = table -> prev;
	}while(table != NULL);
	if(!check){
		error = 1;
		printf("##########Error at Line %d: ", linenum);
		printf("%s ", name);
		printf("is not declared in this scope.##########\n");	
		return unknown;				
	}else{
		

		
		/*while(dimNode!=NULL)
		{
			count++;
			printf("%d\n", dimNode->size);
			dimNode = dimNode->next;
		}
		if(arrayCheck.size() == entry->type->dim) return entry->type->baseType;
		else return array;
		*/
	}

	return unknown;
}
void functRedef(const char* name){
	error = 1;
	printf("##########Error at Line %d: ", linenum);
	printf("%s ", name);
	printf("is redefined.##########\n");	
}
void functRedecl(const char* name){
	error = 1;
	printf("##########Error at Line %d: ", linenum);
	printf("%s ", name);
	printf("is redeclared.##########\n");	
}
void functNotdef(string name){
	error = 1;
	printf("##########Error at Line %d: function ", linenum);
	cout << name ;
	//printf("%s ", name);
	printf(" is declared but not defined.##########\n");	
}
void scalarTypeChaek(BTYPE t1){
	if(t1 == INT_t || t1 == FLOAT_t || t1 == DOUBLE_t || t1 == STRING_t || t1 == BOOL_t){}
	else{
		error = 1;
		printf("##########Error at Line %d: ", linenum);
		printf("Print/Read must be scalar type.##########\n");	
	}


}
BTYPE functNoParamInvokeCheck(struct SymTable* table, const char* name)
{
	int check = 0;
	do{
		struct SymTableNode *entry;
		entry = table->head;	
		while(entry!=NULL)
		{
			if(strcmp(entry->name, name) == 0 && entry->kind == FUNCTION_t && entry->attr == NULL) return entry->type->baseType;
			else if(strcmp(entry->name, name) == 0 && entry->kind == FUNCTION_t){check = 1; break;}
			else if(strcmp(entry->name, name) == 0){check = 2; break;}
			entry = entry->next;
		}
		table = table -> prev;
	}while(table != NULL);
	if(check == 0){
		error = 1;
		printf("##########Error at Line %d: ", linenum);
		printf("function: %s ", name);
		printf("doesn't declared.##########\n");	
	}else if(check == 1)
	{
		error = 1;
		printf("##########Error at Line %d: ", linenum);
		printf("function: %s ", name);
		printf("must need parameter.##########\n");	
	}else if(check == 2)
	{
		error = 1;
		printf("##########Error at Line %d: ", linenum);
		printf("%s ", name);
		printf("is not a function.##########\n");	
	}	
	return unknown;
}
BTYPE functParamInvokeCheck(struct SymTable* table, const char* name, vector<BTYPE> v)
{
	int check = 0;
	do{
		struct SymTableNode *entry;
		entry = table->head;	
		while(entry!=NULL)
		{
			if(strcmp(entry->name, name) == 0 && entry->kind == FUNCTION_t && entry->attr == NULL){check = 1; break;}
			else if(strcmp(entry->name, name) == 0 && entry->kind == FUNCTION_t){
				if(int(v.size()) != entry->attr->funcParam->paramNum){check = 3; break;}
				else
				{
					struct FuncAttrNode* attrNode = entry->attr->funcParam->head;
					int i=0;					
					for(i=0;i<int(v.size());i++){
						if(v[i] == attrNode->value->baseType && !attrNode-> value->isArray){}
						else if(v[i] == array && attrNode-> value->isArray){}
						else{check = 4; break;}
					}
					if(i == v.size()) return entry->type->baseType;
					break;
				}
				
			}
			else if(strcmp(entry->name, name) == 0){check = 2; break;}
			entry = entry->next;
		}
		table = table -> prev;
	}while(table != NULL);
	if(check == 0){
		error = 1;
		printf("##########Error at Line %d: ", linenum);
		printf("function: %s ", name);
		printf("doesn't declared.##########\n");	
	}else if(check == 1)
	{
		error = 1;
		printf("##########Error at Line %d: ", linenum);
		printf("function: %s ", name);
		printf("should be no parameter.##########\n");	
	}else if(check == 2)
	{
		error = 1;
		printf("##########Error at Line %d: ", linenum);
		printf("%s ", name);
		printf("is not a function.##########\n");	
	}else if(check == 3)
	{
		error = 1;
		printf("##########Error at Line %d: ", linenum);
		printf("function: %s's ", name);
		printf("parameter number does not match.##########\n");
	}else if(check == 4)
	{
		error = 1;
		printf("##########Error at Line %d: ", linenum);
		printf("function: %s's ", name);
		printf("parameter type does not match.##########\n");
	}
	return unknown;
}
void forControlCheck(BTYPE t1){
	if(t1 != BOOL_t){
		error = 1;
		printf("##########Error at Line %d: ", linenum);
		printf("The control expression in For must be bool type.##########\n");

	}


}
