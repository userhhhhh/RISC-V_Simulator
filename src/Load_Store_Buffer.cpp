#include "Load_Store_Buffer.h"

LSB_Entry::LSB_Entry(const InstrLSB &instr, bool busy_in) {
    ready = false;
    offset = instr.offset;
    opt = instr.opt;
    Ri = instr.Ri;
    Rj = instr.Rj;
    Qi = instr.Qi;
    Qj = instr.Qj;
    flag_Ri = instr.flag_Ri;
    flag_Rj = instr.flag_Rj;
    result = 0;
    Rob_id = -1;
}

void LSB::init(Rob *rob_in, Reservation_Station *rs_in, RegisterFile *reg_file_in, Memory *mem_in) {
    rob = rob_in;
    rs = rs_in;
    reg_file = reg_file_in;
    mem = mem_in;
}

void LSB::add(InstrLSB &instrLsb) {
    int index = -1;
    bool flag = false;
    for (int i = 0; i < LSB_SIZE; ++i) {
        if (buffer[i].busy) continue;
        flag = true;
        index = i;
    }
    if (!flag) return;
    if(!buffer[index].flag_Ri && !buffer[index].flag_Rj){
        buffer[index].ready = true;
    }
    buffer[index].opt = instrLsb.opt;
    buffer[index].Ri = instrLsb.Ri;
    buffer[index].Rj = instrLsb.Rj;
    buffer[index].Qi = instrLsb.Qi;
    buffer[index].Qj = instrLsb.Qj;
    buffer[index].flag_Ri = instrLsb.flag_Ri;
    buffer[index].flag_Rj = instrLsb.flag_Rj;
    buffer[index].result = instrLsb.result;
    buffer[index].Rob_id = instrLsb.Rob_id;
    buffer[index].to_execute = false;
}

void LSB::flush() {
    buffer = buffer_next;
    result = result_next;
}

void LSB::step() {

    update_data();

    int index = (buffer.head + 1) % LSB_SIZE;
    auto entry = buffer[index];
    if(entry.opt >= LSType::LB && entry.opt <= LSType::LHU){
        if(!entry.flag_Ri && !entry.flag_Rj){
            buffer[index].ready = true;
            buffer_next[index].ready = true;
        }
    }
    if(entry.opt >= LSType::SB && entry.opt <= LSType::SW) {
        if ((rob->buffer.head + 1) % ROB_SIZE != buffer.front().Rob_id) {
            return;
        }
    }
    buffer_next.pop();

//    int get_Ri, get_Rj;
//    if (!entry.flag_Ri) get_Ri = entry.Ri;
//    else get_Ri = rs->buffer[entry.Qi].result;
//    if (!entry.flag_Rj) get_Rj = entry.Rj;
//    else get_Rj = rs->buffer[entry.Qj].result;

    switch (entry.opt) {
        case LSType::LB:
            result_next.value = mem->load_memory(entry.Ri + entry.offset, 1, true);
            result_next.ready = true;
            break;
        case LSType::LH:
            result_next.value = mem->load_memory(entry.Ri + entry.offset, 2, true);
            result_next.ready = true;
            break;
        case LSType::LW:
            result_next.value = mem->load_memory(entry.Ri + entry.offset, 4, true);
            result_next.ready = true;
            break;
        case LSType::LBU:
            result_next.value = mem->load_memory(entry.Ri + entry.offset, 1, false);
            result_next.ready = true;
            break;
        case LSType::LHU:
            result_next.value = mem->load_memory(entry.Ri + entry.offset, 2, false);
            result_next.ready = true;
            break;

        case LSType::SB:
            mem->store_memory(entry.Ri + entry.offset, entry.Rj, 1);
            entry.ready = true;
            break;
        case LSType::SH:
            mem->store_memory(entry.Ri + entry.offset, entry.Rj, 2);
            entry.ready = true;
            break;
        case LSType::SW:
            mem->store_memory(entry.Ri + entry.offset, entry.Rj, 4);
            entry.ready = true;
            break;
        default:
            break;
    }

//    if (entry.opt >= LSType::LB && entry.opt <= LSType::LHU) {
//        // TODO: ?
//    }
}

bool LSB::judge_ready(int i) {
    RS_Data rsData = rs->get_data();
    bool judge1 = buffer[i].flag_Ri || (!buffer[i].flag_Ri && rsData.Rob_id == buffer[i].Rob_id);
    bool judge2 = buffer[i].flag_Rj || (!buffer[i].flag_Rj && rsData.Rob_id == buffer[i].Rob_id);
    bool judge3 = buffer[i].Rob_id == rob->buffer.head + 1;
    bool judge4 = buffer[i].busy;
    if (judge1 && judge2 && judge3 && judge4) {
        return true;
    }
    return false;
}

bool LSB::judge_stop(int i) {
    if (buffer[i].busy && buffer[i].opt >= LSType::SB && buffer[i].opt <= LSType::SW) {
        return true;
    }
    return false;
}

void LSB::push(LSB_Entry x) {
    buffer_next.push(x);
}

void LSB::update_data() {
    RS_Data rsData = rs->get_data();
    if (rsData.ready) {
        for (int i = 0; i < LSB_SIZE; i++) {
            if (buffer[i].opt == LSType::DELETE) continue;
            if (buffer[i].Qi == rsData.Rob_id && buffer[i].flag_Ri) {
                buffer_next[i].Qi = 0;
                buffer_next[i].Ri = rsData.value;
                buffer_next[i].flag_Ri = false;
            }
            if (buffer[i].Qj == rsData.Rob_id && buffer[i].flag_Rj) {
                buffer_next[i].Qj = 0;
                buffer_next[i].Rj = rsData.value;
                buffer_next[i].flag_Rj = false;
            }
        }
    }
//    if ((rob->buffer.head + 1) % ROB_SIZE == buffer.front().Rob_id && !buffer.front().to_execute) {
//        buffer[buffer.head + 1].to_execute = true;
//    }
}

LSB_Data LSB::get_data() {
//    LSB_Data data;
//    data.ready = buffer.front().ready;
//    data.Rob_id = buffer.front().Rob_id;
//    data.value = result.value;
//    return data
    return result;
}

void LSB::display() {
    std::cout << "-------LSB--------" << std::endl;
    for (int i = 0; i < LSB_SIZE; i++) {
        std::cout << "LSB[" << i << "]: ";
        std::cout << "ready: " << buffer[i].ready << " ";
        std::cout << "busy: " << buffer[i].busy << " ";
        std::cout << "Ri: " << buffer[i].Ri << " ";
        std::cout << "Rj: " << buffer[i].Rj << " ";
        std::cout << "Qi: " << buffer[i].Qi << " ";
        std::cout << "Qj: " << buffer[i].Qj << " ";
        std::cout << "flag_Ri: " << buffer[i].flag_Ri << " ";
        std::cout << "flag_Rj: " << buffer[i].flag_Rj << " ";
        std::cout << "result: " << buffer[i].result << " ";
        std::cout << "Rob_id: " << buffer[i].Rob_id << " ";
        std::cout << "to_execute: " << buffer[i].to_execute << std::endl;
    }
    std::cout << "----------------------" << std::endl;
}