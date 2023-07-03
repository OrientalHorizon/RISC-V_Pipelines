//
// Created by iakno on 2023/6/28.
//

#include <bits/stdc++.h>
#include "parser.h"
#include "actuator.h"

signed main() {
    // freopen("tak.data", "r", stdin);
    pc = 0u;
    parser.Init();
    while (true) {
        reg[0] = 0u;
        if (stall) {
            Decode();
            ALU();
            Writeback();
            Mem();
            --stall;
            ++loop;
            continue;
        }
        Mem();
        Decode();
        Writeback();
        InstructionFetch();
        ALU();
//        assert(memQueue.size() - memCycle.size() == 0);
//        assert(writeReg.size() - writeRegCycle.size() == 0);
//        assert(decodeQueue.size() - decodeCycle.size() == 0);
//        assert(executeQueue.size() - executeCycle.size() == 0);
        // cout << decodeQueue.size() << " " << executeQueue.size() << " " << memQueue.size() << " " << writeReg.size() << endl;
        // cout << memQueue.size() - memCycle.size() << " " << writeReg.size() - writeRegCycle.size() << endl;
        // cout << decodeQueue.size() - decodeCycle.size() << " " << executeQueue.size() - executeCycle.size() << endl;
        if (_end && memQueue.empty() && writeReg.empty()
        && decodeQueue.empty() && executeQueue.empty()) break;
        ++loop;
        // cout << std::dec << (reg[10] & 0xFFu) << endl;
    }
    // cout << loop << endl;
    cout << std::dec << (reg[10] & 0xFFu) << endl;
    // printf("%.1lf\%\n", predictor.GetAccuracy() * 100);
    return 0;
}
