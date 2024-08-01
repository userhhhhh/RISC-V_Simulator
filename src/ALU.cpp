#include "ALU.h"

void ALU::flush() {
    opt = opt_next;
    rs1 = rs1_next;
    rs2 = rs2_next;
    Rob_id = Rob_id_next;
    ready = ready_next;
}

void ALU::step() {}

void ALU::init() {}

void ALU::add(OptType opt, int rs1, int rs2, uint32_t Rob_id) {
    ready_next = true;
    opt_next = opt;
    rs1_next = rs1;
    rs2_next = rs2;
    Rob_id_next = Rob_id;
}

int ALU::get_Data() {
    return calculate(opt, rs1, rs2, Rob_id);
}

int ALU::calculate(OptType opt, uint32_t vj, uint32_t vk, uint32_t Rob_id) {
    int result;
    switch (opt) {
        case OptType::ADD:
            result = vj + vk;
            break;
        case OptType::SUB://
            result = vj - vk;
            break;
        case OptType::SLL://
            result = vj << (vk & 31);
            break;
        case OptType::SLT://
            result = vj < vk;
            break;
        case OptType::SLTU://
            result = (unsigned int) vj < (unsigned int) vk;
            break;
        case OptType::XOR://
            result = vj ^ vk;
            break;
        case OptType::SRL://
            result = (unsigned int) vj >> (vk & 31);
            break;
        case OptType::SRA://
            result = vj >> (vk & 31);
            break;
        case OptType::OR://
            result = vj | vk;
            break;
        case OptType::AND://
            result = vj & vk;
            break;

        case OptType::ADDI://
            result = vj + vk;
            break;
        case OptType::SLTI://
            result = vj < vk;
            break;
        case OptType::SLTIU://
            result = (unsigned int) vj < (unsigned int) vk;
            break;
        case OptType::XORI://
            result = vj ^ vk;
            break;
        case OptType::ORI://
            result = vj | vk;
            break;
        case OptType::ANDI://
            result = vj & vk;
            break;
        case OptType::SLLI://
            result = vj << vk;
            break;
        case OptType::SRLI://
            result = (unsigned int) vj >> (vk & 31);
            break;
        case OptType::SRAI://
            result = vj >> (vk & 31);
            break;

        case OptType::JALR://
            result = (vj + vk) & (-2);
            break;

        case OptType::BEQ://
            result = vj == vk;
            break;
        case OptType::BNE://
            result = vj != vk;
            break;
        case OptType::BLT://
            result = vj < vk;
            break;
        case OptType::BGE://
            result = vj >= vk;
            break;
        case OptType::BLTU://
            result = (unsigned int) vj < (unsigned int) vk;
            break;
        case OptType::BGEU://
            result = (unsigned int) vj >= (unsigned int) vk;
            break;

        case OptType::LB://
            result = vj + vk;
            break;
        case OptType::LH://
            result = vj + vk;
            break;
        case OptType::LW://
            result = vj + vk;
            break;
        case OptType::LBU://
            result = vj + vk;
            break;
        case OptType::LHU://
            result = vj + vk;
            break;

        case OptType::SB://
            result = vj + vk;
            break;
        case OptType::SH://
            result = vj + vk;
            break;
        case OptType::SW://
            result = vj + vk;
            break;
        default:
            break;
    }
    return result;
}

