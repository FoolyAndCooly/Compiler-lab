#include <stdlib.h>
#include <string.h>
#include "type.h"

Type create_basic(int basic) {
    Type type = (Type)malloc(sizeof(Type_));
    type->kind = BASIC;
    type->u.basic = basic;
    return type;
}

Type create_array(Type elem, int size) {
    Type type = (Type)malloc(sizeof(Type_));
    type->kind = ARRAY;
    type->u.array.elem = elem;
    type->u.array.size = size;
    return type;
}

Type create_struct() {
    Type type = (Type)malloc(sizeof(Type_));
    type->kind = STRUCTURE;
    type->u.fieldlist = NULL;
    return type;
}

// the first field is return value 
Type create_func() {
    Type type = (Type)malloc(sizeof(Type_));
    type->kind = FUNCTION;
    type->u.fieldlist = NULL;
    return type;
}

FieldList find_field_member(Type struct_type, char* name) {
    FieldList cur = struct_type->u.fieldlist;
    while (cur != NULL) {
        if (strcmp(name, cur->name) == 0) {
            return cur;
	}
	cur = cur->tail;
    }
    return NULL;
}

void append_fieldlist(Type fieldlist, char* name, Type type) {
    FieldList new_field = (FieldList)malloc(sizeof(FieldList_));
    new_field->name = strdup(name);
    new_field->type = type;
    new_field->tail = fieldlist->u.fieldlist;
    fieldlist->u.fieldlist = new_field;
}

