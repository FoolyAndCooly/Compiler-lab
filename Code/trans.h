#ifndef _TRANS_H_
#define _TRANS_H_

#include "node.h"
#include "type.h"

#define MAX_CODE_LENGTH 20

typedef struct Code{
    char str[MAX_CODE_LENGTH];
    Code* next;
}Code;

typedef struct Arg{
    char* name;
    Arg* next;
}Arg;

typedef struct Codelist{
    Code* tail;
}Codelist;
Codelist codelist;



void codelist_append(Code* code);
void Trans_Program();
void Trans_ExtDefList(Node* node);
void Trans_ExtDef(Node* node);
void Trans_FunDec(Node* node, Type retType);
void Trans_VarList(Node* node, Type func);
void Trans_ParamDec(Node* node, Type func);
void Trans_CompSt(Node* node, Type func);
void Trans_StmtList(Node* node, Type func);
void Trans_Stmt(Node* node, Type func);
void Trans_ExtDecList(Node* node, Type type);
void Trans_Specifier(Node* node);
void Trans_StructSpecifier(Node* node);
void Trans_OptTag(Node* node, int* reDefineCheck);
void Trans_Tag(Node* node);
void Trans_DefList(Node* node, Type structure);
void Trans_Def(Node* node, Type structure);
void Trans_DecList(Node* node, Type type, Type structure);
void Trans_Dec(Node* node, Type type, Type structure);
void Trans_Args(Node* node, FieldList list, char* name);
void Trans_VarDec(Node* node, Type type, Type fieldlist);
void Trans_Exp(Node* node);

#endif