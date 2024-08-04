#ifndef RISC5_REORDER_BUFFER_H
#define RISC5_REORDER_BUFFER_H

#include <iostream>
#include "Reservation_Station.h"
#include "Load_Store_Buffer.h"
#include "../util/config.h"
#include "../util/CircularQueue.h"
#include "Program_Counter.h"
#include "Instruction.h"
#include "Predictor.h"

class RegisterFile;
class Reservation_Station;
class LSB;

extern int total;

class Rob_Entry {
public:
    int id;      //单元在队列中的位置
    bool ready;  //是否可以提交

    OptType opt;
    RobType type;

    int other;         // 操作数2
    int value;         // 操作数
    uint32_t rs1, rs2; // instr里面的两个寄存器
    uint32_t rd;       // instr里面的目标寄存器

public:
    Rob_Entry() = default;
    Rob_Entry(const InstrRob &instr);
};

class Rob {
public:
    queue<Rob_Entry, ROB_SIZE> buffer;
    int PC;
    bool flag_clear;
private:
    queue<Rob_Entry, ROB_SIZE> buffer_next;
    int PC_next;
    bool flag_clear_next;

private:
    Reservation_Station *rs;
    LSB *lsb;
    RegisterFile *reg;
    Predictor *predictor;
    ProgramCounter *programCounter;

public:
    void init(Reservation_Station *rs_in, LSB *lsb_in, RegisterFile *reg_in, Predictor *predictor_in, ProgramCounter *programCounter_in);
    void flush();
    void update_data(int Rob_id, int value);
    void set_ready(uint32_t rob_id, uint32_t value);
    void step(bool &to_be_cleared);
    void add(InstrRob instr);
    void commit(bool &to_be_cleared);
    [[nodiscard]] int get_tail_next()const;
    void display();
};

#endif //RISC5_REORDER_BUFFER_H
