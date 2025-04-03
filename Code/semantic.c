#include <stdlib.h>
#include <type.h>
#include <symbol.h>

extern Node* root;

void Program() {
  ExtDefList(root->child[0]);
}

void ExtDefList(Node* node) {
    if (node == NULL) return;
    ExtDef(node->child[0]);
    ExtDefList(node->child[1]);
}

void ExtDef(Node* node) {
    Type type = Specifier(node->child[0]);
    if (strcmp(node->child[1]->name, "ExtDecList") == 0) {
        ExtDecList(node->child[1], type);
    } else if (strcmp(node->child[1]->name, "FunDec") == 0) {
        FunDec(node->child[1]);
	Compst(node->child[2]);
    }
}

Type FunDec(Node* node) {
    Type func = create_func(node->child[0]->attr);
    VarList(node->child[2], func);
    insert_symbol(node->child[0]->name, atoi(node->attr), func);
    return func;
}

void VarList(Node* node, Type func) {
    ParamDec(node->child[0], func);
    if (node->num == 3) {
        VarList(node->child[2], func);
    }
}

void ParamDec(Node* node, Type func) {
    Type type = Specifier(node->child[0]);
    VarDec(node->child[1], type, func);
}

void CompSt(Node* node) {
    DefList(node->child[1], NULL);
    StmtList(node->child[2]);
}

void StmtList(Node* node) {
    if (node != NULL) {
        Stmt(node->child[0]);
	StmtList(node->child[1]);
    }
}

void Stmt(Node* node) {
    if (node->num == 2) {
        Exp(node->child[0]);
    } else if (node->num == 1) {
        CompSt(node->child[0]);
    } else if (node->num == 3) {
        // TODO
	Exp(node->child[2]);
    } else if (node->num == 5) {
        Exp(node->child[2]);
	Stmt(node->child[4]);
    } else if (strcmp(node->child[6]->name, "Stmt")) {
        Exp(node->child[2]);
	Stmt(node->child[4]);
	Stmt(node->child[6]);
    }
}


void ExtDecList(Node* node, Type type) {
    VarDec(node->child[0], type, NULL);
    if (node->num == 3) {
        ExtDecList(node->child[2]);
    }
}

Type Specifier(Node* node) {
    if (strcmp(node->child[0]->name, "TYPE")) {
        if (strcmp(node->child[0]->attr, "int")) {
            return create_basic(TYPE_INT);
	}
	if (strcmp(node->child[0]->attr, "float")) {
            return create_basic(TYPE_FLOAT);
	}
    } else if (strcmp(node->child[0]->name, "StructSpecifier")) {
        return StructSpecifier(node->child[0]);
    }
}

Type StructSpecifier(Node* node) {
    if (node->num == 5) {
        Type structure = OptTag(node->child[1]);
        if (ret != NULL) {
            DefList(node->child[3], structure);
	} else {
            // TODO
	}
    } else if (node->num == 2) {
        return Tag(node->child[1]);
    }
}
c
Type OptTag(Node* node) {
    if (node != NULL) {
        Type ret = create_struct(node->child[0]->attr);
	insert_symbol(node->child[0]->name, atoi(node->attr), ret);
        return ret;
    }
    return NULL;
}

Type Tag(Node* node) {
    Type ret = create_struct(node->child[0]->attr);
    insert_symbol(node->child[0]->name, atoi(node->attr), ret);
    return ret
}

void DefList(Node* node, Type structure) {
    if (node != NULL) {
        Def(node->child[0], structure);
	DefList(node->child[1], structure);
    }
}

void Def(Node* node, Type structure) {
    Type type = Specifier(node->child[0]);
    DecList(node->child[1], type, structure);
}

void DecList(Node* node, Type type, Type structure) {
    if (node->num == 1) {
        Dec(node->child[0], type, structure);
    } else {
        Dec(node->child[0], type, structure);
	DecList(node->child[2], type, structure);
    }
}

void Dec(Node* node, Type type, Type structure) {
    if (node->num == 1) {
        VarDec(node->child[0], type, structure);    
    } else {
        // TODO
    }
}

void VarDec(Node* node, Type type, Type fieldlist) {
    if (node->num == 1) {
        if (fieldlist != NULL) {
	    append_fieldlist(fieldlist, node->child[0]->attr, type);
	} else {
            insert_symbol(node->child[0]->name, atoi(node->attr), type);
	}
    } else {
        Type array = create_array(type, node->child[2]->attr);
        if (fieldlist != NULL) {
	    append_fieldlist(fieldlist, atoi(node->child[0]->attr), array);
	} else {
            insert_symbol(node->child[0]->name, atoi(node->attr), array);
	}
    }
}
