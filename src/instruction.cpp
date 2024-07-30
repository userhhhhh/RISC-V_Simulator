#include "instruction.h"

void Instruction::init(Memory *mem_in) {
    mem = mem_in;
}
void Instruction::flush(){
    pc = pc_next;
    instr = instr_next;
}

void Instruction::decoder(Memory &mem, unsigned Rob_pc) {
    unsigned int pc;
    int instr_get = mem.fetch_32(pc);

}

void Instruction::decode(Instruction_unit &instr, int src) {
    switch (fragment(6, 0, src)) {
        case 0b0110111:
            instr.type = InstructionType::U;
            instr.opt = OptType::LUI;
            instr.rd = fragment(11, 7, src);
            instr.imm = fragment(31, 12, src) << 12;
            break;
        case 0b0010111:
            instr.type = InstructionType::U;
            instr.opt = OptType::AUIPC;
            instr.rd = fragment(11, 7, src);
            instr.imm = fragment(31, 12, src) << 12;
            break;
        case 0b1101111:
            instr.type = InstructionType::J;
            instr.opt = OptType::JAL;
            instr.rd = fragment(11, 7, src);
            instr.imm = num_extend(21, fragment(31, 31, src) << 20 + fragment(19, 12, src) << 12 + fragment(20, 20, src) << 11 + fragment(30, 21, src) << 1);
            break;
        case 0b1100111:
            instr.type = InstructionType::I;
            instr.opt = OptType::JALR;
            instr.rd = fragment(11, 7, src);
            instr.rs1 = fragment(19, 15, src);
            instr.imm = fragment(31, 20, src);
            break;
        case 0b1100011:
            instr.type = InstructionType::B;
            instr.rs1 = fragment(19, 15, src);
            instr.rs2 = fragment(24, 20, src);
            switch (fragment(14, 12, src)) {
                case 0b000: instr.opt = OptType::BEQ; break;
                case 0b001: instr.opt = OptType::BNE; break;
                case 0b100: instr.opt = OptType::BLT; break;
                case 0b101: instr.opt = OptType::BGE; break;
                case 0b110: instr.opt = OptType::BLTU; break;
                case 0b111: instr.opt = OptType::BGEU; break;
            }
            instr.imm = num_extend(13, fragment(31, 31, src) << 12 + fragment(7, 7, src) << 11 + fragment(30, 25, src) << 5 + fragment(11, 8, src) << 1);
            break;
        case 0b0000011:
            instr.type = InstructionType::I;
            instr.rs1 = fragment(19, 15, src);
            switch (fragment(14, 12, src)) {
                case 0b000: instr.opt = OptType::LB; break;
                case 0b001: instr.opt = OptType::LH; break;
                case 0b010: instr.opt = OptType::LW; break;
                case 0b100: instr.opt = OptType::LBU; break;
                case 0b101: instr.opt = OptType::LHU; break;
            }
            instr.rd = fragment(11, 7, src);
            instr.imm = fragment(31, 20, src);
            break;
        case 0b0100011:
            instr.type = InstructionType::S;
            instr.rs1 = fragment(19, 15, src);
            instr.rs2 = fragment(24, 20, src);
            switch (fragment(14, 12, src)) {
                case 0b000: instr.opt = OptType::SB; break;
                case 0b001: instr.opt = OptType::SH; break;
                case 0b010: instr.opt = OptType::SW; break;
            }
            instr.imm = num_extend(12, fragment(31, 25, src) << 5 + fragment(11, 7, src));
            break;
        case 0b0010011:
            instr.type = InstructionType::I;
            instr.rd = fragment(11, 7, src);
            instr.rs1 = fragment(19, 15, src);
            switch (fragment(14, 12, src)) {
                case 0b000: instr.opt = OptType::ADDI; break;
                case 0b010: instr.opt = OptType::SLTI; break;
                case 0b011: instr.opt = OptType::SLTIU; break;
                case 0b100: instr.opt = OptType::XORI; break;
                case 0b110: instr.opt = OptType::ORI; break;
                case 0b111: instr.opt = OptType::ANDI; break;
                case 0b001: instr.opt = OptType::SLLI; break;
                case 0b101:
                    if(fragment(30, 30, src) == 0) instr.opt = OptType::SRLI;
                    else instr.opt = OptType::SRAI;
                    break;
            }
            instr.imm = num_extend(12, fragment(31, 20, src));
            break;
        case 0b0110011:
            instr.type = InstructionType::R;
            instr.rs1 = fragment(19, 15, src);
            instr.rs2 = fragment(24, 20, src);
            instr.rd = fragment(11, 7, src);
            switch (fragment(14, 12, src)) {
                case 0b000:
                    if(fragment(30, 30, src) == 0) instr.opt = OptType::ADD;
                    else instr.opt = OptType::SUB;
                    break;
                case 0b001: instr.opt = OptType::SLL; break;
                case 0b010: instr.opt = OptType::SLT; break;
                case 0b011: instr.opt = OptType::SLTU; break;
                case 0b100: instr.opt = OptType::XOR; break;
                case 0b101:
                    if(fragment(30, 30, src) == 0) instr.opt = OptType::SRL;
                    else instr.opt = OptType::SRA;
                    break;
                case 0b110: instr.opt = OptType::OR; break;
                case 0b111: instr.opt = OptType::AND; break;
            }
            break;
    }
}

void Instruction::step() {

}

