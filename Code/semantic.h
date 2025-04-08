#include "type.h"

void Program();
void ExtDefList(Node* node);
void ExtDef(Node* node);
Type FunDec(Node* node, Type retType);
void VarList(Node* node, Type func);
void ParamDec(Node* node, Type func);
void CompSt(Node* node);
void StmtList(Node* node);
void Stmt(Node* node);
void ExtDecList(Node* node, Type type);
Type Specifier(Node* node);
Type StructSpecifier(Node* node);
Type OptTag(Node* node);
Type Tag(Node* node);
void DefList(Node* node, Type structure);
void Def(Node* node, Type structure);
void DecList(Node* node, Type type, Type structure);
void Dec(Node* node, Type type, Type structure);
void Args(Node* node, Type type);
void VarDec(Node* node, Type type, Type fieldlist);
Type Exp(Node* node);
