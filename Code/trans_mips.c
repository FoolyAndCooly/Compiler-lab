#include "trans_mips.h"

void init_regs(){
    strcpy(regs[0].name,"$zero");
    strcpy(regs[1].name,"$at");
    strcpy(regs[2].name,"$v0");
    strcpy(regs[3].name,"$v1");
    strcpy(regs[4].name,"$a0");
    strcpy(regs[5].name,"$a1");
    strcpy(regs[6].name,"$a2");
    strcpy(regs[7].name,"$a3");
    strcpy(regs[8].name,"$t0");
    strcpy(regs[9].name,"$t1");
    strcpy(regs[10].name,"$t2");
    strcpy(regs[11].name,"$t3");
    strcpy(regs[12].name,"$t4");
    strcpy(regs[13].name,"$t5");
    strcpy(regs[14].name,"$t6");
    strcpy(regs[15].name,"$t7");
    strcpy(regs[16].name,"$s0");
    strcpy(regs[17].name,"$s1");
    strcpy(regs[18].name,"$s2");
    strcpy(regs[19].name,"$s3");
    strcpy(regs[20].name,"$s4");
    strcpy(regs[21].name,"$s5");
    strcpy(regs[22].name,"$s6");
    strcpy(regs[23].name,"$s7");
    strcpy(regs[24].name,"$t8");
    strcpy(regs[25].name,"$t9");
    strcpy(regs[26].name,"$k0");
    strcpy(regs[27].name,"$k1");
    strcpy(regs[28].name,"$gp");
    strcpy(regs[29].name,"$sp");
    strcpy(regs[30].name,"$fp");
    strcpy(regs[31].name,"$ra");
    for(int i=0;i<MAX_REG_SIZE;i++) {
        regs[i].state=REGISTER_FREE;
        regs[i].value = NULL;
    }
}

unsigned int get_free_reg(){
    /*
        Need to be finished.
    */
    return 0;
}


void Trans_MIPS(Codelist codelist, char* filename){
    Code* code = codelist.head->next;
    FILE *file = fopen(filename, "w");
    while(code) {
        Trans_MIPS_SingleLineCode(code);
        code = code->next;
    }
    fclose(file);
}


void Trans_MIPS_SingleLineCode(Code* code){
    assert(code);
    switch(code->codetype){
        case CODE_READ:
            Trans_MIPS_read(code);
            break;
        case CODE_WRITE:
            Trans_MIPS_write(code);
            break;
        default:
            printf("ERROR: Code type unknown\n");
            assert(0);
    }
}


void Trans_MIPS_read(Code* code){}
void Trans_MIPS_write(Code* code){}
void Trans_MIPS_label(Code* code){}
void Trans_MIPS_add(Code* code){}
void Trans_MIPS_sub(Code* code){}
void Trans_MIPS_mul(Code* code){}
void Trans_MIPS_div(Code* code){}
void Trans_MIPS_assignop(Code* code){}
void Trans_MIPS_arg_call(Code* code){}
void Trans_MIPS_goto(Code* code){}
void Trans_MIPS_return(Code* code){}
void Trans_MIPS_ifgoto(Code* code){}
void Trans_MIPS_function(Code* code){}