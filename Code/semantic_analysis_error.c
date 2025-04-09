#include "semantic_analysis_error.h"
#include "type.h"
#include <stdio.h>
#include <string.h>

void semErrOutput(unsigned int errCode, unsigned int line, char* name){
    switch(errCode){
        case NOT_DEFINE_VAR:
            printf("Error type 1 at line %d: Undefined variable \"%s\".\n", line, name);
            break;
        case(NOT_DEFINE_FUNC):
            printf("Error type 2 at line %d: Undefined function \"%s\".\n", line, name);
            break;
        case(DEFINE_VAR_MULTIPLY):
            printf("Error type 3 at line %d: Redefined variable \"%s\".\n", line, name);
            break;
        case(DEFINE_FUNC_MULTIPLY):
            printf("Error type 4 at line %d: Redefined function \"%s\".\n", line, name);
            break;
        case(NOT_MATCH_ASSIGNOP):
            printf("Error type 5 at line %d: Type mismatched for assignment.\n", line);
            break;
        case(ONLY_RIGHT_VAL):
            printf("Error type 6 at line %d: The left-hand side of an assignment must be a variable.\n", line);
            break;
        case(NOT_MATCH_OPERATOR):
            printf("Error type 7 at line %d: Type mismatched for operands.\n", line);
            break;
        case(NOT_MATCH_RETURN):
            printf("Error type 8 at line %d: Type mismatched for return.\n", line);
            break;               
        case(NOT_MATCH_FUNCPARA):
            printf("Error type 9 at line\n"); // TODO
            break;    
        case(NOT_ARR_LB):
            printf("Error type 10 at line %d: \"%s\" is not an array.\n", line, name);
            break;    
        case(NOT_FUNC_LC):
            printf("Error type 11 at line %d: \"%s\" is not a function.\n", line, name);
            break;    
        case(NOT_INT_ACCESS_ARR):
            printf("Error type 12 at line %d: \"%s\" is not a integer.\n", line, name);
            break;    
        case(NOT_STRUCT_DOT):
            printf("Error type 13 at line %d: Illegal use of \".\".\n", line);
            break;    
        case(NOT_DEFINE_FIELD):
            printf("Error type 14 at line %d: Non-existent field \"%s\".\n", line, name);
            break;    
        case(DEFINE_FIELD_MULTIPLY):
            printf("Error type 15 at line %d: Redefined field \"%s\".\n", line, name);
            break;    
        case(DEFINE_STRUCT_MULTIPLY):
            printf("Error type 16 at line %d: Duplicated name \"%s\".\n", line, name);
            break;    
        case(NOT_DEFINE_STRUCT):
            printf("Error type 17 at line %d: Undefined structure \"%s\".\n", line, name);
            break;    
        }
}

