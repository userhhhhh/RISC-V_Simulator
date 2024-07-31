#include "Reservation_Station.h"

void Reservation_Station::init(Rob* rob_in, LSB* lsb_in, ALU* alu_in){
    rob = rob_in;
    lsb = lsb_in;
    alu = alu_in;
}
bool Reservation_Station::isFull(){
    for(int i = 0; i < RS_SIZE; i++){
        if(!buffer[i].busy) return false;
    }
    return true;
}
void Reservation_Station::add(InstrRS &instrRs) {
    int index = -1;
    bool flag = false;
    for(int i = 0; i < RS_SIZE; ++i){
        if(buffer[i].busy) continue;
        flag = true;
        index = i;
    }
    if(!flag) return;
    buffer[index].busy = true;
    buffer[index].opt = instrRs.opt;
    buffer[index].Ri = instrRs.Ri;
    buffer[index].Rj = instrRs.Rj;
    buffer[index].Qi = instrRs.Qi;
    buffer[index].Qj = instrRs.Qj;
    buffer[index].Rob_id = instrRs.Rob_id;
    buffer[index].flag_Ri = instrRs.flag_Ri;
    buffer[index].flag_Rj = instrRs.flag_Rj;
    buffer[index].result = instrRs.result;
    rs_num++;
}
void Reservation_Station::flush(){
    for(int i = 0; i < RS_SIZE; i++){
        buffer[i] = buffer_next[i];
    }
}
bool Reservation_Station::judge_next(int i){
    LSB_Data lsbData = lsb->get_data();
    bool judge1 = buffer[i].busy;
    bool judge2 = !buffer[i].flag_Ri || (buffer[i].Qi == lsbData.Rob_id && lsbData.ready) || (buffer[i].Qi == alu->Rob_id && alu->ready);
    bool judge3 = !buffer[i].flag_Rj || (buffer[i].Qj == lsbData.Rob_id && lsbData.ready) || (buffer[i].Qj == alu->Rob_id && alu->ready);
    if(judge1 && judge2 && judge3) return true;
    return false;
}
int Reservation_Station::get_next() {
    for(int i = 0; i < RS_SIZE; i++){
        if(judge_next(i)) return i;
    }
    return -1;
}
void Reservation_Station::step(){
    update_data();

    int index = get_next();
    if(index < 0) return;
    auto entry = buffer[index];

    LSB_Data lsbData = lsb->get_data();

    int get_Ri, get_Rj;
    if(!entry.flag_Ri) get_Ri = entry.Ri;
    else if(alu->ready && entry.Qi == alu->Rob_id) get_Ri = alu->get_Data();
    else if(lsbData.ready && entry.Qi == lsbData.Rob_id) get_Ri = lsbData.value;
    else {}
    if(!entry.flag_Rj) get_Rj = entry.Rj;
    else if(alu->ready && entry.Qj == alu->Rob_id) get_Rj = alu->get_Data();
    else if(lsbData.ready && entry.Qj == lsbData.Rob_id) get_Rj = lsbData.value;
    else {}

    Send_to_ALU();
}
void Reservation_Station::Send_to_ALU() {
    for(int i = 0; i < RS_SIZE; i++){
        if(buffer[i].opt == OptType::DELETE) continue;
        if(alu->ready && !buffer[i].flag_Ri && !buffer[i].flag_Rj){
            buffer_next[i].result = alu->calculate(buffer[i].opt, buffer[i].Ri, buffer[i].Rj, buffer[i].Rob_id);
//            Send_to_RoB_or_LSB(buffer[i].opt, buffer[i].Rob_id, buffer_next[i].result);
            buffer_next[i].opt = OptType::DELETE;
        }
    }
}
//void Reservation_Station::Send_to_RoB_or_LSB(OptType opt, uint32_t Rob_id, int result){
//    if(opt == OptType::LW || opt == OptType::LH || opt == OptType::LHU || opt == OptType::LB || opt == OptType::LBU){
//        lsb->update_data(opt, result, -1, Rob_id);
//    }else{
//        rob->set_ready(Rob_id, result);
//    }
//}

RS_Data Reservation_Station::get_data(){
    RS_Data data;
    data.ready = alu->ready;
    data.Rob_id = alu->Rob_id;
    data.value = alu->get_Data();
    return data;
}
void Reservation_Station::update_data(){
    LSB_Data lsbData = lsb->get_data();
    if(lsbData.ready){
        for(int i = 0; i < RS_SIZE; i++){
            if(buffer[i].opt == OptType::DELETE) continue;
            if(buffer[i].Qi == lsbData.Rob_id){
                buffer_next[i].Qi = 0;
                buffer_next[i].Ri = lsbData.value;
                buffer_next[i].flag_Ri = false;
            }
            if(buffer[i].Qj == lsbData.Rob_id){
                buffer_next[i].Qj = 0;
                buffer_next[i].Rj = lsbData.value;
                buffer_next[i].flag_Rj = false;
            }
        }
    }
    if(alu->ready){
        for(int i = 0; i < RS_SIZE; i++){
            if(buffer[i].opt == OptType::DELETE) continue;
            if(buffer[i].Qi == alu->Rob_id){
                buffer_next[i].Qi = 0;
                buffer_next[i].Ri = alu->get_Data();
                buffer_next[i].flag_Ri = false;
            }
            if(buffer[i].Qj == alu->Rob_id){
                buffer_next[i].Qj = 0;
                buffer_next[i].Rj = alu->get_Data();
                buffer_next[i].flag_Rj = false;
            }
        }
    }
}
void Reservation_Station::display(){
    std::cout << "-------Reservation Station--------" << std::endl;
//    for(int i = 0; i < RS_SIZE; i++){
//        std::cout << "RS[" << i << "]: ";
//        if(buffer[i].busy){
//            std::cout << "busy" << std::endl;
//        }else{
//            std::cout << "empty" << std::endl;
//        }
//    }
    std::cout << "---------------------------------" << std::endl;
}

