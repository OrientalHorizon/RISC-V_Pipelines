//
// Created by iakno on 2023/6/28.
//

#ifndef INC_2_PIPELINE_PARSER_H
#define INC_2_PIPELINE_PARSER_H

#include <bits/stdc++.h>
#include "utils.h"

using std::cin;
using std::cout;
using std::endl;

extern unsigned int reg[32], mem[300003], pc;

class Parser {

public:
    void Init() {
        std::stringstream ss;
        std::string tmp;
        int addr = 0;
        while (getline(cin, tmp)) {
            // cout << tmp << endl;
            ss.clear();
            ss << tmp;
            if (tmp[0] == '@') {
                char not_used;
                ss >> not_used;
                ss >> std::hex >> addr;
            } else if (isdigit(tmp[0]) || (tmp[0] >= 'A' && tmp[0] <= 'F')) {
                unsigned int x[4];
                while (ss >> std::hex >> x[0]) {
                    mem[addr++] = x[0];
                    for (int i = 1; i <= 3; ++i) {
                        ss >> std::hex >> x[i];
                        mem[addr++] = x[i];
                    }
                }
            }
        }
    }

    unsigned int GetCommand() {
        unsigned int ret = mem[pc] | (mem[pc + 1] << 8u) | (mem[pc + 2] << 16u) | (mem[pc + 3] << 24u);
        // cout << std::dec << pc << endl;
        pc += 4;
        return ret;
    }
};

#endif //INC_2_PIPELINE_PARSER_H
