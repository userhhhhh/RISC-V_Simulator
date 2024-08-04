#ifndef RISC5_INSTRUCTION_H
#define RISC5_INSTRUCTION_H

#include "memory.h"
#include <iostream>
#include "../util/config.h"
#include "Program_Counter.h"

class Instruction_unit {
public:
    InstructionType type;
    unsigned instrAddr;
    OptType opt;
    int rs1;
    int rs2;
    int rd;
    int imm;
};

class Instruction {
public:
    Instruction_unit instr;
    bool ready;
public:
    Instruction_unit instr_next;
    bool ready_next;

public:
    Memory *mem;
    ProgramCounter *programCounter;

public:

    void init(Memory *mem_in, ProgramCounter *programCounter_in);
    void flush();
    void decoder();

    void decode(uint32_t src);
    void display();


//    void process();
//    [[nodiscard]] int get_pc(unsigned Rob_pc, unsigned decoder_pc, bool Rob_flag, bool decoder_flag) const;
};

#endif //RISC5_INSTRUCTION_H
