#ifndef RISC5_LOAD_STORE_BUFFER_H
#define RISC5_LOAD_STORE_BUFFER_H

#include <iostream>
#include "Reservation_Station.h"
#include "memory.h"

#include "../util/utils.h"
#include "../util/config.h"
#include "../util/CircularQueue.h"

class Rob;
class Reservation_Station;
class RegisterFile;

class LSB_Entry {
public:
    bool ready;
    bool busy;

    LSType opt;
    uint32_t Ri, Rj, Qi, Qj;
    bool flag_Ri, flag_Rj;
    int offset;
    int result;

    int Rob_id = -1;

    bool to_execute; //判断能否执行

public:
    LSB_Entry() = default;
    LSB_Entry(const InstrLSB &instr, bool busy_in);
};

class LSB {
public:
    queue<LSB_Entry, LSB_SIZE> buffer;
private:
    queue<LSB_Entry, LSB_SIZE> buffer_next;

private:
    Rob *rob;
    Reservation_Station *rs;
    RegisterFile *reg_file;
    Memory *mem;

public:
    void add(InstrLSB &instrLsb);
    void init(Rob *rob_in, Reservation_Station *rs_in, RegisterFile *reg_file_in, Memory *mem_in);
    void flush();
    void step();
    void push(LSB_Entry x);
    LSB_Data get_data();
    void update_data();
    void display();
    int get_index();
    bool judge_ready(int i);
    bool judge_stop(int i);

    void clear();
    void print();
    LSB& operator= (const LSB& lsb);
};

#endif //RISC5_LOAD_STORE_BUFFER_H
