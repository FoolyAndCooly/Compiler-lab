#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "node.h"
#include "type.h"
#include "semantic_analysis_error.h"
#include "symbol.h"
#include "semantic.h"

void Program() {
  table = create_symbol_table();
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
        FunDec(node->child[1], type);
	CompSt(node->child[2]);
    }
}

Type FunDec(Node* node, Type retType) {
    Type func = create_func();
    append_fieldlist(func, "", retType);
    if(strcmp(node->child[2]->name, "VarList") == 0) {
        VarList(node->child[2], func);
    }
    if (lookup_symbol(node->child[0]->attr) != NULL) {
        semErrOutput(DEFINE_FUNC_MULTIPLY, atoi(node->attr), node->child[0]->attr);
    } else {
        insert_symbol(node->child[0]->attr, node->lineNum, func);
    }
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
	Exp(node->child[1]);
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
        ExtDecList(node->child[2], type);
    }
}

Type Specifier(Node* node) {
    if (strcmp(node->child[0]->name, "TYPE") == 0) {
        if (strcmp(node->child[0]->attr, "int") == 0) {
            return create_basic(TYPE_INT);
	}
	if (strcmp(node->child[0]->attr, "float") == 0) {
            return create_basic(TYPE_FLOAT);
	}
    } else if (strcmp(node->child[0]->name, "StructSpecifier") == 0) {
        return StructSpecifier(node->child[0]);
    }
}

Type StructSpecifier(Node* node) {
    if (node->num == 5) {
        Type structure = OptTag(node->child[1]);
        if (structure != NULL) {
            DefList(node->child[3], structure);
	    }
        else {
            printf("Illegal OptTag?\n");
            assert(0);
	    }
    } 
    else if (node->num == 2) {
        return Tag(node->child[1]);
    }
}

Type OptTag(Node* node) {
    if (node != NULL) {
        Type ret = create_struct();
	    insert_symbol(node->child[0]->attr, node->lineNum, ret);
        return ret;
    }
    return NULL;
}

Type Tag(Node* node) {
    return NULL; // TODO
    // char* struct_name = node->child[0]->attr;
    // SymbolEntry* entry = lookup_symbol(struct_name);
    // 
    // if (entry == NULL) {
    //     semErrOutput(NOT_DEFINE_STRUCT, atoi(node->attr), );
    //     return NULL;
    // }
    // return entry->type;
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
        Type expType = Exp(node->child[2]);
        if (type->u.basic != expType->u.basic) {
            semErrOutput(NOT_MATCH_ASSIGNOP, atoi(node->attr), "");
	}
	VarDec(node->child[0], type, structure);
    }
}

void Args(Node* node, Type type) {

    // Args: NEED TO BE COMPLETED  

    if(node->num == 1)
        Exp(node->child[0]);
    else if(node->num == 3) {
        Exp(node->child[0]);
        Args(node->child[2], type);
    }
    else 
        assert(0);
}


void VarDec(Node* node, Type type, Type fieldlist) {
    if (node->num == 1) {
        if (fieldlist != NULL) {
	    append_fieldlist(fieldlist, node->child[0]->attr, type);
	    if (fieldlist->kind == FUNCTION) {insert_symbol(node->child[0]->attr, node->lineNum, type);}
	} else {
	    if (lookup_symbol(node->child[0]->attr) != NULL) {
                semErrOutput(DEFINE_VAR_MULTIPLY, atoi(node->attr), node->child[0]->attr);
	    } else {
                insert_symbol(node->child[0]->attr, node->lineNum, type);
            }
	}
    } else {
        Type array = create_array(type, atoi(node->child[2]->attr));
	VarDec(node->child[0], array, fieldlist);
    }
}




Type Exp(Node* node) {
    if (node == NULL) return NULL;
    if (node->num == 1) {
        // get an ID
        if (strcmp(node->child[0]->name, "ID") == 0) {
            SymbolEntry* symbol = lookup_symbol(node->child[0]->attr);
            if(symbol == NULL) {
                semErrOutput(NOT_DEFINE_VAR, atoi(node->attr), node->child[0]->attr);   
                return NULL;
            }

            return symbol->type;
        } else if (strcmp(node->child[0]->name, "INT") == 0) {
            return create_basic(TYPE_INT);
        } else if (strcmp(node->child[0]->name, "FLOAT") == 0) {
            return create_basic(TYPE_FLOAT);
        }
    }

    // LP Exp RP
    if (node->num == 3 && strcmp(node->child[0]->name, "LP") == 0) {
        return Exp(node->child[1]); 
    }


    if (node->num == 2) {
        Type type = Exp(node->child[1]);
        if (strcmp(node->child[0]->name, "MINUS") == 0) {
            if (type->kind != BASIC) {
                semErrOutput(NOT_MATCH_OPERATOR, atoi(node->attr), "");
                return NULL;
            }
            return type;
        }
        if (strcmp(node->child[0]->name, "NOT") == 0) {
            if (type->kind != BASIC || type->u.basic != TYPE_INT) {
                semErrOutput(NOT_MATCH_OPERATOR, atoi(node->attr), "");
                return NULL;
            }
            return create_basic(TYPE_INT);
        }
    }

    if (node->num == 3 && strcmp(node->child[2]->name, "Exp") == 0) {
        Type left = Exp(node->child[0]);
        Type right = Exp(node->child[2]);

         // Match types on left and right sides
        if (strcmp(node->child[1]->name, "ASSIGNOP") == 0) {
            if (strcmp(node->child[0]->child[0]->name, "ID") != 0) {
                semErrOutput(ONLY_RIGHT_VAL, atoi(node->attr), "");
	        return NULL;
	    }
            if (left->kind != right->kind || (left->kind == BASIC 
                && left->u.basic != right->u.basic)){
                semErrOutput(NOT_MATCH_ASSIGNOP, atoi(node->attr), "");
                return NULL;
            }
            return right;
        }
       
        if (strcmp(node->child[1]->name, "PLUS") == 0 || strcmp(node->child[1]->name, "MINUS") == 0||
            strcmp(node->child[1]->name, "STAR") == 0 || strcmp(node->child[1]->name, "DIV") == 0) {    
             if (left->kind != right->kind || (left->kind == BASIC 
                && left->u.basic != right->u.basic)){
                semErrOutput(NOT_MATCH_OPERATOR, atoi(node->attr), "");
                return NULL;
            }
	    return left;
        } 
        else if (strcmp(node->child[1]->name, "AND") == 0 || strcmp(node->child[1]->name, "OR") == 0) {
            return create_basic(TYPE_INT);
        } 
        else if (strcmp(node->child[1]->name, "RELOP") == 0) {
            return create_basic(TYPE_INT);
        }
    }

    // about array access
    // TODO
    if (node->num == 4 && strcmp(node->child[1]->name, "LB") == 0) {
        // Type array_type = Exp(node->child[0]);
        // Type index_type = Exp(node->child[2]);
        // 
        // if (array_type->kind != ARRAY) {
        //     semErrOutput(NOT_ARR_LB, node->atoi(attr), array_type);
        //     return NULL;
        // }
        // if(index_type->kind != BASIC || index_type->u.basic != TYPE_INT) {
        //     semErrOutput(NOT_INT_ACCESS_ARR);
        //     return NULL;
        // }
        // return array_type->u.array.elem;
    }

    // structure member access
    if (node->num == 3 && strcmp(node->child[1]->name, "DOT") == 0) {
    // TODO
    //     Type struct_type = Exp(node->child[0]);
    //     if (struct_type->kind != STRUCTURE) {
    //         semErrOutput(NOT_STRUCT_DOT);
    //         return NULL;
    //     }
    //     
    // // continue to complete this: find_member()
    //     FieldList field = find_field_member(struct_type, node->child[2]->attr);
    //     if (!field){
    //         semErrOutput(NOT_DEFINE_FIELD);
    //         return NULL;
    //     } 
    //     return field->type;
    }

    // about function
    if (node->num >= 3 && strcmp(node->child[1]->name, "LP") == 0) {
        SymbolEntry* func = lookup_symbol(node->child[0]->attr);
	if (func == NULL) {
            semErrOutput(NOT_DEFINE_FUNC, atoi(node->attr), node->child[0]->attr);
            return NULL;
	} else if (func->type->kind != FUNCTION) {
            semErrOutput(NOT_FUNC_LC, atoi(node->attr), node->child[0]->attr);
            return NULL;
        }
        
        // if (node->num == 4) { 
        //     if (func->type->u.fieldlist != NULL){
        //         semErrOutput(NOT_MATCH_FUNCPARA);
        //         return NULL;
        //     }  
        // } else {
        //     // check_arguments(func->type->u.fieldlist, Args(node->child[2]));
        // }
        return func->type->u.fieldlist->type;
    }


    printf("Unknown error case of Exp!\n");
    assert(0);
    return NULL;
}
