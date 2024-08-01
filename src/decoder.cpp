#include "decoder.h"

void Decoder::init(RegisterFile *reg_in, Rob *rob_in, Instruction *_instr_in, LSB *lsb_in, Reservation_Station *rs_in) {
    reg = reg_in;
    rob = rob_in;
    instr_in = _instr_in;
    lsb = lsb_in;
    rs = rs_in;
    ready_next = true;
}
void Decoder::step() {
    execute(*instr_in);
}
void Decoder::execute(Instruction &instr) {
    if(instr.instr_next.opt == OptType::SW){
        std::cout << "hhhh" << std::endl;
    }
    ready_next = true;
    flag_next = false;

    pc_next = -1;
    Rob_flag = false;
    RS_flag = false;
    LSB_flag = false;

    switch (instr.instr_next.opt) {
        case OptType::LUI:
            if(rob->buffer.isFull()){
                ready_next = false;
                break;
            }
            instrRob.opt = OptType::LUI;
            instrRob.Rob_opt = RobType::reg;
            instrRob.rd = instr.instr_next.rd;
            instrRob.value = instr.instr_next.imm;
            instrRob.pc_addr = instr.instrAddr_next;
            instrRob.ready = true;
            Rob_flag = true;
            break;
        case OptType::AUIPC:
            if(rob->buffer.isFull()){
                ready_next = false;
                break;
            }
            instrRob.opt = OptType::AUIPC;
            instrRob.Rob_opt = RobType::reg;
            instrRob.rd = instr.instr_next.rd;
            instrRob.value = instr.instrAddr_next + instr.instr_next.imm;
            instrRob.pc_addr = instr.instrAddr_next;
            instrRob.ready = true;
            Rob_flag = true;
            break;
        case OptType::JAL:
            if(rob->buffer.isFull()){
                ready_next = false;
                break;
            }
            instrRob.opt = OptType::JAL;
            instrRob.Rob_opt = RobType::reg;
            instrRob.rd = instr.instr_next.rd;
            // TODO: not sure
            instrRob.value = instr.instrAddr_next + 4; //存储地址
            instrRob.pc_addr = instr.instrAddr_next;
            instrRob.ready = true;
            Rob_flag = true;
            flag_next = true;
            pc_next = (int) instr.instrAddr_next + instr.instr_next.imm;
            break;
        case OptType::JALR:
            // TODO: dependence
            if(rob->buffer.isFull()){
                ready_next = false;
                break;
            }
            instrRob.opt = OptType::JALR;
            instrRob.Rob_opt = RobType::reg;
            instrRob.rd = instr.instr_next.rd;
            // TODO
            instrRob.value = instr.instrAddr_next + 4;
            instrRob.pc_addr = instr.instrAddr_next;
            instrRob.ready = true;
            Rob_flag = true;
            flag_next = true;
            pc_next = (int) ((reg->registers[instr.instr_next.rs1].value + instr.instr_next.imm) & 0xfffffffe);
            break;
        case OptType::BEQ:
        case OptType::BNE:
        case OptType::BLT:
        case OptType::BGE:
        case OptType::BLTU:
        case OptType::BGEU: func_branch(instr); break;

        case OptType::LB:
        case OptType::LH:
        case OptType::LW:
        case OptType::LBU:
        case OptType::LHU: func_load(instr); break;

        case OptType::SB:
        case OptType::SH:
        case OptType::SW: func_store(instr); break;

        case OptType::ADDI:
        case OptType::SLTI:
        case OptType::SLTIU:
        case OptType::XORI:
        case OptType::ORI:
        case OptType::ANDI:
        case OptType::SLLI:
        case OptType::SRLI:
        case OptType::SRAI: func_cal_imm(instr); break;

        case OptType::ADD:
        case OptType::SUB:
        case OptType::SLL:
        case OptType::SLT:
        case OptType::SLTU:
        case OptType::XOR:
        case OptType::SRL:
        case OptType::SRA:
        case OptType::OR:
        case OptType::AND: func_cal(instr); break;

        case OptType::DELETE: func_exit(instr);break;
    }

    RS_Data rsData = rs->get_data();
    LSB_Data lsbData = lsb->get_data();

    if(instrRs.flag_Ri){
        if(rob->buffer[(int) instrRs.Qi].ready){
            instrRs.Ri = rob->buffer[(int) instrRs.Qi].value;
            instrRs.flag_Ri = false;
        }
        if(rsData.ready && instrRs.Qi == rsData.Rob_id){
            instrRs.Ri = rsData.value;
            instrRs.flag_Ri = false;
        }
        if(lsbData.ready && instrRs.Qi == lsbData.Rob_id){
            instrRs.Ri = lsbData.value;
            instrRs.flag_Ri = false;
        }
    }
    if(instrRs.flag_Rj){
        if(rob->buffer[(int) instrRs.Qj].ready){
            instrRs.Rj = rob->buffer[(int) instrRs.Qj].value;
            instrRs.flag_Rj = false;
        }
        if(rsData.ready && instrRs.Qj == rsData.Rob_id){
            instrRs.Rj = rsData.value;
            instrRs.flag_Rj = false;
        }
        if(lsbData.ready && instrRs.Qj == lsbData.Rob_id){
            instrRs.Rj = lsbData.value;
            instrRs.flag_Rj = false;
        }
    }
    if(instrLsb.flag_Ri){
        if(rob->buffer[(int) instrLsb.Qi].ready){
            instrLsb.Ri = rob->buffer[(int) instrLsb.Qi].value;
            instrLsb.flag_Ri = false;
        }
        if(rsData.ready && instrLsb.Qi == rsData.Rob_id){
            instrLsb.Ri = rsData.value;
            instrLsb.flag_Ri = false;
        }
        if(lsbData.ready && instrLsb.Qi == lsbData.Rob_id){
            instrLsb.Ri = lsbData.value;
            instrLsb.flag_Ri = false;
        }
    }
    if(instrLsb.flag_Rj){
        if(rob->buffer[(int) instrLsb.Qj].ready){
            instrLsb.Rj = rob->buffer[(int) instrLsb.Qj].value;
            instrLsb.flag_Rj = false;
        }
        if(rsData.ready && instrLsb.Qj == rsData.Rob_id){
            instrLsb.Rj = rsData.value;
            instrLsb.flag_Rj = false;
        }
        if(lsbData.ready && instrLsb.Qj == lsbData.Rob_id){
            instrLsb.Rj = lsbData.value;
            instrLsb.flag_Rj = false;
        }
    }

    if(Rob_flag){
        rob->add(instrRob);
    }
    if(RS_flag){
        rs->add(instrRs);
    }
    if(LSB_flag){
        lsb->add(instrLsb);
    }
}

void Decoder::func_branch(Instruction &instr){
    // TODO
    if(rob->buffer.isFull()){
        ready_next = false;
        return;
    }
    instrRob.opt = instr.instr_next.opt;
    instrRob.Rob_opt = get_RobType(instr);
    instrRob.rd = instr.instr_next.rd;
    instrRob.pc_addr = instr.instrAddr_next;
    instrRob.ready = false;
    instrRob.value = 0;
    Rob_flag = true;
    instrLsb.opt = get_LSType(instr);
    get_Ri(instr);
    get_Rj(instr);
    instrLsb.result = 0;
    instrLsb.Rob_id = rob->get_tail_next();
    LSB_flag = true;
}
void Decoder::get_Ri(Instruction &instr){
    if(reg->registers[instr.instr_next.rs1].Is_dependent){
        instrLsb.flag_Ri = true;
        instrLsb.Qi = reg->registers[instr.instr_next.rs1].Rob_index;
    }
    else{
        instrLsb.flag_Ri = false;
        instrLsb.Ri = reg->registers[instr.instr_next.rs1].value;
    }
}
void Decoder::get_Ri_Rs(Instruction &instr){
    if(reg->registers[instr.instr_next.rs1].Is_dependent) {
        instrRs.flag_Ri = true;
        instrRs.Qi = reg->registers[instr.instr_next.rs1].Rob_index;
    }
    else {
        instrRs.flag_Ri = false;
        instrRs.Ri = reg->registers[instr.instr_next.rs1].value;
    }
}
void Decoder::get_Rj(Instruction & instr){
    if(reg->registers[instr.instr_next.rs2].Is_dependent){
        instrLsb.flag_Rj = true;
        instrLsb.Qj = reg->registers[instr.instr_next.rs2].Rob_index;
    }
    else{
        instrLsb.flag_Rj = false;
        instrLsb.Rj = reg->registers[instr.instr_next.rs2].value;
    }
}
void Decoder::get_Rj_Rs(Instruction & instr){
    if(reg->registers[instr.instr_next.rs2].Is_dependent){
        instrRs.flag_Rj = true;
        instrRs.Qj = reg->registers[instr.instr_next.rs2].Rob_index;
    }
    else{
        instrRs.flag_Rj = false;
        instrRs.Rj = reg->registers[instr.instr_next.rs2].value;
    }
}
void Decoder::func_load(Instruction &instr){
    if(rob->buffer.isFull() || lsb->buffer.isFull()){
        ready_next = false;
        return;
    }
    instrRob.opt = instr.instr_next.opt;
    instrRob.Rob_opt = get_RobType(instr);
    instrRob.rd = instr.instr_next.rd;
    instrRob.ready = false;
    instrRob.value = 0;
    Rob_flag = true;
    instrLsb.opt = get_LSType(instr);
    get_Ri(instr);
    instrLsb.result = 0;
    instrLsb.Rob_id = rob->get_tail_next();
    LSB_flag = true;
}
void Decoder::func_store(Instruction &instr){
    if(rob->buffer.isFull() || lsb->buffer.isFull()){
        ready_next = false;
        return;
    }
    instrRob.opt = instr.instr_next.opt;
    instrRob.Rob_opt = get_RobType(instr);
    instrRob.rd = instr.instr_next.rd;
    instrRob.pc_addr = instr.instrAddr_next;
    instrRob.ready = false;
    instrRob.value = 0;
    Rob_flag = true;

    instrLsb.opt = get_LSType(instr);
    get_Ri(instr);
    get_Rj(instr);
    instrLsb.result = 0;
    instrLsb.offset = instr.instr_next.imm;
    instrLsb.Rob_id = rob->get_tail_next();
    LSB_flag = true;
}
LSType Decoder::get_LSType(Instruction &instr) {
    switch (instr.instr_next.opt) {
        case OptType::LB:
            return LSType::LB;
        case OptType::LH:
            return LSType::LH;
        case OptType::LW:
            return LSType::LW;
        case OptType::LBU:
            return LSType::LBU;
        case OptType::LHU:
            return LSType::LHU;
        case OptType::SB:
            return LSType::SB;
        case OptType::SH:
            return LSType::SH;
        case OptType::SW:
            return LSType::SW;
        default:
            throw "Error: get_LSType error";
    }
}
RobType Decoder::get_RobType(Instruction &instr) {
    switch (instr.instr_next.opt) {
        case OptType::LUI:
        case OptType::AUIPC:
        case OptType::JAL:
        case OptType::JALR: return RobType::reg;

        case OptType::BEQ:
        case OptType::BNE:
        case OptType::BLT:
        case OptType::BGE:
        case OptType::BLTU:
        case OptType::BGEU: return RobType::branch;

        case OptType::LB:
        case OptType::LH:
        case OptType::LW:
        case OptType::LBU:
        case OptType::LHU:

        case OptType::ADDI:
        case OptType::SLTI:
        case OptType::SLTIU:
        case OptType::XORI:
        case OptType::ORI:
        case OptType::ANDI:
        case OptType::SLLI:
        case OptType::SRLI:
        case OptType::SRAI:

        case OptType::ADD:
        case OptType::SUB:
        case OptType::SLL:
        case OptType::SLT:
        case OptType::SLTU:
        case OptType::XOR:
        case OptType::SRL:
        case OptType::SRA:
        case OptType::OR:
        case OptType::AND: return RobType::reg;

        case OptType::SB:
        case OptType::SH:
        case OptType::SW: return RobType::store;

        case OptType::DELETE: return RobType::exit;

        default: throw "Error: get_RobType error";
    }
}
void Decoder::func_cal_imm(Instruction &instr){
    if(rob->buffer.isFull() || rs->isFull()){
        ready_next = false;
        return;
    }
    instrRob.opt = instr.instr_next.opt;
    instrRob.Rob_opt = get_RobType(instr);
    instrRob.rd = instr.instr_next.rd;
    instrRob.value = instr.instr_next.imm;
    instrRob.pc_addr = instr.instrAddr_next;
    instrRob.ready = false;
    Rob_flag = true;
    instrRs.opt = instr.instr_next.opt;
    get_Ri_Rs(instr);
    instrRs.Rj = instr.instr_next.imm;
    instrRs.flag_Rj = false;
    RS_flag = true;
}
void Decoder::func_cal(Instruction &instr){
    if(rob->buffer.isFull() || rs->isFull()){
        ready_next = false;
        return;
    }
    instrRob.opt = instr.instr_next.opt;
    instrRob.Rob_opt = get_RobType(instr);
    instrRob.rd = instr.instr_next.rd;
    instrRob.value = 0;
    instrRob.pc_addr = instr.instrAddr_next;
    instrRob.ready = false;
    Rob_flag = true;
    instrRs.opt = instr.instr_next.opt;
    get_Ri_Rs(instr);
    get_Rj_Rs(instr);
    RS_flag = true;
}
void Decoder::func_exit(Instruction &instr) {
    // TODO
    instrRob.opt = OptType::DELETE;
    Rob_flag = true;
}
void Decoder::flush() {
    if(!ready_next) return;
    flag = flag_next;
    pc = pc_next;
}
void Decoder::display() {
    std::cout << "-------Decoder--------" << std::endl;
    std::cout << "Ready_next:" << ready_next << std::endl;
    std::cout << "Rob_flag: " << Rob_flag << "    ";
    std::cout << "RS_flag: " << RS_flag << "    ";
    std::cout << "LSB_flag: " << LSB_flag << std::endl;
    std::cout << "flag:" << flag << "    ";
    std::cout << "pc:" << pc << std::endl;
    std::cout << "flag_next:" << flag_next << "    ";
    std::cout << "pc_next:" << pc_next << std::endl;
    std::cout << "----------------------" << std::endl;
}