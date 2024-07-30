#ifndef RISC5_LOAD_STORE_BUFFER_H
#define RISC5_LOAD_STORE_BUFFER_H

#include <iostream>
#include "Reservation_Station.h"

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
    uint32_t Ri, Rj, Qi, Qj;
    bool flag_Ri, flag_Rj;
    int result;

    int Rob_id;

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

public:
    void init(Rob *rob_in, Reservation_Station *rs_in, RegisterFile *reg_file_in);
    void flush();
    void step();
    void push(LSB_Entry x);
    LSB_Data get_data();
    void update_data();
    void Send_to_Rob(int rob_id, uint32_t value);
    void clear();
    void print();
    LSB& operator= (const LSB& lsb);
};

#endif //RISC5_LOAD_STORE_BUFFER_H
