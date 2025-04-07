#ifndef TYPE_H
#define TYPE_H

#define TYPE_INT 0
#define TYPE_FLOAT 1

typedef struct Type_* Type;
typedef struct FieldList_* FieldList;

typedef struct Type_ {
    union {
        int basic; // 0: int, 1: float
        struct {Type elem; int size;} array;
        FieldList fieldlist;
    } u;
}Type_;

typedef struct FieldList_ {
    char* name;
    Type type;
    FieldList tail;
} FieldList_;

Type create_basic(int basic);
Type create_array(Type elem, int size);
Type create_struct(char* name);
Type create_func(char* name);
void append_fieldlist(Type fieldlist, char* name, Type type);

#endif