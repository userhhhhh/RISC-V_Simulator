#ifndef RISC5_ALU_H
#define RISC5_ALU_H

#include <iostream>
#include "../util/config.h"

class ALU {
public:
    bool ready;
    OptType opt;
    uint32_t rs1, rs2;
    uint32_t Rob_id;
private:
    bool ready_next;
    OptType opt_next;
    uint32_t rs1_next, rs2_next;
    uint32_t Rob_id_next;
public:
    void init();
    void flush();
    void step();
    void add(OptType opt, int rs1, int rs2, uint32_t Rob_id);
    int calculate(OptType opt, uint32_t rs1, uint32_t rs2, uint32_t Rob_id);
    void clear();
    void print();
    ALU& operator= (const ALU& alu);

};

#endif //RISC5_ALU_H
