#include "Reorder_buffer.h"

int total = 0;

Rob_Entry::Rob_Entry(const InstrRob &instr) {
    ready = instr.ready;
    type = instr.Rob_opt;
    value = instr.value;
    rd = instr.rd;
    other = instr.other;
    rs1 = instr.rs1;
    rs2 = instr.rs2;
}

void Rob::commit(bool &to_be_cleared) {
    if(buffer.isEmpty()) return;
    Rob_Entry entry = *buffer.begin();
    if(!entry.ready) return;
    buffer_next.pop();
    int actualValue;
    total++;
    switch (entry.type) {
        case RobType::jalr:
            reg->write(entry.rd, (buffer.head + 1) % ROB_SIZE, entry.other);
            rs->Rob_to_RS(entry.id, entry.other);
            programCounter->set_stop(false);
            programCounter->set_pc_next(entry.value);
            break;
        case RobType::reg:
            reg->write(entry.rd, (buffer.head + 1) % ROB_SIZE, entry.value);
            rs->Rob_to_RS(entry.rd, entry.value);
            break;
        case RobType::load:
            reg->write(entry.rd, (buffer.head + 1) % ROB_SIZE, entry.value);
            rs->Rob_to_RS(entry.id, entry.value);
            break;
        case RobType::store:
            actualValue = reg->registers[entry.rs2].value;
            lsb->Rob_to_lsb(entry.id, actualValue, entry.value);
            break;
        case RobType::branch:
            // value是 rs传回的值
            if(entry.other & 1){  //预测是跳转
                if(entry.value){   //应该跳转
                    predictor->state_up(entry.other);
                }
                else{
                    predictor->state_down(entry.other);
                    PC_next = entry.other + 3;
                    to_be_cleared = true;
                }
            }
            else { //预测不跳转
                if(entry.value){   //应该跳转
                    predictor->state_up(entry.other);
                    PC_next = entry.other;
                    to_be_cleared = true;
                }
                else{
                    predictor->state_down(entry.other);
                }
            }
            if(entry.value){
                PC_next = entry.value;
            }
            break;
        case RobType::exit:
            int x = reg->registers[10].value & 0xff;
            std::cout << x << std::endl;
            exit(0);
    }
}

void Rob::init(Reservation_Station *rs_in, LSB *lsb_in, RegisterFile *reg_in, Predictor *predictor_in, ProgramCounter *PC_in) {
    rs = rs_in;
    lsb = lsb_in;
    reg = reg_in;
    predictor = predictor_in;
    programCounter = PC_in;
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
void Rob::step(bool &to_be_cleared){
    commit(to_be_cleared);
}
void Rob::update_data(int Rob_id, int value) {
    buffer_next[Rob_id].value = value;
    buffer_next[Rob_id].ready = true;
}
void Rob::add(InstrRob instr) {
    Rob_Entry entry(instr);
    entry.id = get_tail_next();
    buffer_next.push(entry);
}
int Rob::get_tail_next() const {
    return (buffer.tail + 1) % ROB_SIZE;
}
void Rob::display() {
    std::cout << "--------ROB---------" << std::endl;
    int i = 0;
    for(auto it = buffer.begin(); it != buffer.end(); ++it){
        i ++;
        std::cout << "id:" << it->id << " ready:" << it->ready  << " type:" << (int)it->type << " value:" << it->value << " rd:" << it->rd << std::endl;
    }
    std::cout << "Rob_size:" << i << std::endl;
    int j = 0;
    for(auto it = buffer_next.begin(); it != buffer_next.end(); ++it){
        j++;
        std::cout << "id:" << it->id << " ready:" << it->ready  << " type:" << (int)it->type << " value:" << it->value << " rd:" << it->rd << std::endl;
    }
    std::cout << "Rob_next_size:" << j << std::endl;
    std::cout << "----------------------" << std::endl;
}


