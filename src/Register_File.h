#ifndef RISC5_REGISTER_FILE_H
#define RISC5_REGISTER_FILE_H

#include <iostream>
#include "Reorder_buffer.h"

class Rob;

class Register_Entry{
public:
    int value = 0;
    bool Is_dependent = false; //无依赖关系
    int Rob_index = -1;
};

class RegisterFile {
public:
    Register_Entry registers[32];
private:
    Register_Entry registers_next[32];
private:
    Rob *rob;
public:
    void init(Rob *rob_in);
    void flush();
    void execute(bool clear_flag);
    void write(uint32_t reg_index, int Rob_index, int value);
    void clear(); //清除所有依赖关系
    void print();
    RegisterFile& operator= (const RegisterFile& rf);
    void update_independence(int reg_index, int Rob_index);
    void display();
};

#endif //RISC5_REGISTER_FILE_H
