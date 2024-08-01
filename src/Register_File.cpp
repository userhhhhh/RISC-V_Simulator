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
void RegisterFile::flush() {
    for (int i = 0; i < 32; i++) {
        registers[i] = registers_next[i];
    }
}
void RegisterFile::write(uint32_t reg_index, int Rob_index, int value) {
    if(Rob_index == registers[reg_index].Rob_index) {
        registers_next[reg_index].Is_dependent = false;
        registers_next[reg_index].Rob_index = -1;
    }
    else {
        registers_next[reg_index].Is_dependent = true;
        registers_next[reg_index].Rob_index = Rob_index;
    }
    registers_next[reg_index].value = value;
}
void RegisterFile::clear() {
    for (auto & i : registers_next)
        i.Is_dependent = false;
}
void RegisterFile::print() {
    for (int i = 0; i < 32; i++) {
        std::cout << "Register " << i << ": " << registers[i].value << std::endl;
    }
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
    for (int i = 0; i < 16; i++) {
        std::cout << "Register " << i << ": " << registers[i].value << std::endl;
    }
    for (int i = 16; i < 32; i++) {
        std::cout << "Register " << i << ": " << registers[i].value << std::endl;
    }
    std::cout << std::dec;
    std::cout << "---------------------------" << std::endl;
}
