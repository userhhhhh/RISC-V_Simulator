#include "Reorder_buffer.h"

Rob_Entry::Rob_Entry(const InstrRob &instr, bool busy_in) {
    ready = instr.ready;
    type = instr.Rob_opt;
    value = instr.value;
    rd = instr.rd;
    busy = busy_in;
}
void Rob::init(Reservation_Station *rs_in, LSB *lsb_in, RegisterFile *reg_in) {
    rs = rs_in;
    lsb = lsb_in;
    reg = reg_in;
}
void Rob::flush(){
    buffer = buffer_next;
}
void Rob::set_ready(uint32_t rob_id, uint32_t value) {
    for(auto it = buffer_next.begin(); it != buffer_next.end(); ++it){
        if(it->id == rob_id){
            it->value = value;
            it->ready = true;
        }
    }
}
void Rob::step(){
    RS_Data rsData = rs->get_data();
    if(rsData.ready){
        buffer_next[rsData.Rob_id].value = rsData.value;
        buffer_next[rsData.Rob_id].ready = true;
    }
    LSB_Data lsbData = lsb->get_data();
    if(lsbData.ready){
        buffer_next[lsbData.Rob_id].value = lsbData.value;
        buffer_next[lsbData.Rob_id].ready = true;
    }
    if(buffer.isEmpty()) return;
    Rob_Entry entry = *buffer.begin();
    if(!entry.ready) return;
    buffer_next.pop();
    switch(entry.type){
        case RobType::reg:
            reg->write(entry.rd, (buffer.head + 1) % ROB_SIZE, entry.value);
            break;
        case RobType::store:
            break;
        case RobType::branch:
            // TODO
            if(entry.value){
                PC_next = entry.value;
            }
            break;
        case RobType::exit:
            throw reg->registers[10].value & 0xff;
    }
}
void Rob::add(InstrRob instr) {
    Rob_Entry entry(instr, true);
    if(entry.type == RobType::reg){
        reg->update_independence(entry.rd, buffer.tail);
    }
    buffer_next.push(entry);
}
int Rob::get_tail_next(){
    return (buffer.tail + 1) % ROB_SIZE;
}
void Rob::display() {
    std::cout << "--------ROB---------" << std::endl;
    std::cout << "PC:" << PC << std::endl;
    for(auto it = buffer.begin(); it != buffer.end(); ++it){
        std::cout << "id:" << it->id << " ready:" << it->ready << " busy:" << it->busy << " type:" << (int)it->type << " value:" << it->value << " rd:" << it->rd << std::endl;
    }
    std::cout << "----------------------" << std::endl;
}


