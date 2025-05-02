#include "lib.h"
#include "node.h"
#include "type.h"
#include "semantic_analysis_error.h"
#include "symbol.h"
#include "semantic.h"

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
    if(!type)   return;
    if (strcmp(node->child[1]->name, "ExtDecList") == 0) {
        ExtDecList(node->child[1], type);
    } else if (strcmp(node->child[1]->name, "FunDec") == 0) {
        Type func = FunDec(node->child[1], type);
	CompSt(node->child[2], func);
    }
}

Type FunDec(Node* node, Type retType) {
    Type func = create_func(retType, node->child[0]->attr);
    if(!node->child[2]){
        printf("Error: unexpected nullptr in FunDec\n");
        assert(0);
    }
    if(!insert_function_symbol(node->child[0]->attr, node->lineNum, func))
        return NULL;

    if(strcmp(node->child[2]->name, "VarList") == 0) {
        VarList(node->child[2], func);
    }
    return lookup_symbol_with_a_type(node->child[0]->attr, FUNCTION)->type;
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

void CompSt(Node* node, Type func) {
    enter_scope(); 
    DefList(node->child[1], NULL);
    StmtList(node->child[2], func);
    exit_scope();
}

void StmtList(Node* node, Type func) {
    if (node != NULL) {
        Stmt(node->child[0], func);
	StmtList(node->child[1], func);
    }
}

void Stmt(Node* node, Type func) {
    if(!node || !func)   return;
    if (node->num == 2) {
        Exp(node->child[0]);
    } else if (node->num == 1) {
        CompSt(node->child[0], NULL);
    } else if (node->num == 3) {
	Type type = Exp(node->child[1]);
    assert(func->kind == FUNCTION);
    assert(func->retType != NULL);
    if(!type)   return;
    if (cmp_type(type, func->retType) == 0) {
            semErrOutput(NOT_MATCH_RETURN, atoi(node->attr), "");
	        return ;
	}
    } else if (node->num == 5) {
        Exp(node->child[2]);
	    Stmt(node->child[4], func);
    } else if (strcmp(node->child[6]->name, "Stmt")) {
        Exp(node->child[2]);
	    Stmt(node->child[4], NULL);
        Stmt(node->child[6], NULL);
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
    printf("Error: specifier error: %s \n", node->child[0]->name);
    assert(0);
    return NULL;
}

Type StructSpecifier(Node* node) {
    if (node->num == 5) {
        int reDefineCheck = 0;
        Type structure = OptTag(node->child[1], &reDefineCheck);
        if (structure != NULL) {
            DefList(node->child[3], structure);
	    }
        else {
            // structure redefined.
            if(reDefineCheck){
                return NULL;
            }
            // logic for annoymous structure
            // annoymous Structure's name: "1", "2", "3", ....
            // is impossible for common structure name (only by number)
            static int anonyStructNum = 1;
            sprintf(buffer, "%d", anonyStructNum);
            char *struct_name = (char*)calloc(1, (strlen(buffer)+1) * sizeof(char));
            strcpy(struct_name, buffer);
            printf("struct name: %s\n", struct_name);
            Type ret = create_struct(struct_name);
            insert_symbol(struct_name, node->lineNum, ret);
            anonyStructNum += 1;
            Type struct_type = create_struct(struct_name);
            DefList(node->child[3], struct_type);
	    }
    } 
    else if (node->num == 2) {
        return Tag(node->child[1]);
    }
}

Type OptTag(Node* node, int* reDefineCheck) {
    assert(*reDefineCheck == 0);
    if (node != NULL) {
        assert(node->child[0] != NULL);
        char* struct_name = node->child[0]->attr;
        
        // struct_name redefinition check
        SymbolEntry* dup_name = lookup_symbol(node->name);
        if(dup_name){
        //    semErrOutput(DEFINE_STRUCT_MULTIPLY, node->child[0]->lineNum, struct_name);
            *reDefineCheck = 1;
            return NULL;
        } 

        Type ret = create_struct(struct_name);
        insert_symbol(struct_name, node->lineNum, ret);
        return ret;
    }
    return NULL;
}

Type Tag(Node* node) {
    // TODO
     char* struct_name = node->child[0]->attr;
     SymbolEntry* entry = lookup_symbol_with_a_type(struct_name, STRUCTURE);
     if (entry == NULL) {
         semErrOutput(NOT_DEFINE_STRUCT, atoi(node->attr), "");
         return NULL;
     }
     if(entry->type->kind != STRUCTURE){
        printf("Not a StructTag?\n");
        assert(0);
     }
     return entry->type;
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
        // error: field initialization in structure definition

        if(structure && structure->kind == STRUCTURE){
            semErrOutput(DEFINE_FIELD_MULTIPLY, node->lineNum, "");
            return;
        }
        
        Type expType = Exp(node->child[2]);
        if (!expType && !cmp_type(type, expType)) {
            semErrOutput(NOT_MATCH_ASSIGNOP, atoi(node->attr), "");
	}
	VarDec(node->child[0], type, structure);
    }
}

void Args(Node* node, FieldList list, char* name) {
    if (list == NULL) {
        semErrOutput(NOT_MATCH_FUNCPARA, atoi(node->attr), name);
        return ;
    }
    if (node->num == 1) {
        if (cmp_type(Exp(node->child[0]), list->type) == 0 | list->tail != NULL) {
            semErrOutput(NOT_MATCH_FUNCPARA, atoi(node->attr), name);
            return ;
	}
    } else {
        if (cmp_type(Exp(node->child[0]), list->type) == 0) {
            semErrOutput(NOT_MATCH_FUNCPARA, atoi(node->attr), name);
            return ;
	}
        Args(node, list->tail, name);
    }

}


void VarDec(Node* node, Type type, Type fieldlist) {
    if (!node || !type) return;

    // Case 1: 处理数组声明
    if (node->num != 1 && strcmp(node->child[1]->name, "LB") == 0) {
        // 递归处理内层维度（如 [4]）
        VarDec(node->child[0], type, fieldlist);

        // 获取变量名（如 "b"）
        char* var_name = get_var_name(node);
        if (!var_name) return;

        // 查找符号表，避免重复插入
        SymbolEntry* entry = lookup_symbol(var_name);
        if (!entry) {
            // 如果不存在，先插入初始类型（内层数组）
            insert_symbol(var_name, node->lineNum, type);
            entry = lookup_symbol(var_name);
        }

        // 创建外层数组类型（如 array(array(int,4),5）
        int outer_size = atoi(node->child[2]->attr);
        Type outer_array = create_array(entry->type, outer_size);

        // 更新符号表中的类型（关键修改：不重新插入，只更新类型）
        entry->type = outer_array;

        // 处理结构体字段或函数参数
        if (fieldlist != NULL) {
            if (fieldlist->kind == STRUCTURE) {
                append_fieldlist(fieldlist, var_name, outer_array);
            } else if (fieldlist->kind == FUNCTION) {
                append_fieldlist(fieldlist, var_name, outer_array);
            }
        }
    }
    // Case 2: 普通变量或结构体字段
    else if (node->num == 1 && strcmp(node->child[0]->name, "ID") == 0) {
        char* var_name = node->child[0]->attr;
        // 结构体字段检查
        if (fieldlist != NULL && fieldlist->kind == STRUCTURE) {
            FieldList cur = fieldlist->u.fieldlist;
            while (cur) {
                if (strcmp(cur->name, var_name) == 0) {
                    semErrOutput(DEFINE_FIELD_MULTIPLY, node->lineNum, var_name);
                    return;
                }
                cur = cur->tail;
            }
            append_fieldlist(fieldlist, var_name, type);
        }
        // 普通变量或函数参数（仅在不存在时插入）
        else {
            if (lookup_symbol(var_name)) {
                semErrOutput(DEFINE_VAR_MULTIPLY, node->lineNum, var_name);
            } else {
                insert_symbol(var_name, node->lineNum, type);
            }
        }
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
        if(!type)   return NULL;
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
        if(!left || !right) return NULL;
         // Match types on left and right sides
        if (strcmp(node->child[1]->name, "ASSIGNOP") == 0) {
	    // printf("val %d\n", is_lvalue(node));
            if (!is_lvalue(node)) {
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
            
                if (!cmp_type(left, right)){
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
    if (node->num == 4 && strcmp(node->child[1]->name, "LB") == 0) {
        Type array_type = Exp(node->child[0]);
        Type index_type = Exp(node->child[2]);
        if (!array_type || array_type->kind != ARRAY) {
            semErrOutput(NOT_ARR_LB, node->lineNum, "");
            return NULL;
        }
        if(!index_type || index_type->kind != BASIC || index_type->u.basic != TYPE_INT) {
            semErrOutput(NOT_INT_ACCESS_ARR, node->lineNum, "");
            return NULL;
        }
        return array_type->u.array.elem;
    }

    // structure member access
    if (node->num == 3 && strcmp(node->child[1]->name, "DOT") == 0) {
        // exp -> exp dot ... -> id
        if(!node->child[0] || !node->child[0]->child[0]){
            printf("Error: unexpected nullptr\n");
            assert(0);
        }
        Node* struct_node = node->child[0]->child[0]; 
        SymbolEntry* struct_symbol = lookup_symbol_with_a_type(struct_node->attr, STRUCTURE);
        if(!struct_symbol || struct_symbol->type->kind != STRUCTURE) {
            semErrOutput(NOT_STRUCT_DOT, node->lineNum, "");
            return NULL;
        }
        assert(struct_symbol->type);
         FieldList field = find_field_member(struct_symbol->type, node->child[2]->attr);
         if (!field){
             semErrOutput(NOT_DEFINE_FIELD, node->lineNum, "");
             return NULL;
         } 
         return field->type;
        }

    // about function
    if (node->num >= 3 && strcmp(node->child[1]->name, "LP") == 0) {
        SymbolEntry* func = lookup_symbol_with_a_type(node->child[0]->attr, FUNCTION);
	if (func == NULL) {
            func = lookup_symbol(node->child[0]->attr);
            if(func != NULL)
                semErrOutput(NOT_FUNC_LC, atoi(node->attr), node->child[0]->attr);
            else
                semErrOutput(NOT_DEFINE_FUNC, atoi(node->attr), node->child[0]->attr);
            return NULL;
	}   
        if (node->num == 3) { 
            if (func->type->u.fieldlist != NULL){
                semErrOutput(NOT_MATCH_FUNCPARA, atoi(node->attr), func->name);
                return NULL;
            }  
        } else {
	    Args(node->child[2], func->type->u.fieldlist, func->name);
        }
        return func->type->retType;
    }


    printf("Unknown error case of Exp!\n");
    assert(0);
    return NULL;
}



// 辅助函数：递归提取VarDec节点中的变量名
char* get_var_name(Node* node) {
    while (node != NULL) {
        if (node->num == 1 && strcmp(node->child[0]->name, "ID") == 0) {
            return node->child[0]->attr; // 找到变量名
        }
        node = node->child[0]; // 继续递归左子树
    }
    return NULL; // 理论上不会执行到此处
}
