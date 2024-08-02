#ifndef RISC5_REORDER_BUFFER_H
#define RISC5_REORDER_BUFFER_H

#include <iostream>
#include "Reservation_Station.h"
#include "Load_Store_Buffer.h"
#include "../util/config.h"
#include "../util/CircularQueue.h"

class RegisterFile;
class Reservation_Station;
class LSB;

class Rob_Entry {
public:
    int id;
    bool ready;
    bool busy;

    OptType opt;
    RobType type;
    int value;
    uint32_t rd;

public:
    Rob_Entry() = default;
    Rob_Entry(const InstrRob &instr, bool busy_in);
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

public:
    void init(Reservation_Station *rs_in, LSB *lsb_in, RegisterFile *reg_in);
    void flush();
    void set_ready(uint32_t rob_id, uint32_t value);
    void step();
    void add(InstrRob instr);
    int get_tail_next();
    void display();
};

#endif //RISC5_REORDER_BUFFER_H
