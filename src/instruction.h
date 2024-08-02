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
    // 执行位置在 0处的指令
    //   0 0      0 0      4 0
    //        =>       =>
    //   0 0      4 0      4 0
    // 执行位置在 4处的指令
    //   4 0      4 0      8 4
    //        =>       =>
    //   4 0      8 4      8 4
    // decoder看到的是第二种状态，认为 instruction一定在 decoder前面

    friend class Naive_Simulator;

public:
    unsigned pc;
    unsigned instrAddr;
    Instruction_unit instr;
    bool ready;
public:
    unsigned pc_next;
    unsigned instrAddr_next;
    Instruction_unit instr_next;
    bool ready_next;

public:
    Memory *mem;

public:
    void display();
    void init(Memory *mem_in);
    void flush();
    void decoder( unsigned Rob_pc, unsigned decoder_pc, bool Rob_flag, bool decoder_flag);
    void process();
    void decode(uint32_t src);
    [[nodiscard]] int get_pc(unsigned Rob_pc, unsigned decoder_pc, bool Rob_flag, bool decoder_flag) const;
};

#endif //RISC5_INSTRUCTION_H
