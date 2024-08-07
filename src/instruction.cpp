#include "instruction.h"

void Instruction::init(Memory *mem_in, ProgramCounter *programCounter_in) {
    mem = mem_in;
    programCounter = programCounter_in;
}
void Instruction::flush(){
    instr = instr_next;
    ready = ready_next;
}

void Instruction::decoder() {
    uint32_t instr_get = mem->fetch_32(programCounter->pc);

    ready_next = true;
    decode(instr_get);
}

void Instruction::decode(uint32_t src) {
//    if(src == 0x0ff00513){
//        instr_next.opt = OptType::DELETE;
//        return;
//    }
    switch (fragment(6, 0, src)) {
        case 0b0110111:
            instr_next.type = InstructionType::U;
            instr_next.opt = OptType::LUI;
            instr_next.rd = fragment(11, 7, src);
            instr_next.imm = fragment(31, 12, src) << 12;
            break;
        case 0b0010111:
            instr_next.type = InstructionType::U;
            instr_next.opt = OptType::AUIPC;
            instr_next.rd = fragment(11, 7, src);
            instr_next.imm = fragment(31, 12, src) << 12;
            break;
        case 0b1101111:
            instr_next.type = InstructionType::J;
            instr_next.opt = OptType::JAL;
            instr_next.rd = fragment(11, 7, src);
            instr_next.imm = num_extend(21, (fragment(31, 31, src) << 20) + (fragment(19, 12, src) << 12) + (fragment(20, 20, src) << 11) + (fragment(30, 21, src) << 1));
            break;
        case 0b1100111:
            instr_next.type = InstructionType::I;
            instr_next.opt = OptType::JALR;
            instr_next.rd = fragment(11, 7, src);
            instr_next.rs1 = fragment(19, 15, src);
            instr_next.imm = fragment(31, 20, src);
            break;
        case 0b1100011:
            instr_next.type = InstructionType::B;
            instr_next.rs1 = fragment(19, 15, src);
            instr_next.rs2 = fragment(24, 20, src);
            switch (fragment(14, 12, src)) {
                case 0b000: instr_next.opt = OptType::BEQ; break;
                case 0b001: instr_next.opt = OptType::BNE; break;
                case 0b100: instr_next.opt = OptType::BLT; break;
                case 0b101: instr_next.opt = OptType::BGE; break;
                case 0b110: instr_next.opt = OptType::BLTU; break;
                case 0b111: instr_next.opt = OptType::BGEU; break;
            }
            instr_next.imm = num_extend(13, (fragment(31, 31, src) << 12) + (fragment(7, 7, src) << 11) + (fragment(30, 25, src) << 5) + (fragment(11, 8, src) << 1));
            break;
        case 0b0000011:
            instr_next.type = InstructionType::I;
            instr_next.rs1 = fragment(19, 15, src);
            switch (fragment(14, 12, src)) {
                case 0b000: instr_next.opt = OptType::LB; break;
                case 0b001: instr_next.opt = OptType::LH; break;
                case 0b010: instr_next.opt = OptType::LW; break;
                case 0b100: instr_next.opt = OptType::LBU; break;
                case 0b101: instr_next.opt = OptType::LHU; break;
            }
            instr_next.rd = fragment(11, 7, src);
            instr_next.imm = fragment(31, 20, src);
            break;
        case 0b0100011:
            instr_next.type = InstructionType::S;
            instr_next.rs1 = fragment(19, 15, src);
            instr_next.rs2 = fragment(24, 20, src);
            switch (fragment(14, 12, src)) {
                case 0b000: instr_next.opt = OptType::SB; break;
                case 0b001: instr_next.opt = OptType::SH; break;
                case 0b010: instr_next.opt = OptType::SW; break;
            }
            instr_next.imm = num_extend(12, (fragment(31, 25, src) << 5) + fragment(11, 7, src));
            break;
        case 0b0010011:
            instr_next.type = InstructionType::I;
            instr_next.rd = fragment(11, 7, src);
            instr_next.rs1 = fragment(19, 15, src);
            switch (fragment(14, 12, src)) {
                case 0b000: instr_next.opt = OptType::ADDI; break;
                case 0b010: instr_next.opt = OptType::SLTI; break;
                case 0b011: instr_next.opt = OptType::SLTIU; break;
                case 0b100: instr_next.opt = OptType::XORI; break;
                case 0b110: instr_next.opt = OptType::ORI; break;
                case 0b111: instr_next.opt = OptType::ANDI; break;
                case 0b001: instr_next.opt = OptType::SLLI; break;
                case 0b101:
                    if(fragment(30, 30, src) == 0) instr_next.opt = OptType::SRLI;
                    else instr_next.opt = OptType::SRAI;
                    break;
            }
            instr_next.imm = num_extend(12, fragment(31, 20, src));
            break;
        case 0b0110011:
            instr_next.type = InstructionType::R;
            instr_next.rs1 = fragment(19, 15, src);
            instr_next.rs2 = fragment(24, 20, src);
            instr_next.rd = fragment(11, 7, src);
            switch (fragment(14, 12, src)) {
                case 0b000:
                    if(fragment(30, 30, src) == 0) instr_next.opt = OptType::ADD;
                    else instr_next.opt = OptType::SUB;
                    break;
                case 0b001: instr_next.opt = OptType::SLL; break;
                case 0b010: instr_next.opt = OptType::SLT; break;
                case 0b011: instr_next.opt = OptType::SLTU; break;
                case 0b100: instr_next.opt = OptType::XOR; break;
                case 0b101:
                    if(fragment(30, 30, src) == 0) instr_next.opt = OptType::SRL;
                    else instr_next.opt = OptType::SRA;
                    break;
                case 0b110: instr_next.opt = OptType::OR; break;
                case 0b111: instr_next.opt = OptType::AND; break;
            }
            break;
    }
}

void Instruction::display() {
    std::cout << "-------Instruction--------" << std::endl;
//    std::cout << "pc:" << std::hex << pc << std::dec << "    ";
    std::cout << "instrAddr:" << std::hex << instr.instrAddr << std::dec << std::endl;
    std::cout << "opt:" << instr.opt << "    ";
    std::cout << "rs1:" << instr.rs1 << "    ";
    std::cout << "rs2:" << instr.rs2 << "    ";
    std::cout << "rd:" << instr.rd << "     ";
    std::cout << "imm:" << instr.imm << "    ";
    std::cout << "ready:" << ready << std::endl;

//    std::cout << "pc_next:" << std::hex << pc_next << std::dec << "    ";
    std::cout << "instrAddr_next:" << std::hex << instr_next.instrAddr << std::dec << std::endl;
    std::cout << "opt_next:" << instr_next.opt << "    ";
    std::cout << "rs1_next:" << instr_next.rs1 << "    ";
    std::cout << "rs2_next:" << instr_next.rs2 << "    ";
    std::cout << "rd_next:" << instr_next.rd << "     ";
    std::cout << "imm_next:" << instr_next.imm << "    ";
    std::cout << "ready_next:" << ready_next << std::endl;
    std::cout << "--------------------------" << std::endl;
}


