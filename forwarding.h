//
// Created by iakno on 2023/6/28.
//

#ifndef INC_2_PIPELINE_FORWARDING_H
#define INC_2_PIPELINE_FORWARDING_H

#include <bits/stdc++.h>
#include "utils.h"

extern unsigned int reg[32], mem[300003], pc;
unsigned curRegNum, curRegVal;

unsigned GetReg(unsigned pos) {
    assert(pos < 32 && pos >= 0);
    if (pos == curRegNum) return curRegVal;
    return reg[pos];
}
void ALUCheckReg(unsigned pos, unsigned &val) {
    assert(pos < 32 && pos >= 0);
    if (pos == curRegNum) val = curRegVal;
}
void PreChangeReg(unsigned pos, unsigned val) {
    assert(pos < 32 && pos >= 0);
    if (pos == 0) {
        reg[0] = 0u;
        curRegNum = 0, curRegVal = 0u;
    }
    else {
        curRegNum = pos, curRegVal = val;
    }
}

//unsigned curMemAddrL, curMemAddrR, curMemVal;
//unsigned GetMem(unsigned pos) {
//    assert(pos < 300003 && pos >= 0);
//    if (pos >= curMemAddrL && pos <= curMemAddrR) {
//        return curMemVal;
//    }
//    return mem[pos];
//}
//void PreChangeMem(int l, int r, unsigned val) {
//    curMemAddrL = l, curMemAddrR = r, curMemVal = val;
//}

#endif //INC_2_PIPELINE_FORWARDING_H
