#include "temu.h"
#include <stdlib.h>
#include <string.h>

CPU_state cpu;

const char *regfile[] = {"$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra"};

CP0_state cp0;

const char *regfile_cp0[] = {"$0", "$1", "$2", "$3", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15", "$16", "$17", "$18", "$19", "$20", "$21", "$22", "$23", "$24", "$25", "$26", "$27", "$28", "$29", "$30", "$31"};

void display_reg() {
        int i;
        for(i = 0; i < 32; i ++) {
                printf("%s\t\t0x%08x\t\t%d\n", regfile[i], cpu.gpr[i]._32, cpu.gpr[i]._32);
        }

        printf("%s\t\t0x%08x\t\t%d\n", "$pc", cpu.pc, cpu.pc);
}

uint32_t reg_val_w(const char *name) {
    for (int i = 0; i < 32; ++i) {
        if (strcmp(name, regfile[i]) == 0) {
            return reg_w(i);
        }
    }
    return 0;
}

void SignalException(uint32_t badaddress,uint32_t type){    
    if ((cp0.status & 0x00000002) == 2){ // 已处在异常处理状态，判断是否是中断
        if(type == 0x00){
            return;
        }else{
            cp0.cause = cp0.cause & 0xffffff00;
            cp0.cause = cp0.cause | (type << 1);
            cp0.badvaddr = badaddress;
        }
    }else {
        cp0.cause = cp0.cause & 0xffffff00;
        cp0.cause = cp0.cause | (type << 1);
        cp0.badvaddr = badaddress;

        uint32_t    last_pc  = cpu.pc-4;

        uint32_t op =  last_pc >> 26;
        uint32_t func = last_pc &  0x0000003F;
        if (op == 0x04 || op == 0x05 || op == 0x07 || op == 0x06 || op == 0x02 || op == 0x03 || op == 0x01 || (op==0 && func == 0x08) || (op==0 && func==0x09)){
            cp0.epc = last_pc;
            cp0.cause = cp0.cause | 0x80000000;
        }else{
            cp0.epc = cpu.pc;
            cp0.cause = cp0.cause & 0x7fffffff;
        }

        cp0.status = cp0.status | 0x00000002;
    }

    printf("now there is an exception!\n");
    return;
}

uint32_t reg_val_w_cp0(const char *name) {
    for (int i = 0; i < 32; ++i) {
        if (strcmp(name, regfile_cp0[i]) == 0) {
            return reg_w_cp0(i);
        }
    }
    return 0;
}

