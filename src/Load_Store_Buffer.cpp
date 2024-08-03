#include "Load_Store_Buffer.h"

LSB_Entry::LSB_Entry(const InstrLSB &instr) {
    ready = instr.ready;
    value1 = instr.value1;
    value2 = instr.value2;
    opt = instr.opt;
    Rob_id = instr.Rob_id;
}

void LSB::init(Rob *rob_in, Reservation_Station *rs_in, RegisterFile *reg_file_in, Memory *mem_in) {
    rob = rob_in;
    rs = rs_in;
    reg_file = reg_file_in;
    mem = mem_in;
}

void LSB::add(InstrLSB &instrLsb) {
    LSB_Entry entry(instrLsb);
    buffer_next.push(entry);
}

void LSB::flush() {
    buffer = buffer_next;
}

void LSB::step() {
    if (buffer.isEmpty()) return;
    auto entry = buffer.front();
    if(!entry.ready) return;

    buffer_next.pop();

    switch (entry.opt) {
        case LSType::LB:
            entry.value2 = mem->load_memory(entry.value1, 1, true);
            rob->update_data(entry.Rob_id, entry.value2);
            break;
        case LSType::LH:
            entry.value2 = mem->load_memory(entry.value1, 2, true);
            rob->update_data(entry.Rob_id, entry.value2);
            break;
        case LSType::LW:
            entry.value2 = mem->load_memory(entry.value1, 4, true);
            rob->update_data(entry.Rob_id, entry.value2);
            break;
        case LSType::LBU:
            entry.value2 = mem->load_memory(entry.value1, 1, false);
            rob->update_data(entry.Rob_id, entry.value2);
            break;
        case LSType::LHU:
            entry.value2 = mem->load_memory(entry.value1, 2, false);
            rob->update_data(entry.Rob_id, entry.value2);
            break;

        case LSType::SB:
            mem->store_memory(entry.value1, entry.value2, 1);
            break;
        case LSType::SH:
            mem->store_memory(entry.value1, entry.value2, 2);
            break;
        case LSType::SW:
            mem->store_memory(entry.value1, entry.value2, 4);
            break;
        default:
            break;
    }
}

void LSB::Rob_to_lsb(int Rob_id, int value, int addr) {
    for (int i = 0; i < LSB_SIZE; i++) {
        if (buffer[i].Rob_id == Rob_id) {
            buffer_next[i].value1 = addr;
            buffer_next[i].value2 = value;
            buffer_next[i].ready = true;
        }
    }
}
void LSB::display() {
    std::cout << "-------LSB--------" << std::endl;
    int i = 0;
    for (auto it = buffer.begin(); it != buffer.end(); ++it) {
        i++;
        std::cout << "Ready:" << it->ready << "        ";
        std::cout << "Value1: " << it->value1 << "       ";
        std::cout << "Value2: " << it->value2 << "       ";
        std::cout << "Rob_id: " << it->Rob_id << std::endl;
    }
    std::cout << "LSB size: " << i << std::endl;
    int j = 0;
    for(auto it = buffer_next.begin(); it != buffer_next.end(); ++it){
        j++;
        std::cout << "Ready:" << it->ready << "        ";
        std::cout << "Value1: " << it->value1 << "       ";
        std::cout << "Value2: " << it->value2 << "       ";
        std::cout << "Rob_id: " << it->Rob_id << std::endl;
    }
    std::cout << "LSB_next size: " << j << std::endl;
    std::cout << "----------------------" << std::endl;
}
void LSB::Rs_to_lsb(int Rob_id, int value) {
    for(int i = 0; i < LSB_SIZE; i++){
        if(buffer[i].Rob_id == Rob_id){
            buffer_next[i].value1 = value;
            buffer_next[i].ready = true;
        }
    }
}