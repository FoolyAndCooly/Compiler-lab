#ifndef _TRANS_MIPS_H_
#define _TRANS_MIPS_H_

#include "lib.h"
#include "trans.h"
#define MAX_REG_SIZE 32

typedef enum {REGISTER_FREE, REGISTER_OCCUPIED} RegState;
typedef struct Reg {
    char name[20];
    RegState state;
    char* value;
} Reg;
Reg regs[MAX_REG_SIZE];

void init_regs();
unsigned int get_free_reg();

void Trans_MIPS(Codelist codelist, char* filename);
void Trans_MIPS_SingleLineCode(Code* code);

void Trans_MIPS_read(Code* code);
void Trans_MIPS_write(Code* code);
void Trans_MIPS_label(Code* code);
void Trans_MIPS_add(Code* code);
void Trans_MIPS_sub(Code* code);
void Trans_MIPS_mul(Code* code);
void Trans_MIPS_div(Code* code);
void Trans_MIPS_assignop(Code* code);
void Trans_MIPS_arg_call(Code* code);
void Trans_MIPS_goto(Code* code);
void Trans_MIPS_return(Code* code);
void Trans_MIPS_ifgoto(Code* code);
void Trans_MIPS_function(Code* code);

#endif