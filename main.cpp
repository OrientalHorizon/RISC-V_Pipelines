//
// Created by iakno on 2023/6/28.
//

#include <bits/stdc++.h>
#include "parser.h"
#include "actuator.h"
#include "forwarding.h"
extern unsigned int reg[32], mem[300003], pc;
signed main() {
    // freopen("sample.data", "r", stdin);

    pc = 0u;
    parser.Init();
    while (true) {
        // cout << "reg[2] = " << std::dec << reg[2] << endl;
        // printf("%d\n", halt);
        reg[0] = 0u;
        if (stall) {
            Decode();
            EX();
            Mem();
            WB();
            --stall;
        }

        WB();
        Mem();
        EX();
        Decode();
        IF();
//        cout << decodeQueue.empty() << executeQueue.empty() << memQueue.empty() << writeReg.empty()
//        << endl;
        if (halt && memQueue.empty() && writeReg.empty()
        && decodeQueue.empty() && executeQueue.empty()) break;
        ++loop;
    }
    cout << std::dec << (reg[10] & 0xFFu) << endl;
    return 0;
}
