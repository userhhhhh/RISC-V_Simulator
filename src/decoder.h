#ifndef RISC5_DECODER_H
#define RISC5_DECODER_H

#include "instruction.h"
#include "Load_Store_Buffer.h"
#include "Reservation_Station.h"
#include "../util/config.h"
#include <iostream>

class RegisterFile;
class Rob;
class Instruction;

class Decoder {
public:
    bool flag;
    int pc;
private:
    bool flag_next;
    int pc_next;

private:
    bool Rob_flag;
    bool RS_flag;
    bool LSB_flag;
    InstrRob instrRob;
    InstrRS instrRs;
    InstrLSB instrLsb;
public:
    RegisterFile *reg;
    Rob *rob;
    Instruction *instr_in;

public:
    void init(RegisterFile *reg_in, Rob *rob_in, Instruction *_instr_in);
    void step();
    void execute(Instruction &instr);
    void func_branch(Instruction &instr);
    void func_load(Instruction &instr);
    void func_store(Instruction &instr);
    void func_cal_imm(Instruction &instr);
    void func_cal(Instruction &instr);
    void func_exit(Instruction &instr);
    void get_Ri(Instruction &instr);
    void get_Rj(Instruction &instr);
    LSType get_LSType(Instruction &instr);
    RobType get_RobType(Instruction &instr);
    void flush();

};

#endif //RISC5_DECODER_H
