#include "Register_File.h"

void RegisterFile::init(Rob *rob_in) {
    rob = rob_in;
    for (auto & i : registers) {
        i.Is_dependent = false;
        i.Rob_index = -1;
    }
}
void RegisterFile::execute(bool clear_flag) {
    if(clear_flag) {
        clear();
    }
}
void RegisterFile::get(int index, bool &depend_get, int &value_get){
    depend_get = registers[index].Is_dependent;
    if(!depend_get) value_get = registers[index].value;
    else value_get = -1;
}

void RegisterFile::flush() {
    for (int i = 0; i < 32; i++) {
        registers[i] = registers_next[i];
        modify[i] = 0;
    }
}

void RegisterFile::write(uint32_t reg_index, int Rob_index, int value) {
    registers_next[reg_index].value = value;
    if(registers_next[reg_index].Is_dependent) {
        if(Rob_index == registers_next[reg_index].Rob_index) {
            registers_next[reg_index].Is_dependent = false;
            registers_next[reg_index].Rob_index = -1;
        }
    }
//    if(Rob_index == registers[reg_index].Rob_index && ) {
//        registers_next[reg_index].Is_dependent = false;
//        registers_next[reg_index].Rob_index = -1;
//    }
}

void RegisterFile::clear() {
    for (auto & i : registers_next)
        i.Is_dependent = false;
}
RegisterFile& RegisterFile::operator= (const RegisterFile& rf) {
    if(this == &rf) return *this;
    for (int i = 0; i < 32; i++) {
        registers[i] = rf.registers[i];
    }
    return *this;
}
void RegisterFile::update_independence(int reg_index, int Rob_index) {
    registers_next[reg_index].Is_dependent = true;
    registers_next[reg_index].Rob_index = Rob_index;
}
void RegisterFile::display() {
    std::cout << "-------Register File--------" << std::endl;
    std::cout << std::hex;
    for (int i = 0; i < 32; i++) {
        std::cout << "Register " << i << ": " << registers[i].value << "     ";
        std::cout << "Is dependent: " << registers[i].Is_dependent << "     ";
        std::cout << "ROB index: " << registers[i].Rob_index ;
        std::cout << "Register_next " << i << ": " << registers_next[i].value << "     ";
        std::cout << "Is dependent_next: " << registers_next[i].Is_dependent << "     ";
        std::cout << "ROB index_next: " << registers_next[i].Rob_index ;
        std::cout << std::endl;
    }
    std::cout << std::dec;
    std::cout << "---------------------------" << std::endl;
}
