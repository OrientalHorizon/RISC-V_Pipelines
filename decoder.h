//
// Created by iakno on 2023/6/29.
//

#ifndef INC_2_PIPELINE_DECODER_H
#define INC_2_PIPELINE_DECODER_H

#include <bits/stdc++.h>
#include "utils.h"
#include "forwarding.h"

class Decoder {
    friend void IF();
private:
    unsigned BTypeGetOffset(unsigned cmd) {
        std::bitset<32> tmp_bs(cmd), imm(0);
        for (int i = 1; i <= 4; ++i) {
            int j = i + 7;
            imm[i] = tmp_bs[j];
        }
        imm[11] = tmp_bs[7];
        for (int i = 5; i <= 10; ++i) {
            int j = i + 20;
            imm[i] = tmp_bs[j];
        }
        imm[12] = tmp_bs[31];
        return extend(imm.to_ulong(), 12);
    }
    unsigned STypeGetOffset(unsigned cmd) {
        std::bitset<32> tmp_bs(cmd), imm(0);
        for (int i = 0; i <= 4; ++i) {
            int j = i + 7;
            imm[i] = tmp_bs[j];
        }
        for (int i = 5; i <= 11; ++i) {
            int j = i + 20;
            imm[i] = tmp_bs[j];
        }
        return extend(imm.to_ulong(), 11);
    }
    operation UTypeDecode(unsigned cmd) {
        operation ret;
        ret.opcode = cmd & 0x7Fu;
        ret.rd = (cmd >> 7u) & 0x1Fu;
        ret.imm = cmd >> 12u;
        return ret;
    }
public:
    operation LUIDecode(unsigned cmd) {
        operation ret = UTypeDecode(cmd);
        ret.op = LUI;
        return ret;
    }
    operation AUIPCDecode(unsigned cmd) {
        operation ret = UTypeDecode(cmd);
        ret.op = AUIPC;
        return ret;
    }
    operation JALDecode(unsigned cmd) {
        operation ret; ret.op = JAL, ret.opcode = cmd & 0x7fu;
        std::bitset<32> tmp_bs(cmd), imm(0u);
        for (int i = 1; i <= 10; ++i) {
            imm[i] = tmp_bs[i + 20];
        }
        imm[11] = tmp_bs[20];
        for (int i = 12; i <= 19; ++i) {
            imm[i] = tmp_bs[i];
        }
        imm[20] = tmp_bs[31];
        unsigned tmp = extend(imm.to_ulong(), 20);
        ret.imm = tmp, ret.rd = (cmd >> 7u) & 0x1fu;
        return ret;
    }
    operation JALRDecode(unsigned cmd) {
        operation ret; ret.op = JALR, ret.opcode = cmd & 0x7fu;
        ret.rd = (cmd >> 7u) & 0x1Fu;
        ret.rs1 = (cmd >> 15u) & 0x1Fu;
        ret.val1 = GetReg(ret.rs1);
        ret.imm = (cmd >> 20u) & 0xFFFu;
        ret.imm = extend(ret.imm, 11);
        return ret;
    }
    operation BTypeDecode(unsigned cmd, std::pair<bool, unsigned> branch) {
        operation ret; ret.opcode = cmd & 0x7fu;
        ret.rs1 = (cmd >> 15u) & 0x1Fu;
        ret.rs2 = (cmd >> 20u) & 0x1Fu;
        ret.val1 = GetReg(ret.rs1), ret.val2 = GetReg(ret.rs2);// Unsigned!!
        ret.imm = BTypeGetOffset(cmd);
        ret.funct3 = (cmd >> 12u) & 0x7u;
        ret.isBranch = true, ret.jump = branch.first, ret.others = branch.second;
        switch (ret.funct3) {
            case 0x0u: ret.op = BEQ; break;
            case 0x1u: ret.op = BNE; break;
            case 0x4u: ret.op = BLT; break;
            case 0x5u: ret.op = BGE; break;
            case 0x6u: ret.op = BLTU; break;
            case 0x7u: ret.op = BGEU; break;
        }
        return ret;
    }
    operation LTypeDecode(unsigned cmd) {
        operation ret; ret.opcode = cmd & 0x7fu;
        ret.rd = (cmd >> 7u) & 0x1Fu;
        ret.rs1 = (cmd >> 15u) & 0x1Fu;
        ret.val1 = GetReg(ret.rs1);
        ret.imm = (cmd >> 20u) & 0xFFFu;
        ret.imm = extend(ret.imm, 11);
        ret.funct3 = (cmd >> 12u) & 0x7u;
        switch (ret.funct3) {
            case 0x0u: ret.op = LB; break;
            case 0x1u: ret.op = LH; break;
            case 0x2u: ret.op = LW; break;
            case 0x4u: ret.op = LBU; break;
            case 0x5u: ret.op = LHU; break;
        }
        return ret;
    }
    operation STypeDecode(unsigned cmd) {
        // cout << "reg[2] = " << std::dec << reg[2] << endl;
        operation ret; ret.opcode = cmd & 0x7fu;
        ret.rs1 = (cmd >> 15u) & 0x1Fu;
        ret.rs2 = (cmd >> 20u) & 0x1Fu;
        ret.val1 = GetReg(ret.rs1), ret.val2 = GetReg(ret.rs2);
        ret.imm = STypeGetOffset(cmd);
        ret.funct3 = (cmd >> 12u) & 0x7u;
        switch (ret.funct3) {
            case 0x0u: ret.op = SB; break;
            case 0x1u: ret.op = SH; break;
            case 0x2u: ret.op = SW; break;
        }
        return ret;
    }
    operation ITypeDecode(unsigned cmd) {
        operation ret; ret.opcode = cmd & 0x7fu;
        ret.rd = (cmd >> 7u) & 0x1Fu;
        ret.rs1 = (cmd >> 15u) & 0x1Fu;
        ret.val1 = GetReg(ret.rs1);
        ret.imm = (cmd >> 20u) & 0xFFFu;
        ret.imm = extend(ret.imm, 11);
        ret.funct3 = (cmd >> 12u) & 0x7u;
        switch (ret.funct3) {
            case 0x0u: ret.op = ADDI; break;
            case 0x2u: ret.op = SLTI; break;
            case 0x3u: ret.op = SLTIU; break;
            case 0x4u: ret.op = XORI; break;
            case 0x6u: ret.op = ORI; break;
            case 0x7u: ret.op = ANDI; break;
            case 0x1u: {
                ret.rs2 = (cmd >> 20u) & 0x1Fu;
                ret.op = SLLI;
                break;
            }
            case 0x5u: {
                ret.funct7 = (cmd >> 25u) & 0x7Fu;
                if (ret.funct7 == 0x0u) ret.op = SRLI;
                else ret.op = SRAI;
                ret.rs2 = (cmd >> 20u) & 0x1Fu;
                break;
            }
        }
        return ret;
    }
    operation RTypeDecode(unsigned cmd) {
        operation ret; ret.opcode = cmd & 0x7fu;
        ret.rd = (cmd >> 7u) & 0x1Fu;
        ret.rs1 = (cmd >> 15u) & 0x1Fu;
        ret.rs2 = (cmd >> 20u) & 0x1Fu;
        ret.val1 = GetReg(ret.rs1), ret.val2 = GetReg(ret.rs2);
        ret.funct3 = (cmd >> 12u) & 0x7u;
        ret.funct7 = (cmd >> 25u) & 0x7Fu;
        switch (ret.funct3) {
            case 0x0u: {
                if (ret.funct7 == 0x0u) ret.op = ADD;
                else ret.op = SUB;
                break;
            }
            case 0x1u: ret.op = SLL; break;
            case 0x2u: ret.op = SLT; break;
            case 0x3u: ret.op = SLTU; break;
            case 0x4u: ret.op = XOR; break;
            case 0x5u: {
                if (ret.funct7 == 0x0u) ret.op = SRL;
                else ret.op = SRA;
                break;
            }
            case 0x6u: ret.op = OR; break;
            case 0x7u: ret.op = AND; break;
        }
        return ret;
    }
};

#endif //INC_2_PIPELINE_DECODER_H
