#ifndef RISC5_INSTRUCTION_H
#define RISC5_INSTRUCTION_H

#include "memory.h"
#include <iostream>
#include "../util/config.h"

class Instruction_unit {
public:
    InstructionType type;
    OptType opt;
    int rs1;
    int rs2;
    int rd;
    int imm;
};

class Instruction {
public:
    unsigned pc;
    unsigned instrAddr;
    Instruction_unit instr;
    bool ready;
private:
    unsigned pc_next;
    unsigned instrAddr_next;
    Instruction_unit instr_next;
    bool ready_next;

public:
    Memory *mem;

public:
    void init(Memory *mem_in);
    void flush();
    void step();
    void decoder(Memory &mem, unsigned Rob_pc);
    void decode(Instruction_unit &instr, int src);
};

#endif //RISC5_INSTRUCTION_H
