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
    bool valid;

    OptType opt;
    int Ri, Rj, Qi, Qj;
    uint32_t Rob_id;
    bool flag_Ri, flag_Rj;

    bool ready;
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
    void Rs_To_Rs(int Rob_id, int value);
    void Rob_to_RS(int Rob_id, int value);
    void display();
};

#endif //RISC5_RESERVATION_STATION_H
