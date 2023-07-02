//
// Created by iakno on 2023/6/28.
//

#include <bits/stdc++.h>
#include "parser.h"
#include "actuator.h"

signed main() {
//    freopen("array_test1.data", "r", stdin);
//    freopen("array_test1.out", "w", stdout);
    pc = 0u;
    parser.Init();
    while (true) {
        // cout << "reg[2] = " << std::dec << reg[2] << endl;
        // printf("%d\n", halt);
        reg[0] = 0u;
        if (stall) {
            Writeback();
            Mem();
            ALU();
            Decode();
            --stall;
            ++loop;
            continue;
        }
        Writeback();
        Mem();
        ALU();
        Decode();
        InstructionFetch();
        assert(memQueue.size() - memCycle.size() == 0);
        assert(writeReg.size() - writeRegCycle.size() == 0);
        assert(decodeQueue.size() - decodeCycle.size() == 0);
        assert(executeQueue.size() - executeCycle.size() == 0);
        // cout << memQueue.size() - memCycle.size() << " " << writeReg.size() - writeRegCycle.size() << endl;
        // cout << decodeQueue.size() - decodeCycle.size() << " " << executeQueue.size() - executeCycle.size() << endl;
        if (_end && memQueue.empty() && writeReg.empty()
        && decodeQueue.empty() && executeQueue.empty()) break;
        ++loop;
        // cout << std::dec << (reg[10] & 0xFFu) << endl;
    }
    // cout << loop << endl;
    cout << std::dec << (reg[10] & 0xFFu) << endl;
    // printf("%lf\n", predictor.GetAccuracy());
    return 0;
}
