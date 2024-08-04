#ifndef RISC5_REGISTER_FILE_H
#define RISC5_REGISTER_FILE_H

#include <iostream>
#include "Reorder_buffer.h"

class Rob;

class Register_Entry{
public:
    int value = 0;             //寄存器的值
    bool Is_dependent = false; //寄存器是否有依赖
    int Rob_index = -1;        //如果有依赖，依赖对应在ROB中的位置
};

class RegisterFile {
public:
    Register_Entry registers[32];
    int modify[32];            //标注这个周期内有没有修改，周期结束清空
private:
    Register_Entry registers_next[32];
private:
    Rob *rob;
public:
    void init(Rob *rob_in);
    void flush();
    void execute(bool clear_flag);
    void write(uint32_t reg_index, int Rob_index, int value);
    void get(int index, bool &depend_get, int &value_get);
    void clear(); //清除所有依赖关系
    RegisterFile& operator= (const RegisterFile& rf);
    void update_independence(int reg_index, int Rob_index);
    void display();
};

#endif //RISC5_REGISTER_FILE_H
