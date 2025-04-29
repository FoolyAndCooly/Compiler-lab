#ifndef _TRANS_H_
#define _TRANS_H_

#include "node.h"
#include "type.h"

#define MAX_CODE_LENGTH 20

typedef struct Code{
    char str[MAX_CODE_LENGTH];
    struct Code* next;
}Code;

typedef struct Arg{
    char* name;
    struct Arg* next;
}Arg;

typedef struct Codelist{
    Code* head;
    Code* tail;
}Codelist;
Codelist codelist;



void codelist_append(Code* code);
void Trans_Program();
void Trans_ExtDefList(Node* node);
void Trans_ExtDef(Node* node);
void Trans_FunDec(Node* node);
void Trans_VarList(Node* node);
void Trans_ParamDec(Node* node);
void Trans_CompSt(Node* node);
void Trans_StmtList(Node* node);
void Trans_Stmt(Node* node);
void Trans_ExtDecList(Node* node);
void Trans_Specifier(Node* node);
void Trans_StructSpecifier(Node* node);
void Trans_OptTag(Node* node);
void Trans_Tag(Node* node);
void Trans_DefList(Node* node);
void Trans_Def(Node* node);
void Trans_DecList(Node* node);
void Trans_Dec(Node* node);
void Trans_Args(Node* node, Arg** arg_list_head, Arg** arg_list_tail);
void Trans_VarDec(Node* node);
void Trans_Exp(Node* node, char* place);

#endif