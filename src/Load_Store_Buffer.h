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

    LSType opt;
    int value1;
    int value2; // 两个寄存器的值

    // load
    // value1：load的地址  value2：load的值
    // store
    // value1：store的地址  value2：store的值

    int Rob_id = -1;

public:
    LSB_Entry() = default;
    LSB_Entry(const InstrLSB &instr);
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
    void display();
    void Rob_to_lsb(int Rob_id, int value, int addr);
    void Rs_to_lsb(int Rob_id, int value); //告诉地址
};

#endif //RISC5_LOAD_STORE_BUFFER_H
