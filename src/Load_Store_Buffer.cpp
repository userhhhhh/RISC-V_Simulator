#include "Load_Store_Buffer.h"

LSB_Entry::LSB_Entry(const InstrLSB &instr, bool busy_in) {
    ready = false;
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
void LSB::init(Rob *rob_in, Reservation_Station *rs_in, RegisterFile *reg_file_in){
    rob = rob_in;
    rs = rs_in;
    reg_file = reg_file_in;
}
void LSB::add(InstrLSB &instrLsb){
    int index = -1;
    bool flag = false;
    for(int i = 0; i < LSB_SIZE; ++i){
        if(buffer[i].busy) continue;
        flag = true;
        index = i;
    }
    if(!flag) return;
    buffer[index].ready = true;
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
void LSB::flush(){
    buffer = buffer_next;
}
void LSB::step(){
    update_data();
    int index = (buffer.head + 1) % LSB_SIZE;
    auto entry = buffer[index];
    buffer_next.pop();

    int get_Ri, get_Rj;
    if(!entry.flag_Ri) get_Ri = entry.Ri;
    else get_Ri = rs->buffer[entry.Qi].result;
    if(!entry.flag_Rj) get_Rj = entry.Rj;
    else get_Rj = rs->buffer[entry.Qj].result;

    if(entry.opt >= LSType::LB && entry.opt <= LSType::LHU){
        reg_file->write(get_Ri, entry.Rob_id, get_Rj);
    }
    else if(entry.opt >= LSType::SB && entry.opt <= LSType::SW){
        reg_file->write(get_Ri, entry.Rob_id, get_Rj);
    }
}
void LSB::push(LSB_Entry x){
    buffer_next.push(x);
}
void LSB::update_data(){
    RS_Data rsData = rs->get_data();
    if(rsData.ready){
        for(int i = 0; i < LSB_SIZE; i++){
            if(buffer[i].opt == LSType::DELETE) continue;
            if(buffer[i].Qi == rsData.Rob_id && buffer[i].flag_Ri){
                buffer_next[i].Qi = 0;
                buffer_next[i].Ri = rsData.value;
                buffer_next[i].flag_Ri = false;
            }
            if(buffer[i].Qj == rsData.Rob_id && buffer[i].flag_Rj){
                buffer_next[i].Qj = 0;
                buffer_next[i].Rj = rsData.value;
                buffer_next[i].flag_Rj = false;
            }
        }
    }
    if(rob->buffer.head + 1 == buffer.front().Rob_id && !buffer.front().to_execute){
        buffer[buffer.head + 1].to_execute = true;
    }
}
LSB_Data LSB::get_data() {
    LSB_Data data;
    data.ready = buffer.front().ready;
    data.Rob_id = buffer.front().Rob_id;
    data.value = buffer.front().result;
    return data;
}