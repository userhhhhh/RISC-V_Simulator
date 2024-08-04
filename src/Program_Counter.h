#ifndef RISC5_PROGRAM_COUNTER_H
#define RISC5_PROGRAM_COUNTER_H

#include <iostream>

class ProgramCounter {
public:
    int pc;
    bool flag_set_by_decoder = false;
    bool stop = false;                //处理 jalr的情况
    bool wait = false;                //处理停止的情况
public:
    int pc_next;
    bool flag_set_by_decoder_next = false;

public:
    void init();
    void set_pc_next(int index);
    void flush();
    void display();
    void set_stop(bool stop_);
};

#endif //RISC5_PROGRAM_COUNTER_H
