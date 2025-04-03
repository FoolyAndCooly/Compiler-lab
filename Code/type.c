#include <stdlib.h>
#include <string.h>
#include <type.h>

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

Type create_struct(char* name) {
    Type type = (Type)malloc(sizeof(Type_));
    type->kind = STRUCTURE;
    type->u.fieldlist = (FieldList)malloc(FieldList_);
    strcpy(type->u.fieldlist->name, name);
    return type;
}

Type create_func(char* name) {
    Type type = (Type)malloc(sizeof(Type_));
    type->kind = FUNCTION;
    type->u.fieldlist = (FieldList)malloc(FieldList_);
    strcpy(type->u.fieldlist->name, name);
    return type;
}

void append_fieldlist(Type fieldlist, char* name, Type type) {
    FieldList new_field = (FieldList)malloc(FieldList_);
    strcpy(new_field->name, name);
    new_field->type = type;
    new_field->tail = fieldlist->u.fieldlist->type->u.fieldlist;
    fieldlist->u.fieldlist->type->u.fieldlist = new_field;
}

