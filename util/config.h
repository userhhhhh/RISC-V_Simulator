#ifndef RISC5_CONFIG_H
#define RISC5_CONFIG_H

#include <iostream>
#include <cstdint>

const int MEMORY_SIZE = 1 << 20;
const int RS_SIZE = 16;
const int LSB_SIZE = 16;
const int ROB_SIZE = 1;

enum class RobType {
    reg, store, branch, exit
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
    uint32_t rd, rs1, rs2, imm;
    uint32_t pc_addr;
};
class InstrRS{
public:
    OptType opt;
    uint32_t Ri, Rj, Qi, Qj;
    bool flag_Ri, flag_Rj;
    int result;
    int Rob_id;
};
class InstrLSB{
public:
    LSType opt;
    uint32_t Ri, Rj, Qi, Qj;
    bool flag_Ri, flag_Rj;
    int result;
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
