#define TYPE_INT 0
#define TYPE_FLOAT 1

typedef struct Type_* Type;
typedef struct FieldList_* FieldList;

struct Type_ {
    enum {BASIC, ARRAY, STRUCTURE, FUNCTION} kind;
    union {
        int basic; // 0: int, 1: float
        struct (Type elem; int size;} array;
        FieldList fieldlist;
    } u;
};

struct FieldList_ {
    char* name;
    Type type;
    FieldList tail;
};

Type create_basic(int basic);
Type create_array(Type elem, int size);
Type create_struct(char* name);
Type create_func(char* name);
void append_fieldlist(Type fieldlist, char* name, Type type);

