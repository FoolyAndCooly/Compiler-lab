#include "type.h"

void Program();
void ExtDefList(Node* node);
void ExtDef(Node* node);
Type FunDec(Node* node, Type retType);
void VarList(Node* node, Type func);
void ParamDec(Node* node, Type func);
void CompSt(Node* node, Type func);
void StmtList(Node* node, Type func);
void Stmt(Node* node, Type func);
void ExtDecList(Node* node, Type type);
Type Specifier(Node* node);
Type StructSpecifier(Node* node);
Type OptTag(Node* node, int* reDefineCheck);
Type Tag(Node* node);
void DefList(Node* node, Type structure);
void Def(Node* node, Type structure);
void DecList(Node* node, Type type, Type structure);
void Dec(Node* node, Type type, Type structure);
void Args(Node* node, FieldList list, char* name);
void VarDec(Node* node, Type type, Type fieldlist);
Type Exp(Node* node);


// 辅助函数：递归提取VarDec节点中的变量名
char* get_var_name(Node* node);