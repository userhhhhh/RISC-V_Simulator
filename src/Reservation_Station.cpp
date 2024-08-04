#include "Reservation_Station.h"

void Reservation_Station::init(Rob *rob_in, LSB *lsb_in, ALU *alu_in) {
    rob = rob_in;
    lsb = lsb_in;
    alu = alu_in;
}

bool Reservation_Station::isFull() {
    for (int i = 0; i < RS_SIZE; i++) {
        if (!buffer[i].valid) return false;
    }
    return true;
}

void Reservation_Station::Rob_to_RS(int Rob_id, int value) {
    for (int i = 0; i < RS_SIZE; i++) {
        if (!buffer[i].valid) continue;
        if (buffer[i].Qi == Rob_id) {
            buffer_next[i].Qi = -1;
            buffer_next[i].Ri = value;
            buffer_next[i].flag_Ri = false;
        }
        if(buffer[i].Qj == Rob_id){
            buffer_next[i].Qj = -1;
            buffer_next[i].Rj = value;
            buffer_next[i].flag_Rj = false;
        }
    }
}

void Reservation_Station::add(InstrRS &instrRs) {
    int index = -1;
    for (int i = 0; i < RS_SIZE; ++i) {
        if (buffer[i].valid) continue;
        else {
            index = i;
            break;
        }
    }
    if (index == -1) return;
    buffer_next[index].valid = true;
    buffer_next[index].opt = instrRs.opt;
    buffer_next[index].Ri = instrRs.Ri;
    buffer_next[index].Rj = instrRs.Rj;
    buffer_next[index].Qi = instrRs.Qi;
    buffer_next[index].Qj = instrRs.Qj;
    buffer_next[index].Rob_id = instrRs.Rob_id;
    buffer_next[index].flag_Ri = instrRs.flag_Ri;
    buffer_next[index].flag_Rj = instrRs.flag_Rj;
    buffer_next[index].ready = instrRs.ready;
}

void Reservation_Station::flush() {
    for (int i = 0; i < RS_SIZE; i++) {
        buffer[i] = buffer_next[i];
    }
}

void Reservation_Station::step() {
    for(int i = 0; i < RS_SIZE; i++){
        if(!buffer[i].valid) continue;
        if(buffer[i].flag_Ri || buffer[i].flag_Rj) continue;
        auto entry = buffer[i];
        int result = alu->calculate(entry.opt, entry.Ri, entry.Rj, entry.Rob_id);
        buffer_next[i].valid = false;
//        Rs_To_Rs(entry.Rob_id, result);
//        rob->update_data(entry.Rob_id, result);
        if(entry.opt == JALR){
            rob->update_data(entry.Rob_id, result);
        }
        else if(entry.opt >= LB && entry.opt <= LHU){
            lsb->Rs_to_lsb(entry.Rob_id, result);
        }
        else if(entry.opt >= SB && entry.opt <= SW){
            rob->update_data(entry.Rob_id, result);
        }
        else if(entry.opt >= ADDI && entry.opt <= SRAI){
            Rs_To_Rs(entry.Rob_id, result);
            rob->update_data(entry.Rob_id, result);
        }
        else if(entry.opt >= ADD && entry.opt <= XOR){
            Rs_To_Rs(entry.Rob_id, result);
            rob->update_data(entry.Rob_id, result);
        }
        else if(entry.opt >= BEQ && entry.opt <= BGEU){
            rob->update_data(entry.Rob_id, result);
        }
    }
}

void Reservation_Station::Rs_To_Rs(int Rob_id, int value){
    for(int i = 0; i < RS_SIZE; ++i){
        if(buffer[i].flag_Ri && buffer[i].Qi == Rob_id){
            buffer_next[i].Ri = value;
            buffer_next[i].flag_Ri = false;
            buffer_next[i].Qi = -1;
        }
        if(buffer[i].flag_Rj && buffer[i].Qj == Rob_id){
            buffer_next[i].Rj = value;
            buffer_next[i].flag_Rj = false;
            buffer_next[i].Qj = -1;
        }
        if(!buffer_next[i].flag_Ri && !buffer_next[i].flag_Rj){
            buffer_next[i].ready = true;
        }
    }
}

void Reservation_Station::display() {
    std::cout << "-------Reservation Station--------" << std::endl;
    for(int i = 0; i < RS_SIZE; i++){
        std::cout << "RS[" << i << "]: ";
        if(buffer[i].valid){
            std::cout << "valid ";
            std::cout << "opt: " << buffer[i].opt << " ";
            std::cout << "Ri: " << buffer[i].Ri << " ";
            std::cout << "Rj: " << buffer[i].Rj << " ";
            std::cout << "Qi: " << buffer[i].Qi << " ";
            std::cout << "Qj: " << buffer[i].Qj << " ";
            std::cout << "Rob_id: " << buffer[i].Rob_id << " ";
            std::cout << "flag_Ri: " << buffer[i].flag_Ri << " ";
            std::cout << "flag_Rj: " << buffer[i].flag_Rj << " ";
            std::cout << "ready: " << buffer[i].ready << std::endl;
        }
        else{
            std::cout << "invalid" << std::endl;
        }
    }
    for(int i = 0; i < RS_SIZE; i++){
        std::cout << "RS[" << i << "]: ";
        if(buffer_next[i].valid){
            std::cout << "valid ";
            std::cout << "opt: " << buffer_next[i].opt << " ";
            std::cout << "Ri: " << buffer_next[i].Ri << " ";
            std::cout << "Rj: " << buffer_next[i].Rj << " ";
            std::cout << "Qi: " << buffer_next[i].Qi << " ";
            std::cout << "Qj: " << buffer_next[i].Qj << " ";
            std::cout << "Rob_id: " << buffer_next[i].Rob_id << " ";
            std::cout << "flag_Ri: " << buffer_next[i].flag_Ri << " ";
            std::cout << "flag_Rj: " << buffer_next[i].flag_Rj << " ";
            std::cout << "ready: " << buffer_next[i].ready << std::endl;
        }
        else{
            std::cout << "invalid" << std::endl;
        }
    }
    std::cout << "---------------------------------" << std::endl;
}

