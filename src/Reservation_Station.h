#ifndef RISC5_RESERVATION_STATION_H
#define RISC5_RESERVATION_STATION_H

#include <iostream>
#include "../util/config.h"
#include "ALU.h"
#include "Register_File.h"

class Rob;
class LSB;

class RS_Entry {
public:
    bool busy;

    OptType opt;
    int Ri, Rj, Qi, Qj;
    uint32_t Rob_id;
    bool flag_Ri, flag_Rj;

    int result;
    bool result_ready;
};

class Reservation_Station {
public:
//    int rs_num = 0;
    RS_Entry buffer[RS_SIZE];
private:
//    int rs_next_num = 0;
    RS_Entry buffer_next[RS_SIZE];
private:
    ALU* alu;
    Rob* rob;
    LSB* lsb;

public:
    bool isFull();
    void add(InstrRS &instrRs);
    void init(Rob* rob_in, LSB* lsb_in, ALU* alu_in);
    void flush();
    void step();
    RS_Data get_data();
    void update_data();
//    void Send_to_ALU();
    int get_next();
    bool judge_next(int i);
    void display();
};

#endif //RISC5_RESERVATION_STATION_H
