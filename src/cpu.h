#ifndef RISC5_CPU_H
#define RISC5_CPU_H

#include <iostream>
#include "Memory.h"
#include "Register_File.h"
#include "Reorder_buffer.h"
#include "Load_Store_Buffer.h"
#include "Reservation_Station.h"
#include "decoder.h"
#include "ALU.h"
#include "instruction.h"
#include "../util/config.h"

class CPU {
private:
    Memory *mem;
    RegisterFile reg;
    Rob rob;
    LSB lsb;
    Reservation_Station rs;
    Decoder decoder;
    ALU alu;
    Instruction instr;

public:
    CPU() = default;
    void init(Memory *mem_in);
    void execute();
    void flush();
    bool finish();
};

#endif //RISC5_CPU_H
