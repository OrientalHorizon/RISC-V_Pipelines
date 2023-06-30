//
// Created by iakno on 2023/6/28.
//

#include <bits/stdc++.h>
#include "parser.h"
#include "actuator.h"
#include "forwarding.h"
extern unsigned int reg[32], mem[300003], pc;
signed main() {
    pc = 0u;
    parser.Init();
    while (true) {
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
        if (halt && memQueue.empty() && writeReg.empty()
        && decodeQueue.empty() && executeQueue.empty()) break;
        ++loop;
    }
    cout << (reg[10] & 0xFFu) << endl;
    return 0;
}
