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
    void decoder(unsigned Rob_pc, unsigned decoder_pc, bool Rob_flag, bool decoder_flag);
    void decode(int src);
    [[nodiscard]] int get_pc(unsigned Rob_pc, unsigned decoder_pc, bool Rob_flag, bool decoder_flag) const;
};

#endif //RISC5_INSTRUCTION_H
