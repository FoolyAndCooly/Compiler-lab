#include "trans.h"
#include <stdio.h>

void codelist_append(Code* code) {
    codelist.tail->next = code;
    codelist.tail = code;
}

char* new_temp() {
    static unsigned int temp_counter = 0;       
    char* name = (char*)malloc(sizeof(char) * MAX_CODE_LENGTH);
    snprintf(name, 16, "t%d", temp_counter++);
    return name;
}

char* new_label() {
    static unsigned int label_counter = 0;       
    char* name = (char*)malloc(sizeof(char) * MAX_CODE_LENGTH);
    snprintf(name, 16, "L%d", label_counter++);
    return name;
}

void print_code(Code* code) {
    while(code) {
        printf("%s", code->str);
	code = code->next;
    }
}

void Trans_Program() {
    Code* code = (Code*)malloc(sizeof(Code));
    Trans_ExtDefList(root->child[0]);
    print_code(code->next);
}

void Trans_ExtDefList(Node* node) {
    if (node == NULL) return;
    Trans_ExtDef(node->child[0]);
    Trans_ExtDefList(node->child[1]);
}

void Trans_ExtDef(Node* node) {
    if (strcmp(node->child[1]->name, "ExtDecList") == 0) {
        Trans_ExtDecList(node->child[1], type);
    } else if (strcmp(node->child[1]->name, "FunDec") == 0) {
	Trans_CompSt(node->child[2]);
    }
}

void Trans_ExtDecList(Node* node, Type type){
	not finished
}



void Trans_Specifier(Node* node);
void Trans_StructSpecifier(Node* node);
void Trans_OptTag(Node* node, int* reDefineCheck);
void Trans_Tag(Node* node);


void Trans_CompSt(Node* node, Type func) {
    Trans_DefList(node->child[1]);
    Trans_StmtList(node->child[2]);
}

void Trans_StmtList(Node* node, Type func) {
    if (node != NULL) {
        Trans_Stmt(node->child[0], func);
	Trans_StmtList(node->child[1], func);
    }
}

void Trans_Cond(Node* node, char* label_true, char* label_false) {
    if (strcmp(node->child[1]->name, "RELOP") == 0) {
        char* t1 = new_temp();
	char* t2 = new_temp();
	Trans_Exp(node->child[0], t1);
	Trans_Exp(node->child[2], t2);
	Code* code1 = (Code*)malloc(sizeof(Code));
	sprintf(code1->str, "IF %s %s %s GOTO %s\n", t1, node->child[1]->attr, t2, label_true);
        codelist_append(code1);
	Code* code2 = (Code*)malloc(sizeof(Code));
	sprintf(code2->str, "GOTO %s\n", t1, node->child[1]->attr, t2, label_false);
        codelist_append(code2);
    } else if (strcmp(node->child[0]->name, "NOT") == 0) {
        Trans_Cond(node->child[1], label_false, label_true);
    } else if (strcmp(node->child[0]->name, "AND") == 0) {
        char* label = new_label();
	Trans_Cond(node->child[0], label1, label_false);
        Code* code1 = (Code*)malloc(sizeof(Code));
	sprintf(code1->str, "LABEL %s\n", label1);
	codelist_append(code1);
	Trans_Cond(node->child[2], label_true, label_false);
    } else if (strcmp(node->child[0]->name, "OR") == 0) {
        char* label = new_label();
	Trans_Cond(node->child[0], label_true, label1);
        Code* code1 = (Code*)malloc(sizeof(Code));
	sprintf(code1->str, "LABEL %s\n", label1);
	codelist_append(code1);
	Trans_Cond(node->child[2], label_true, label_false);
    } else {
        char* t1 = new_temp();
	Trans_Exp(node->child[0], t1);
        Code* code1 = (Code*)malloc(sizeof(Code));
	sprintf(code1->str, "IF %s != #0 GOTO %s\n", t1, label_true);
	codelist_append(code1);
        Code* code2 = (Code*)malloc(sizeof(Code));
	sprintf(code2->str, "GOTO %s\n", label_false);
	codelist_append(code2);
    }
}

void Trans_Stmt(Node* node) {
    if(!node || !func)   return;
    if (node->num == 2) {
        Trans_Exp(node->child[0], NULL);
    } else if (node->num == 1) {
        Trans_CompSt(node->child[0]);
    } else if (node->num == 3) { // return 
        char* t1 = new_temp();
	Trans_Exp(node->child[1], t1);
	Code* code2 = (Code*)malloc(sizeof(Code));
	sprintf(code2->str, "RETURN %s\n", t1);
        codelist_append(code2);
    } else if (node->num == 5) { // while
        char* label1 = new_label();
	char* label2 = new_label();
	char* label3 = new_label();
	Code* code1 = (Code*)malloc(sizeof(Code));
	sprintf(code1->str, "LABEL %s\n", label1);
	code_append(code1);
        Trans_Cond(node->child[2], label2, label3);
	Code* code2 = (Code*)malloc(sizeof(Code));
	sprintf(code2->str, "LABEL %s\n", label2);
	code_append(code2);
	Trans_Stmt(node->child[4]);
	Code* code3 = (Code*)malloc(sizeof(Code));
	sprintf(code3->str, "GOTO %s\n", label1);
	code_append(code3);
	Code* code4 = (Code*)malloc(sizeof(Code));
	sprintf(code4->str, "LABEL %s\n", label3);
	code_append(code4);
    } else if (strcmp(node->child[6]->name, "LOWER_THAN_ELSE")) { // if
        char* label1 = new_label();
	char* label2 = new_label();
        Trans_Cond(node->child[2], label1, label2);
        Code* code1 = (Code*)malloc(sizeof(Code));
	sprintf(code1->str, "LABEL %s\n", label1);
	code_append(code1);
	Trans_Stmt(node->child[4]);
	Code* code2 = (Code*)malloc(sizeof(Code));
	sprintf(code2->str, "LABEL %s\n", label2);
	code_append(code2);
    } else if (strcmp(node->child[6]->name, "Stmt")) { // if else
        char* label1 = new_label();
	char* label2 = new_label();
	char* label3 = new_label();
        Trans_Cond(node->child[2], label1, label2);
        Code* code1 = (Code*)malloc(sizeof(Code));
	sprintf(code1->str, "LABEL %s\n", label1);
	code_append(code1);
	Trans_Stmt(node->child[4]);
	Code* code2 = (Code*)malloc(sizeof(Code));
	sprintf(code2->str, "LABEL %s\n", label2);
	code_append(code2);
        Trans_Stmt(node->child[6]);
	Code* code3 = (Code*)malloc(sizeof(Code));
	sprintf(code3->str, "GOTO %s\n", label1);
	code_append(code3);
	Code* code4 = (Code*)malloc(sizeof(Code));
	sprintf(code4->str, "LABEL %s\n", label3);
	code_append(code4);
    }
}

void Trans_DefList(Node* node) {
    if (node != NULL) {
        Trans_Def(node->child[0], structure);
	Trans_DefList(node->child[1], structure);
    }
}

void Trans_Def(Node* node, Type structure) {
    Trans_DecList(node->child[1], type, structure);
}

void Trans_DecList(Node* node, Type type, Type structure) {
    if (node->num == 1) {
        // ID
        if (node->child[0]->num == 1) {
            Trans_Exp(node->child[2], node->child[0]->child[0]->attr);
	}
    } else {
        if (node->child[0]->num == 1) {
            Trans_Exp(node->child[2], node->child[0]->child[0]->attr);
	}
	Trans_DecList(node->child[2], type, structure);
    }
}

void Trans_Args(Node* node, Arg arg_list) {
    if (node->num == 1) {
        char* t1 = new_temp();
        Arg* new_arg = (Arg*)malloc(sizeof(Arg));
	new_arg->name = t1;
	arg_list->next = new_arg;
	Trans_Exp(node->child[0], t1);
    } else (node->num == 3) {
        char* t1 = new_temp();
	Trans_Exp(node->child[0], t1);
        Arg* new_arg = (Arg*)malloc(sizeof(Arg));
	new_arg->name = t1;
	arg_list->next = new_arg;
        Trans_Args(node->child[2], new_arg);
    }
}

void Trans_Exp(Node* node, char* place) {
    if (strcmp(node->child[0]->name, "INT") == 0) {
        Code* code1 = (Code*)malloc(sizeof(Code));
	sprintf(code1->str, "%s := #%s\n", place, node->child[0]->attr);
	codelist_append(code1);
    } else if (strcmp(node->child[0]->name, "ID") == 0) {
        Code* code1 = (Code*)malloc(sizeof(Code));
	sprintf(code1->str, "%s := #%s\n", place, node->child[0]->attr);
	codelist_append(code1);
    } else if (strcmp(node->child[1]->name, "ASSIGNOP") == 0) {
        // Exp1 -> ID
	if (node->child[0]->num == 1) {
	    char* t1 = new_temp();
	    Trans_Exp(node->child[2], t1);
	    Code* code2_1 = (Code*)malloc(sizeof(Code));
	    sprintf(code2_1->str, "%s := %s\n", node->child[0]->child[0]->attr, t1);
	    codelist_append(code2_1);
	    Code* code2_2 = (Code*)malloc(sizeof(Code));
	    sprintf(code2_2->str, "%s := %s\n", place, node->child[0]->child[0]->attr);
	    codelist_append(code2_2);
	}
    } else if (strcmp(node->child[1]->name, "PLUS") == 0) {
        char* t1 = new_temp();
	char* t2 = new_temp();
	Trans_Exp(node->child[0], t1);
	Trans_Exp(node->child[2], t2);
	Code* code3 = (Code*)malloc(Code);
	sprintf(code3->str, "%s := %s + %s\n", place, t1, t2);
	codelist_append(code3);
    } else if (strcmp(node->child[0]->name, "MINUS") == 0) {
        char* t1 = new_temp();
	Trans_Exp(node->child[1], t1);
	Code* code2 = (Code*)malloc(sizeof(Code));
	sprintf(code2->str, "%s := #0 - %s", place, t1);
	codelist_append(code2);
    } else if (strcmp(node->child[1]->name, "RELOP") == 0 || strcmp(node->child[1]->name, "AND") == 0 || strcmp(node->child[1]->name, "OR") == 0 || strcmp(node->child[0]->name, "NOT") == 0) {
        char* label1 = new_label();
	char* label2 = new_label();
        Code* code0 = (Code*)malloc(sizeof(Code));
	sprintf(code0->str, "%s := #0\n", place);
	codelist_append(code0);
        Trans_Cond(node->child[0], label1, label2);
        Code* code1 = (Code*)malloc(sizeof(Code));
	sprintf(code1->str, "LABEL %s\n", label1);
	codelist_append(code1);
        Code* code2 = (Code*)malloc(sizeof(Code));
	sprintf(code2->str, "%s := #1\n", place);
	codelist_append(code2);
        Code* code3 = (Code*)malloc(sizeof(Code));
	sprintf(code3->str, "LABEL %s\n", label2);
	codelist_append(code3);
    } else if (node->num == 3 && strcmp(node->child[0]->name, "ID") == 0) {
        if (strcmp(node->child[0]->attr, "read") == 0) {
            Code* code1 = (Code*)malloc(sizeof(Code));
	    sprintf(code1->str, "READ %s\n", place);
	    codelist_append(code1);
	} else {
            Code* code2 = (Code*)malloc(sizeof(Code));
	    sprintf(code2->str, "%s := CALL %s\n", place, node->child[0]->attr);
	    codelist_append(code2);
	} 
    } else if (node->num == 4 && strcmp(node->child[0]->name, "ID") == 0) {
	Args* head = (Args*)malloc(sizeof(Args));
	Args* arg_list = head;
	Trans_Args(node->child[2], arg_list);
	if (strcmp(node->child[0]->attr, "write")) {
            Code* code1 = (Code*)malloc(sizeof(Code));
	    sprintf(code1->str, "WRITE %s\n", arg_list->next->name);
	    codelist_append(code1);
	    Code* code2 = (Code*)malloc(sizeof(Code));
	    sprintf(code2->str, "%s := #0\n", place);
	    codelist_append(code2);
	}
	Args* cur = head->next;
	while(cur != NULL) {
	    Code* code3 = (Code*)malloc(sizeof(Code));
	    sprintf(code3->str, "Args %s\n", cur->name);
	    codelist_append(code3);
	    cur = cur->next;
	}
	Code* code4 = (Code*)malloc(sizeof(Code));
	sprintf(code4->str, "%s := CALL %s\n", place, node->child[0]->attr);
	codelist_append(code4);
    }
}
