//
// Created by iakno on 2023/6/29.
//

#ifndef INC_2_PIPELINE_EXECUTOR_H
#define INC_2_PIPELINE_EXECUTOR_H

#include <bits/stdc++.h>
#include "utils.h"
#include "forwarding.h"
#include "predictor.h"
extern std::queue<std::pair<int, unsigned> > writeReg;
extern std::queue<std::pair<unsigned, std::pair<bool, unsigned> > > decodeQueue;
extern std::queue<operation> executeQueue;
extern std::queue<MemOP> memQueue;
extern std::queue<unsigned> decodeCycle;
extern std::queue<unsigned> memCycle;
extern std::queue<unsigned> writeRegCycle;
extern Predictor predictor;

class Executor {
public:
    void LUI(const operation &op) {
        // cout << std::dec << op.rd << " " << (op.imm << 12u) << endl;
        PreChangeReg(op.rd, op.imm << 12u);
        writeReg.push(std::make_pair(op.rd, op.imm << 12u));
        writeRegCycle.push(loop + 2);
    }
    void AUIPC(const operation &op) {
        unsigned val = (op.imm << 12u) + pc - 4;
        PreChangeReg(op.rd, val);
        writeReg.push(std::make_pair(op.rd, val));
        writeRegCycle.push(loop + 2);
    }
    void JAL(const operation &op) {
        PreChangeReg(op.rd, pc);
        // 强制跳转！
        writeReg.push(std::make_pair(op.rd, pc));
        pc += op.imm - 4;
        writeRegCycle.push(loop + 2);
    }
    void JALR(const operation &op) {
        PreChangeReg(op.rd, pc);
        unsigned val = op.val1;
        ALUCheckReg(op.rs1, val);
        pc = val + op.imm;
        pc >>= 1u; pc <<= 1u;
        writeReg.push(std::make_pair(op.rd, pc));
        writeRegCycle.push(loop + 2);
    }
    void BEQ(operation op) {
        // 看预测对不对
        ALUCheckReg(op.rs1, op.val1);
        ALUCheckReg(op.rs2, op.val2);
        bool realJump = (op.val1 == op.val2);
        if (realJump == op.jump) {
            if (op.jump) predictor.Jump();
            else predictor.NotJump();
            return;
        }
        else if (realJump) {
            // 应该跳，预测错误
            predictor.Jump();
            pc = op.others;
            while (!decodeQueue.empty()) {
                decodeQueue.pop();
                decodeCycle.pop();
            }
            while (!executeQueue.empty()) {
                executeQueue.pop();
            }
        }
        else {
            // 不应该跳，预测错误
            predictor.NotJump();
            pc = op.others;
            while (!decodeQueue.empty()) {
                decodeQueue.pop();
                decodeCycle.pop();
            }
            while (!executeQueue.empty()) {
                executeQueue.pop();
            }
        }
    }
    void BNE(operation op) {
        // 看预测对不对
        ALUCheckReg(op.rs1, op.val1);
        ALUCheckReg(op.rs2, op.val2);
        // Forwarding，从上一个 ALU 连到下一个 ALU，后同
        bool realJump = (op.val1 != op.val2);
        if (realJump == op.jump) {
            if (op.jump) predictor.Jump();
            else predictor.NotJump();
            return;
        }
        else if (realJump) {
            // 应该跳，预测错误
            predictor.Jump();
            pc = op.others;
            while (!decodeQueue.empty()) {
                decodeQueue.pop();
                decodeCycle.pop();
            }
            while (!executeQueue.empty()) {
                executeQueue.pop();
            }
        }
        else {
            // 不应该跳，预测错误
            predictor.NotJump();
            pc = op.others;
            while (!decodeQueue.empty()) {
                decodeQueue.pop();
                decodeCycle.pop();
            }
            while (!executeQueue.empty()) {
                executeQueue.pop();
            }
        }
    }
    void BLT(operation op) {
        // 看预测对不对
        ALUCheckReg(op.rs1, op.val1);
        ALUCheckReg(op.rs2, op.val2);
        bool realJump = ((signed)op.val1 < (signed)op.val2);
        if (realJump == op.jump) {
            if (op.jump) predictor.Jump();
            else predictor.NotJump();
            return;
        }
        else if (realJump) {
            // 应该跳，预测错误
            predictor.Jump();
            pc = op.others;
            while (!decodeQueue.empty()) {
                decodeQueue.pop();
                decodeCycle.pop();
            }
            while (!executeQueue.empty()) {
                executeQueue.pop();
            }
        }
        else {
            // 不应该跳，预测错误
            predictor.NotJump();
            pc = op.others;
            while (!decodeQueue.empty()) {
                decodeQueue.pop();
                decodeCycle.pop();
            }
            while (!executeQueue.empty()) {
                executeQueue.pop();
            }
        }
    }
    void BGE(operation op) {
        // 看预测对不对
        ALUCheckReg(op.rs1, op.val1);
        ALUCheckReg(op.rs2, op.val2);
        bool realJump = ((signed)op.val1 >= (signed)op.val2);
        if (realJump == op.jump) {
            if (op.jump) predictor.Jump();
            else predictor.NotJump();
            return;
        }
        else if (realJump) {
            // 应该跳，预测错误
            predictor.Jump();
            pc = op.others;
            while (!decodeQueue.empty()) {
                decodeQueue.pop();
                decodeCycle.pop();
            }
            while (!executeQueue.empty()) {
                executeQueue.pop();
            }
        }
        else {
            // 不应该跳，预测错误
            predictor.NotJump();
            pc = op.others;
            while (!decodeQueue.empty()) {
                decodeQueue.pop();
                decodeCycle.pop();
            }
            while (!executeQueue.empty()) {
                executeQueue.pop();
            }
        }
    }
    void BLTU(operation op) {
        // 看预测对不对
        ALUCheckReg(op.rs1, op.val1);
        ALUCheckReg(op.rs2, op.val2);
        bool realJump = (op.val1 < op.val2);
        if (realJump == op.jump) {
            if (op.jump) predictor.Jump();
            else predictor.NotJump();
            return;
        }
        else if (realJump) {
            // 应该跳，预测错误
            predictor.Jump();
            pc = op.others;
            while (!decodeQueue.empty()) {
                decodeQueue.pop();
                decodeCycle.pop();
            }
            while (!executeQueue.empty()) {
                executeQueue.pop();
            }
        }
        else {
            // 不应该跳，预测错误
            predictor.NotJump();
            pc = op.others;
            while (!decodeQueue.empty()) {
                decodeQueue.pop();
                decodeCycle.pop();
            }
            while (!executeQueue.empty()) {
                executeQueue.pop();
            }
        }
    }
    void BGEU(operation op) {
        // 看预测对不对
        ALUCheckReg(op.rs1, op.val1);
        ALUCheckReg(op.rs2, op.val2);
        bool realJump = (op.val1 >= op.val2);
        if (realJump == op.jump) {
            if (op.jump) predictor.Jump();
            else predictor.NotJump();
            return;
        }
        else if (realJump) {
            // 应该跳，预测错误
            predictor.Jump();
            pc = op.others;
            while (!decodeQueue.empty()) {
                decodeQueue.pop();
                decodeCycle.pop();
            }
            while (!executeQueue.empty()) {
                executeQueue.pop();
            }
        }
        else {
            // 不应该跳，预测错误
            predictor.NotJump();
            pc = op.others;
            while (!decodeQueue.empty()) {
                decodeQueue.pop();
                decodeCycle.pop();
            }
            while (!executeQueue.empty()) {
                executeQueue.pop();
            }
        }
    }

    void LB(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        MemOP cur(true, op.val1 + op.imm, 1, op.rd, true);
        memQueue.push(cur);
        memCycle.push(loop + 1);
    }
    void LH(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        MemOP cur(true, op.val1 + op.imm, 2, op.rd, true);
        memQueue.push(cur);
        memCycle.push(loop + 1);
    }
    void LW(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        MemOP cur(true, op.val1 + op.imm, 4, op.rd, false);
        memQueue.push(cur);
        memCycle.push(loop + 1);
    }
    void LBU(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        MemOP cur(true, op.val1 + op.imm, 1, op.rd, false);
        memQueue.push(cur);
        memCycle.push(loop + 1);
    }
    void LHU(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        MemOP cur(true, op.val1 + op.imm, 2, op.rd, false);
        memQueue.push(cur);
        memCycle.push(loop + 1);
    }

    void SB(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        ALUCheckReg(op.rs2, op.val2);
        MemOP cur(false, op.val1 + op.imm, 1, op.val2 & 0xFFu, false);
        memQueue.push(cur);
        memCycle.push(loop + 1);
    }
    void SH(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        ALUCheckReg(op.rs2, op.val2);
        MemOP cur(false, op.val1 + op.imm, 2, op.val2 & 0xFFFFu, false);
        memQueue.push(cur);
        memCycle.push(loop + 1);
    }
    void SW(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        ALUCheckReg(op.rs2, op.val2);
        // cout << std::dec << op.rs1 << " " << op.imm << endl;
        MemOP cur(false, op.val1 + op.imm, 4, op.val2, false);
        memQueue.push(cur);
        memCycle.push(loop + 1);
    }

    void ADDI(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        unsigned val = op.val1 + op.imm;
        PreChangeReg(op.rd, val);
        writeReg.push(std::make_pair(op.rd, val));
        writeRegCycle.push(loop + 2);
    }
    void SLTI(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        bool val = ((signed)op.val1 < (signed)op.imm);
        PreChangeReg(op.rd, val);
        writeReg.push(std::make_pair(op.rd, val));
        writeRegCycle.push(loop + 2);
    }
    void SLTIU(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        bool val = (op.val1 < op.imm);
        PreChangeReg(op.rd, val);
        writeReg.push(std::make_pair(op.rd, val));
        writeRegCycle.push(loop + 2);
    }
    void XORI(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        unsigned val = op.val1 ^ op.imm;
        PreChangeReg(op.rd, val);
        writeReg.push(std::make_pair(op.rd, val));
        writeRegCycle.push(loop + 2);
    }
    void ORI(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        unsigned val = op.val1 | op.imm;
        PreChangeReg(op.rd, val);
        writeReg.push(std::make_pair(op.rd, val));
        writeRegCycle.push(loop + 2);
    }
    void ANDI(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        unsigned val = op.val1 & op.imm;
        PreChangeReg(op.rd, val);
        writeReg.push(std::make_pair(op.rd, val));
        writeRegCycle.push(loop + 2);
    }
    void SLLI(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        unsigned val = op.val1 << op.rs2;
        PreChangeReg(op.rd, val);
        writeReg.push(std::make_pair(op.rd, val));
        writeRegCycle.push(loop + 2);
    }
    void SRLI(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        unsigned val = op.val1 >> op.rs2;
        PreChangeReg(op.rd, val);
        writeReg.push(std::make_pair(op.rd, val));
        writeRegCycle.push(loop + 2);
    }
    void SRAI(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        unsigned val = (signed)op.val1 >> (signed)op.rs2;
        PreChangeReg(op.rd, val);
        writeReg.push(std::make_pair(op.rd, val));
        writeRegCycle.push(loop + 2);
    }

    void ADD(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        ALUCheckReg(op.rs2, op.val2);
        unsigned val = op.val1 + op.val2;
        PreChangeReg(op.rd, val);
        writeReg.push(std::make_pair(op.rd, val));
        writeRegCycle.push(loop + 2);
    }
    void SUB(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        ALUCheckReg(op.rs2, op.val2);
        unsigned val = op.val1 - op.val2;
        PreChangeReg(op.rd, val);
        writeReg.push(std::make_pair(op.rd, val));
        writeRegCycle.push(loop + 2);
    }
    void SLL(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        ALUCheckReg(op.rs2, op.val2);
        unsigned val = op.val1 << (op.val2 & 0x1Fu);
        PreChangeReg(op.rd, val);
        writeReg.push(std::make_pair(op.rd, val));
        writeRegCycle.push(loop + 2);
    }
    void SLT(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        ALUCheckReg(op.rs2, op.val2);
        bool val = ((signed)op.val1 < (signed)op.val2);
        PreChangeReg(op.rd, val);
        writeReg.push(std::make_pair(op.rd, val));
        writeRegCycle.push(loop + 2);
    }
    void SLTU(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        ALUCheckReg(op.rs2, op.val2);
        bool val = (op.val1 < op.val2);
        PreChangeReg(op.rd, val);
        writeReg.push(std::make_pair(op.rd, val));
        writeRegCycle.push(loop + 2);
    }
    void XOR(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        ALUCheckReg(op.rs2, op.val2);
        unsigned val = op.val1 ^ op.val2;
        PreChangeReg(op.rd, val);
        writeReg.push(std::make_pair(op.rd, val));
        writeRegCycle.push(loop + 2);
    }
    void SRL(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        ALUCheckReg(op.rs2, op.val2);
        unsigned val = op.val1 >> (op.val2 & 0x1Fu);
        PreChangeReg(op.rd, val);
        writeReg.push(std::make_pair(op.rd, val));
        writeRegCycle.push(loop + 2);
    }
    void SRA(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        ALUCheckReg(op.rs2, op.val2);
        unsigned val = (signed)op.val1 >> (signed)(op.val2 & 0x1Fu);
        PreChangeReg(op.rd, val);
        writeReg.push(std::make_pair(op.rd, val));
        writeRegCycle.push(loop + 2);
    }
    void OR(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        ALUCheckReg(op.rs2, op.val2);
        unsigned val = op.val1 | op.val2;
        PreChangeReg(op.rd, val);
        writeReg.push(std::make_pair(op.rd, val));
        writeRegCycle.push(loop + 2);
    }
    void AND(operation op) {
        ALUCheckReg(op.rs1, op.val1);
        ALUCheckReg(op.rs2, op.val2);
        unsigned val = op.val1 & op.val2;
        PreChangeReg(op.rd, val);
        writeReg.push(std::make_pair(op.rd, val));
        writeRegCycle.push(loop + 2);
    }
};

#endif //INC_2_PIPELINE_EXECUTOR_H
