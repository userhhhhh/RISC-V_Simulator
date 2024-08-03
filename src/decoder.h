#ifndef RISC5_DECODER_H
#define RISC5_DECODER_H

#include "instruction.h"
#include "Load_Store_Buffer.h"
#include "Reservation_Station.h"
#include "../util/config.h"
#include "Program_Counter.h"
#include <iostream>
#include "Predictor.h"


class RegisterFile;
class Rob;
class Instruction;

class Decoder {
private:
    bool Rob_flag;
    bool RS_flag;
    bool LSB_flag;
    InstrRob instrRob;
    InstrRS instrRs;
    InstrLSB instrLsb;
public:
    RegisterFile *reg;
    Reservation_Station *rs;
    Rob *rob;
    Instruction *instr_in;
    LSB *lsb;
    ProgramCounter *programCounter;
    Predictor *predictor;

public:
    void init(RegisterFile *reg_in, Rob *rob_in, Instruction *_instr_in, LSB *lsb_in, Reservation_Station *rs_in, ProgramCounter *programCounter_in, Predictor *predictor_in);
    void step();
    void execute(Instruction &instr);
    void func_branch(Instruction &instr);
    void func_load(Instruction &instr);
    void func_store(Instruction &instr);
    void func_cal_imm(Instruction &instr);
    void func_cal(Instruction &instr);
    void func_exit(Instruction &instr);
    void get_Ri_Rs(Instruction &instr);
    void get_Rj_Rs(Instruction &instr);
    static LSType get_LSType(Instruction &instr);
    static RobType get_RobType(Instruction &instr);
    void flush();
    void display();
};

#endif //RISC5_DECODER_H
