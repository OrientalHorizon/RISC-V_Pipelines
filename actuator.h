//
// Created by iakno on 2023/6/28.
//

#ifndef INC_2_PIPELINE_ACTUATOR_H
#define INC_2_PIPELINE_ACTUATOR_H

#include <bits/stdc++.h>
#include "utils.h"
#include "parser.h"
#include "decoder.h"
#include "executor.h"
#include "predictor.h"
#include "forwarding.h"

Parser parser;
Predictor predictor;
Decoder decoder;
Executor executor;
std::queue<std::pair<unsigned, std::pair<bool, unsigned> > > decodeQueue;
std::queue<unsigned> decodeCycle;
std::queue<operation> executeQueue;
std::queue<unsigned> executeCycle;
std::queue<MemOP> memQueue;
std::queue<unsigned> memCycle;
std::queue<std::pair<int, unsigned> > writeReg;
std::queue<unsigned> writeRegCycle;
// 读 (true) 还是写 (false)，第二个元素是内存地址，第三个元素是存到哪里（读）或者是值（写）

extern unsigned stall;
void AddStall(unsigned delta) {
    stall += delta;
}

void InstructionFetch() { // Finished writing IF
    if (_end) return;
    unsigned int cmd = parser.GetCommand();
    // cout << std::setfill('0') <<  std::setw(8) << std::hex << std::uppercase << cmd << endl;
    if (cmd == 0x0ff00513u) {
        _end = true;
        return;
    }

    unsigned opcode = cmd & 0x7Fu;
    if (opcode == 0b1100011) {
        // 开始分支预测
        bool predict = predictor.Predict();
        unsigned offset = decoder.BTypeGetOffset(cmd);
        if (predict) {
            // TODO: get offset
            decodeQueue.push(std::make_pair(cmd, std::make_pair(true, pc)));
            pc += offset - 4;
            decodeCycle.push(loop + 1);
        }
        else {
            decodeQueue.push(std::make_pair(cmd, std::make_pair(false, pc + offset - 4)));
            decodeCycle.push(loop + 1);
        }
    }
    else if (opcode == 0b0000011) {
        // Load: Stall three cycles
        decodeQueue.push(std::make_pair(cmd, std::make_pair(false, 0u)));
        decodeCycle.push(loop + 1);
        AddStall(3u);
    }
    else if (opcode == 0b1101111) {
        // JAL
        decodeQueue.push(std::make_pair(cmd, std::make_pair(true, pc)));
        decodeCycle.push(loop + 1);
        pc += decoder.JALGetOffset(cmd) - 4;
    }
    else if (opcode == 0b1100111) {
        // JALR
        decodeQueue.push(std::make_pair(cmd, std::make_pair(false, 0u)));
        decodeCycle.push(loop + 1);
        AddStall(2u);
    }
    else {
        decodeQueue.push(std::make_pair(cmd, std::make_pair(false, 0u)));
        decodeCycle.push(loop + 1);
    }
}

void Writeback() { // Finished writing WB
    if (writeReg.empty()) return;
    unsigned expected = writeRegCycle.front();
    if (expected > loop) {
        // 可能出现“后来居上”的情况，后面加进来的操作先执行
        std::pair<int, unsigned> nxt = writeReg.front();
        writeReg.pop();
        writeRegCycle.pop();
        writeReg.push(nxt);
        writeRegCycle.push(expected);
    }
    std::pair<int, unsigned> cur = writeReg.front();
    writeReg.pop();
    writeRegCycle.pop();
    reg[cur.first] = cur.second;
    reg[0] = 0u;
}

void Decode() {
    if (decodeQueue.empty()) return;
    unsigned expected = decodeCycle.front();
    if (expected > loop) return;
    unsigned int cmd = decodeQueue.front().first;
    std::pair<bool, unsigned> branch = decodeQueue.front().second;
    decodeQueue.pop();
    decodeCycle.pop();
    unsigned opcode = cmd & 0x7Fu;
    operation opt;
    switch (opcode) {
        case 0b0110111: {
            // LUI
            opt = decoder.LUIDecode(cmd);
            break;
        }
        case 0b0010111: {
            // AUIPC
            opt = decoder.AUIPCDecode(cmd);
            break;
        }
        case 0b1101111: {
            // JAL
            opt = decoder.JALDecode(cmd, branch.second);
            break;
        }
        case 0b1100111: {
            // JALR
            opt = decoder.JALRDecode(cmd);
            break;
        }
        case 0b1100011: {
            // B-type
            opt = decoder.BTypeDecode(cmd, branch);
            break;
        }
        case 0b0000011: {
            // L-type
            opt = decoder.LTypeDecode(cmd);
            break;
        }
        case 0b0100011: {
            // S-type
            opt = decoder.STypeDecode(cmd);
            break;
        }
        case 0b0010011: {
            // I-type
            opt = decoder.ITypeDecode(cmd);
            break;
        }
        case 0b0110011: {
            // R-type
            opt = decoder.RTypeDecode(cmd);
            break;
        }
    }
    executeQueue.push(opt);
    executeCycle.push(loop + 1);
}

void ALU() {
    if (executeQueue.empty()) return;
    unsigned expected = executeCycle.front();
    if (expected > loop) return;
    operation opt = executeQueue.front();
    executeQueue.pop();
    executeCycle.pop();
    switch (opt.op) {
        case LUI:
            executor.LUI(opt);
            break;
        case AUIPC:
            executor.AUIPC(opt);
            break;
        case JAL:
            executor.JAL(opt);
            break;
        case JALR:
            executor.JALR(opt);
            break;
        case BEQ:
            executor.BEQ(opt);
            break;
        case BNE:
            executor.BNE(opt);
            break;
        case BLT:
            executor.BLT(opt);
            break;
        case BGE:
            executor.BGE(opt);
            break;
        case BLTU:
            executor.BLTU(opt);
            break;
        case BGEU:
            executor.BGEU(opt);
            break;
        case LB:
            executor.LB(opt);
            break;
        case LH:
            executor.LH(opt);
            break;
        case LW:
            executor.LW(opt);
            break;
        case LBU:
            executor.LBU(opt);
            break;
        case LHU:
            executor.LHU(opt);
            break;
        case SB:
            executor.SB(opt);
            break;
        case SH:
            executor.SH(opt);
            break;
        case SW:
            executor.SW(opt);
            break;
        case ADDI:
            executor.ADDI(opt);
            break;
        case SLTI:
            executor.SLTI(opt);
            break;
        case SLTIU:
            executor.SLTIU(opt);
            break;
        case XORI:
            executor.XORI(opt);
            break;
        case ORI:
            executor.ORI(opt);
            break;
        case ANDI:
            executor.ANDI(opt);
            break;
        case SLLI:
            executor.SLLI(opt);
            break;
        case SRLI:
            executor.SRLI(opt);
            break;
        case SRAI:
            executor.SRAI(opt);
            break;
        case ADD:
            executor.ADD(opt);
            break;
        case SUB:
            executor.SUB(opt);
            break;
        case SLL:
            executor.SLL(opt);
            break;
        case SLT:
            executor.SLT(opt);
            break;
        case SLTU:
            executor.SLTU(opt);
            break;
        case XOR:
            executor.XOR(opt);
            break;
        case SRL:
            executor.SRL(opt);
            break;
        case SRA:
            executor.SRA(opt);
            break;
        case OR:
            executor.OR(opt);
            break;
        case AND:
            executor.AND(opt);
            break;
    }
}

void Mem() {
    if (memQueue.empty()) return;
    unsigned expected = memCycle.front();
    if (expected > loop) return;
    MemOP cur = memQueue.front();
    memQueue.pop();
    memCycle.pop();
    if (cur.isRead) {
        unsigned val;
        switch (cur.delta) {
            case 1: {
                val = mem[cur.addr];
                if (cur.extend) val = extend(val, 7);
                break;
            }
            case 2: {
                val = mem[cur.addr] | (mem[cur.addr + 1] << 8);
                if (cur.extend) val = extend(val, 15);
                break;
            }
            case 4: {
                val = mem[cur.addr] | (mem[cur.addr + 1] << 8)
                        | (mem[cur.addr + 2] << 16) | (mem[cur.addr + 3] << 24);
                break;
            }
        }
        PreChangeReg(cur.val, val);
        writeReg.push(std::make_pair(cur.val, val));
        writeRegCycle.push(loop + 1);
    }
    else {
        switch (cur.delta) {
            case 1: {
                mem[cur.addr] = cur.val & 0xFFu;
                break;
            }
            case 2: {
                mem[cur.addr] = cur.val & 0xFFu;
                mem[cur.addr + 1] = (cur.val >> 8) & 0xFFu;
                break;
            }
            case 4: {
                mem[cur.addr] = cur.val & 0xFFu;
                mem[cur.addr + 1] = (cur.val >> 8) & 0xFFu;
                mem[cur.addr + 2] = (cur.val >> 16) & 0xFFu;
                mem[cur.addr + 3] = (cur.val >> 24) & 0xFFu;
                break;
            }
        }
    }
}

#endif //INC_2_PIPELINE_ACTUATOR_H
