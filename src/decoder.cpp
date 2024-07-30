#include "decoder.h"

void Decoder::init(RegisterFile *reg_in, Rob *rob_in, Instruction *_instr_in){
    reg = reg_in;
    rob = rob_in;
    instr_in = _instr_in;
}
void Decoder::step() {
    execute(*instr_in);
}
void Decoder::execute(Instruction &instr) {
    if(!instr.ready){
        return;
    }
    pc_next = -1;
    Rob_flag = false;
    RS_flag = false;
    LSB_flag = false;

    switch (instr.instr.opt) {
        case OptType::LUI:
            instrRob.opt = OptType::LUI;
            instrRob.rd = instr.instr.rd;
            instrRob.imm = instr.instr.imm;
            Rob_flag = true;
            break;
        case OptType::AUIPC:
            instrRob.opt = OptType::AUIPC;
            instrRob.rd = instr.instr.rd;
            instrRob.imm = instr.instr.imm;
            Rob_flag = true;
            break;
        case OptType::JAL:
            instrRob.opt = OptType::JAL;
            instrRob.rd = instr.instr.rd;
            instrRob.imm = instr.instr.imm;
            Rob_flag = true;
            flag_next = true;
            pc_next = instr.pc + instr.instr.imm;
            break;
        case OptType::JALR:
            instrRob.opt = OptType::JALR;
            instrRob.rd = instr.instr.rd;
            instrRob.rs1 = instr.instr.rs1;
            instrRob.imm = instr.instr.imm;
            Rob_flag = true;
            flag_next = true;
            pc_next = (instr.pc + instr.instr.imm) & 0xfffffffe;
            break;
        case OptType::BEQ:
            func_branch(instr);
            break;
        case OptType::BNE:
            func_branch(instr);
            break;
        case OptType::BLT:
            func_branch(instr);
            break;
        case OptType::BGE:
            func_branch(instr);
            break;
        case OptType::BLTU:
            func_branch(instr);
            break;
        case OptType::BGEU:
            func_branch(instr);
            break;
        case OptType::LB:
            func_load(instr);
            break;
        case OptType::LH:
            func_load(instr);
            break;
        case OptType::LW:
            func_load(instr);
            break;
        case OptType::LBU:
            func_load(instr);
            break;
        case OptType::LHU:
            func_load(instr);
            break;
        case OptType::SB:
            func_store(instr);
            break;
        case OptType::SH:
            func_store(instr);
            break;
        case OptType::SW:
            func_store(instr);
            break;
        case OptType::ADDI:
            func_cal_imm(instr);
            break;
        case OptType::SLTI:
            func_cal_imm(instr);
            break;
        case OptType::SLTIU:
            func_cal_imm(instr);
            break;
        case OptType::XORI:
            func_cal_imm(instr);
            break;
        case OptType::ORI:
            func_cal_imm(instr);
            break;
        case OptType::ANDI:
            func_cal_imm(instr);
            break;
        case OptType::SLLI:
            func_cal_imm(instr);
            break;
        case OptType::SRLI:
            func_cal_imm(instr);
            break;
        case OptType::SRAI:
            func_cal_imm(instr);
            break;
        case OptType::ADD:
            func_cal(instr);
            break;
        case OptType::SUB:
            func_cal(instr);
            break;
        case OptType::SLL:
            func_cal(instr);
            break;
        case OptType::SLT:
            func_cal(instr);
            break;
        case OptType::SLTU:
            func_cal(instr);
            break;
        case OptType::XOR:
            func_cal(instr);
            break;
        case OptType::SRL:
            func_cal(instr);
            break;
        case OptType::SRA:
            func_cal(instr);
            break;
        case OptType::OR:
            func_cal(instr);
            break;
        case OptType::AND:
            func_cal(instr);
            break;
        case OptType::DELETE:
            func_exit(instr);
            break;
    }
}
void Decoder::func_branch(Instruction &instr){
    instrRob.opt = instr.instr.opt;
    instrRob.Rob_opt = get_RobType(instr);
    instrRob.rs1 = instr.instr.rs1;
    instrRob.rs2 = instr.instr.rs2;
    instrRob.imm = instr.instr.imm;
    Rob_flag = true;
    instrLsb.opt = get_LSType(instr);
    get_Ri(instr);
    get_Rj(instr);
    instrLsb.result = 0;
    instrLsb.Rob_id = rob->get_tail_next();
    LSB_flag = true;
}
void Decoder::get_Ri(Instruction &instr){
    if(reg->registers[instr.instr.rs1].valid){
        instrLsb.flag_Ri = true;
        instrLsb.Qi = reg->registers[instr.instr.rs1].Rob_index;
    }
    else{
        instrLsb.flag_Ri = false;
        instrLsb.Ri = reg->registers[instr.instr.rs1].value;
    }
}
void Decoder::get_Rj(Instruction & instr){
    if(reg->registers[instr.instr.rs2].valid){
        instrLsb.flag_Rj = true;
        instrLsb.Qj = reg->registers[instr.instr.rs2].Rob_index;
    }
    else{
        instrLsb.flag_Rj = false;
        instrLsb.Rj = reg->registers[instr.instr.rs2].value;
    }
}
void Decoder::func_load(Instruction &instr){
    instrRob.opt = instr.instr.opt;
    instrRob.rd = instr.instr.rd;
    instrRob.rs1 = instr.instr.rs1;
    instrRob.imm = instr.instr.imm;
    Rob_flag = true;
    instrLsb.opt = get_LSType(instr);
    get_Ri(instr);
    instrLsb.result = 0;
    instrLsb.Rob_id = rob->get_tail_next();
    LSB_flag = true;
}
void Decoder::func_store(Instruction &instr){
    instrRob.opt = instr.instr.opt;
    instrRob.Rob_opt = get_RobType(instr);
    instrRob.rs1 = instr.instr.rs1;
    instrRob.rs2 = instr.instr.rs2;
    instrRob.imm = instr.instr.imm;
    Rob_flag = true;
    instrLsb.opt = get_LSType(instr);
    get_Ri(instr);
    get_Rj(instr);
    instrLsb.result = 0;
    instrLsb.Rob_id = rob->get_tail_next();
    LSB_flag = true;
}
LSType Decoder::get_LSType(Instruction &instr) {
    switch (instr.instr.opt) {
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
    switch (instr.instr.opt) {
        case OptType::LUI:
            return RobType::reg;
        case OptType::AUIPC:
            return RobType::reg;
        case OptType::JAL:
            return RobType::branch;
        case OptType::JALR:
            return RobType::branch;
        case OptType::BEQ:
            return RobType::branch;
        case OptType::BNE:
            return RobType::branch;
        case OptType::BLT:
            return RobType::branch;
        case OptType::BGE:
            return RobType::branch;
        case OptType::BLTU:
            return RobType::branch;
        case OptType::BGEU:
            return RobType::branch;
        case OptType::LB:
            return RobType::reg;
        case OptType::LH:
            return RobType::reg;
        case OptType::LW:
            return RobType::reg;
        case OptType::LBU:
            return RobType::reg;
        case OptType::LHU:
            return RobType::reg;
        case OptType::SB:
            return RobType::store;
        case OptType::SH:
            return RobType::store;
        case OptType::SW:
            return RobType::store;
        default:
            throw "Error: get_RobType error";
    }
}
void Decoder::func_cal_imm(Instruction &instr){
    instrRob.opt = instr.instr.opt;
    instrRob.Rob_opt = get_RobType(instr);
    instrRob.rs1 = instr.instr.rs1;
    instrRob.rs2 = instr.instr.rs2;
    instrRob.imm = instr.instr.imm;
    Rob_flag = true;
    instrRs.opt = instr.instr.opt;
    get_Ri(instr);
    get_Rj(instr);
    instrRs.rd = instr.instr.rd;
    instrRs.imm = instr.instr.imm;
    RS_flag = true;
}
void Decoder::func_cal(Instruction &instr){
    instrRob.opt = instr.instr.opt;
    instrRob.Rob_opt = get_RobType(instr);
    instrRob.rs1 = instr.instr.rs1;
    instrRob.rs2 = instr.instr.rs2;
    instrRob.rd = instr.instr.rd;
    Rob_flag = true;
    instrRs.opt = instr.instr.opt;
    get_Ri(instr);
    get_Rj(instr);
    instrRs.rd = instr.instr.rd;
    RS_flag = true;
}
void Decoder::func_exit(Instruction &instr) {
    instrRob.opt = OptType::DELETE;
    Rob_flag = true;
}
void Decoder::flush() {
    flag = flag_next;
    pc = pc_next;
}