//
// Created by iakno on 2023/6/28.
//

#include <bits/stdc++.h>
#include "parser.h"
#include "actuator.h"

signed main() {
    // freopen("multiarray.data", "r", stdin);
    // freopen("array_test1.out", "w", stdout);
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
        }
        Writeback();
        Mem();
        ALU();
        Decode();
        InstructionFetch();

        if (_end && memQueue.empty() && writeReg.empty()
        && decodeQueue.empty() && executeQueue.empty()) break;
        ++loop;
        // cout << std::dec << (reg[10] & 0xFFu) << endl;
    }
    cout << std::dec << (reg[10] & 0xFFu) << endl;
    // printf("%lf\n", predictor.GetAccuracy());
    return 0;
}
