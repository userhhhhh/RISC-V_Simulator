#include "Program_Counter.h"

void ProgramCounter::init() {
    pc = 0;
}
void ProgramCounter::set_pc_next(int index) {
    pc_next = index;
    flag_set_by_decoder_next = true;
}
void ProgramCounter::flush() {
//    if(wait){
//        wait = false;
//        return;
//    }
    pc = pc_next;
    flag_set_by_decoder = flag_set_by_decoder_next;
    flag_set_by_decoder_next = false;
    pc_next += 4;
}
void ProgramCounter::display() {
    std::cout << "---------pc---------" << std::endl;
    std::cout << std::hex;
    std::cout << "PC: " << pc << std::endl;
    std::cout << "PC_NEXT: " << pc_next << std::endl;
    std::cout << std::dec;
    std::cout << "--------------------" << std::endl;
}
void ProgramCounter::set_stop(bool stop_) {
    stop = stop_;
}