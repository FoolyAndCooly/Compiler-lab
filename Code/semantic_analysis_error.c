#include "semantic_analysis_error.h"
#include "type.h"
#include <stdio.h>

void semErrOutput(unsigned int errCode){
    switch(errCode){
        case NOT_DEFINE_VAR:
            printf("Error type 1 at line\n");
            break;
        case(NOT_DEFINE_FUNC):
            printf("Error type 2 at line\n");
            break;
        case(DEFINE_VAR_MULTIPLY):
            printf("Error type 3 at line\n");
            break;
        case(DEFINE_FUNC_MULTIPLY):
            printf("Error type 4 at line\n");
            break;
        case(NOT_MATCH_ASSIGNOP):
            printf("Error type 5 at line\n");
            break;
        case(ONLY_RIGHT_VAL):
            printf("Error type 6 at line\n");
            break;
        case(NOT_MATCH_OPERATOR):
            printf("Error type 7 at line\n");
            break;
        case(NOT_MATCH_RETURN):
            printf("Error type 8 at line\n");
            break;               
        case(NOT_MATCH_FUNCPARA):
            printf("Error type 9 at line\n");
            break;    
        case(NOT_ARR_LB):
            printf("Error type 10 at line\n");
            break;    
        case(NOT_FUNC_LC):
            printf("Error type 11 at line\n");
            break;    
        case(NOT_INT_ACCESS_ARR):
            printf("Error type 12 at line\n");
            break;    
        case(NOT_STRUCT_DOT):
            printf("Error type 13 at line\n");
            break;    
        case(NOT_DEFINE_FIELD):
            printf("Error type 14 at line\n");
            break;    
        case(DEFINE_FIELD_MULTIPLY):
            printf("Error type 15 at line\n");
            break;    
        case(DEFINE_STRUCT_MULTIPLY):
            printf("Error type 16 at line\n");
            break;    
        case(NOT_DEFINE_STRUCT):
            printf("Error type 17 at line\n");
            break;    
        }
}


FieldList find_field_member(Type struct_type, const char* field_name) {
    FieldList current = struct_type->u.fieldlist;
    while (current != NULL) {
        if (strcmp(current->name, field_name) == 0) {
            return current;
        }
        current = current->tail;
    }
    return 0;
}