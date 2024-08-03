#ifndef RISC5_CONFIG_H
#define RISC5_CONFIG_H

#include <iostream>
#include <cstdint>

const int MEMORY_SIZE = 1 << 20;
const int RS_SIZE = 6;
const int LSB_SIZE = 6;
const int ROB_SIZE = 2;
const int PREDICTOR_SIZE = 256;

enum class RobType {
    reg, load, store, branch, exit, jalr
};
enum class LSType {
    LB, LH, LW, LBU, LHU,
    SB, SH, SW,
    DELETE   //for delete
};
enum class InstructionType {
    R, // 0110011
    I, // 00x0011
    S, // 0100011
    B, // 1100011
    U, // 0x10111
    J  // 1101111
    // END:0ff00513
};
enum OptType {
    LUI, AUIPC,
    JAL,
    JALR,
    BEQ, BNE, BLT, BGE, BLTU, BGEU,
    LB, LH, LW, LBU, LHU,
    SB, SH, SW,
    ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI,
    ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND,
    DELETE   //for delete
};

class InstrRob{
public:
    OptType opt;
    RobType Rob_opt;

    uint32_t rd;
    int value;
    int other;

    bool ready;
};
class InstrRS{
public:
    OptType opt;
    int Ri, Rj, Qi, Qj;
    bool flag_Ri, flag_Rj;
    int Rob_id;

    bool ready;
};
class InstrLSB{
public:
    bool ready;

    LSType opt;
    int value1;
    int value2; // 两个寄存器的值

    int Rob_id;
};

class RS_Data{
public:
    bool ready;
    int Rob_id;
    int value;
};
class LSB_Data{
public:
    bool ready;
    int Rob_id;
    int value;
};

#endif //RISC5_CONFIG_H
