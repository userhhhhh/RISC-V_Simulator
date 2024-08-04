#include "decoder.h"

void Decoder::init(RegisterFile *reg_in, Rob *rob_in, Instruction *_instr_in, LSB *lsb_in, Reservation_Station *rs_in, ProgramCounter *programCounter_in, Predictor *predictor_in) {
    reg = reg_in;
    rob = rob_in;
    instr_in = _instr_in;
    lsb = lsb_in;
    rs = rs_in;
    programCounter = programCounter_in;
    predictor = predictor_in;
}
void Decoder::step() {
    execute(*instr_in);
}
void Decoder::execute(Instruction &instr) {
    Rob_flag = false;
    RS_flag = false;
    LSB_flag = false;

    switch (instr.instr_next.opt) {
        case OptType::LUI:
            instrRob.opt = OptType::LUI;
            instrRob.Rob_opt = RobType::reg;
            instrRob.rd = instr.instr_next.rd;
            instrRob.value = instr.instr_next.imm;
            instrRob.ready = true;
            instrRob.rs1 = instr.instr_next.rs1;
            instrRob.rs2 = instr.instr_next.rs2;
            Rob_flag = true;
            reg->update_independence(instrRob.rd, rob->get_tail_next());
            break;
        case OptType::AUIPC:
            instrRob.opt = OptType::AUIPC;
            instrRob.Rob_opt = RobType::reg;
            instrRob.rd = instr.instr_next.rd;
            instrRob.value = programCounter->pc + instr.instr_next.imm;
            instrRob.ready = true;
            instrRob.rs1 = instr.instr_next.rs1;
            instrRob.rs2 = instr.instr_next.rs2;
            reg->update_independence(instrRob.rd, rob->get_tail_next());
            Rob_flag = true;
            break;
        case OptType::JAL:
            instrRob.opt = OptType::JAL;
            instrRob.Rob_opt = RobType::reg;
            instrRob.rd = instr.instr_next.rd;
            instrRob.value = programCounter->pc + 4; //存储地址
            instrRob.rs1 = instr.instr_next.rs1;
            instrRob.rs2 = instr.instr_next.rs2;
            programCounter->set_pc_next(programCounter->pc + instr.instr_next.imm);
            reg->update_independence(instrRob.rd, rob->get_tail_next());
            instrRob.ready = true;
            Rob_flag = true;
            break;
        case OptType::JALR:
            instrRob.opt = OptType::JALR;
            instrRob.Rob_opt = RobType::jalr;
            instrRob.rd = instr.instr_next.rd;
            instrRob.rs1 = instr.instr_next.rs1;
            instrRob.rs2 = instr.instr_next.rs2;
            instrRob.other = programCounter->pc + 4;   //这个特殊，值存在 other里面，计算出的 pc地址为 value
            instrRob.ready = false;
            Rob_flag = true;

            int tmp_Ri;
            reg->get(instr.instr_next.rs1, instrRs.flag_Ri, tmp_Ri);
            if(instrRs.flag_Ri)  instrRs.Qi = tmp_Ri;
            else  instrRs.Ri = tmp_Ri;
            instrRs.Rj = instr.instr_next.imm;
            instrRs.flag_Rj = false;
            instrRs.opt = OptType::JALR;
            instrRs.Rob_id = rob->get_tail_next();
            RS_flag = true;

            reg->update_independence(instrRob.rd, rob->get_tail_next());
            programCounter->set_stop(true);
            break;
        case OptType::BEQ:
        case OptType::BNE:
        case OptType::BLT:
        case OptType::BGE:
        case OptType::BLTU:
        case OptType::BGEU:
            process(instr); break;

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

void Decoder::process(Instruction &instr){
    while(!rob->buffer.isEmpty() || !rs->isEmpty() || !lsb->buffer.isEmpty()){
        bool tmp = false;
        rob->step(tmp);
        rs->step();
        lsb->step();
        rob->flush();
        rs->flush();
        lsb->flush();
        reg->flush();
    }
    bool tmp;
    int value1, value2;
    reg->get(instr.instr_next.rs1, tmp, value1);
    reg->get(instr.instr_next.rs2, tmp, value2);
    switch(instr.instr_next.opt){
        case OptType::BEQ:
            if(value1 == value2){
                programCounter->set_pc_next(programCounter->pc + instr.instr_next.imm);
            }
            else{
                programCounter->set_pc_next(programCounter->pc + 4);
            }
            break;
        case OptType::BNE:
            if(value1 != value2){
                programCounter->set_pc_next(programCounter->pc + instr.instr_next.imm);
            }
            else{
                programCounter->set_pc_next(programCounter->pc + 4);
            }
            break;
        case OptType::BLT:
            if(value1 < value2){
                programCounter->set_pc_next(programCounter->pc + instr.instr_next.imm);
            }
            else{
                programCounter->set_pc_next(programCounter->pc + 4);
            }
            break;
        case OptType::BGE:
            if(value1 >= value2){
                programCounter->set_pc_next(programCounter->pc + instr.instr_next.imm);
            }
            else{
                programCounter->set_pc_next(programCounter->pc + 4);
            }
            break;
        case OptType::BLTU:
            if((unsigned)value1 < (unsigned)value2){
                programCounter->set_pc_next(programCounter->pc + instr.instr_next.imm);
            }
            else{
                programCounter->set_pc_next(programCounter->pc + 4);
            }
            break;
        case OptType::BGEU:
            if((unsigned)value1 >= (unsigned)value2){
                programCounter->set_pc_next(programCounter->pc + instr.instr_next.imm);
            }
            else{
                programCounter->set_pc_next(programCounter->pc + 4);
            }
            break;
        default: break;
    }
}


void Decoder::func_branch(Instruction &instr){

    instrRob.opt = instr.instr_next.opt;
    instrRob.Rob_opt = get_RobType(instr);
    instrRob.ready = false;
    instrRob.rs1 = instr.instr_next.rs1;
    instrRob.rs2 = instr.instr_next.rs2;
    instrRob.value = 0;
    Rob_flag = true;
    if(predictor->jump(programCounter->pc)){
        programCounter->set_pc_next(programCounter->pc + instr.instr_next.imm);
        instrRob.other = programCounter->pc + 1;
    }
    else{
        programCounter->set_pc_next(programCounter->pc + 4);
        instrRob.other = programCounter->pc + instr.instr_next.imm;
    }

    int tmp_Ri;
    reg->get(instr.instr_next.rs1, instrRs.flag_Ri, tmp_Ri);
    if(instrRs.flag_Ri)  instrRs.Qi = tmp_Ri;
    else  instrRs.Ri = tmp_Ri;
    int tmp_Rj;
    reg->get(instr.instr_next.rs2, instrRs.flag_Rj, tmp_Rj);
    if(instrRs.flag_Rj)  instrRs.Qj = tmp_Rj;
    else  instrRs.Rj = tmp_Rj;

    instrRs.opt = instr.instr_next.opt;
    instrRs.Rob_id = rob->get_tail_next();
    RS_flag = true;
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
    instrRob.opt = instr.instr_next.opt;
    instrRob.Rob_opt = get_RobType(instr);
    instrRob.rd = instr.instr_next.rd;
    instrRob.ready = false;
    instrRob.value = 0;
    instrRob.rs1 = instr.instr_next.rs1;
    instrRob.rs2 = instr.instr_next.rs2;
    Rob_flag = true;

    instrRs.opt = instr.instr_next.opt;
    get_Ri_Rs(instr);
    instrRs.Rj = instr.instr_next.imm;
    instrRs.flag_Rj = false;
    instrRs.Rob_id = rob->get_tail_next();
    if(instrRs.flag_Ri) instrRs.ready = false;
    else instrRs.ready = true;
    RS_flag = true;

    instrLsb.opt = get_LSType(instr);
    instrLsb.Rob_id = rob->get_tail_next();
    instrLsb.ready = false;
    LSB_flag = true;

    reg->update_independence(instrRob.rd, rob->get_tail_next());
}
void Decoder::func_store(Instruction &instr){
    instrRob.opt = instr.instr_next.opt;
    instrRob.Rob_opt = get_RobType(instr);
    instrRob.rd = instr.instr_next.rd;
    instrRob.rs1 = instr.instr_next.rs1;
    instrRob.rs2 = instr.instr_next.rs2;
    instrRob.ready = false;
    instrRob.value = 0;
    Rob_flag = true;

    instrRs.opt = instr.instr_next.opt;
    get_Ri_Rs(instr);
    instrRs.Rj = instr.instr_next.imm;
    instrRs.flag_Rj = false;
    instrRs.Rob_id = rob->get_tail_next();
    if(instrRs.flag_Ri) instrRs.ready = false;
    else instrRs.ready = true;
    RS_flag = true;

    instrLsb.opt = get_LSType(instr);
    instrLsb.Rob_id = rob->get_tail_next();
    instrLsb.ready = false;
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
    instrRob.opt = instr.instr_next.opt;
    instrRob.Rob_opt = get_RobType(instr);
    instrRob.rd = instr.instr_next.rd;
    instrRob.value = instr.instr_next.imm;
    instrRob.rs1 = instr.instr_next.rs1;
    instrRob.rs2 = instr.instr_next.rs2;
    instrRob.ready = false;
    Rob_flag = true;

    instrRs.opt = instr.instr_next.opt;
    get_Ri_Rs(instr);
    instrRs.Rj = instr.instr_next.imm;
    instrRs.flag_Rj = false;
    if(instrRs.flag_Ri) instrRs.ready = false;
    else instrRs.ready = true;
    instrRs.Rob_id = rob->get_tail_next();
    RS_flag = true;

    reg->update_independence(instrRob.rd, rob->get_tail_next());
}
void Decoder::func_cal(Instruction &instr){
    instrRob.opt = instr.instr_next.opt;
    instrRob.Rob_opt = get_RobType(instr);
    instrRob.rd = instr.instr_next.rd;
    instrRob.rs1 = instr.instr_next.rs1;
    instrRob.rs2 = instr.instr_next.rs2;
    instrRob.value = 0;
    instrRob.ready = false;
    Rob_flag = true;

    instrRs.opt = instr.instr_next.opt;
    get_Ri_Rs(instr);
    get_Rj_Rs(instr);
    if(instrRs.flag_Ri || instrRs.flag_Rj) instrRs.ready = false;
    else instrRs.ready = true;
    instrRs.Rob_id = rob->get_tail_next();
    RS_flag = true;

    reg->update_independence(instrRob.rd, rob->get_tail_next());
}
void Decoder::func_exit(Instruction &instr) {
    instrRob.opt = OptType::DELETE;
    Rob_flag = true;
}

void Decoder::flush() {}

void Decoder::display() {
    std::cout << "-------Decoder--------" << std::endl;
//    std::cout << "Ready_next:" << ready_next << std::endl;
    std::cout << "Rob_flag: " << Rob_flag << "    ";
    std::cout << "RS_flag: " << RS_flag << "    ";
    std::cout << "LSB_flag: " << LSB_flag << std::endl;
//    std::cout << "flag:" << flag << "    ";
//    std::cout << "pc:" << pc << std::endl;
//    std::cout << "flag_next:" << flag_next << "    ";
//    std::cout << "pc_next:" << pc_next << std::endl;
    std::cout << "----------------------" << std::endl;
}