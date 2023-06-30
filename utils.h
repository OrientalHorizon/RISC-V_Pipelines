//
// Created by iakno on 2023/6/28.
//

#ifndef INC_2_PIPELINE_UTILS_H
#define INC_2_PIPELINE_UTILS_H

#include <bits/stdc++.h>

using std::cin;
using std::cout;
using std::endl;
unsigned int reg[32], mem[300003], pc;
bool halt;
unsigned loop;
unsigned stall;


// 实现符号位扩展
unsigned int extend(unsigned int u, unsigned int num) { // num: 最高位是几，0-based
    std::bitset<32> b(u);
    if (b[num]) {
        for (unsigned i = num + 1u; i < 32u; ++i) {
            b[i] = true;
        }
    }
    return b.to_ulong();
}

enum OP {
    LUI, AUIPC, JAL, JALR,
    BEQ, BNE, BLT, BGE, BLTU, BGEU,
    LB, LH, LW, LBU, LHU,
    SB, SH, SW,
    ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI,
    ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND
};
struct MemOP {
    bool isRead;
    unsigned addr, delta;
    unsigned val;
    bool extend;
    explicit MemOP(bool _isRead = true, unsigned _addr = 0u, unsigned _delta = 0u, unsigned _val = 0u, bool _extend = false)
    : isRead(_isRead), addr(_addr), delta(_delta), val(_val), extend(_extend) {}
};

struct operation {
    unsigned opcode, rd, funct3, rs1, rs2, funct7, imm, val1, val2;
    OP op;
    bool isBranch, jump; unsigned others;
    operation() : opcode(0u), rd(0u), funct3(0u), rs1(0u), rs2(0u), funct7(0u), imm(0u),
    val1(0u), val2(0u), op(LUI), isBranch(false), jump(false), others(0u) {}
};


#endif //INC_2_PIPELINE_UTILS_H
