// #include <iostream>
// #include "Naive_Simulator.h"

// void Naive_Simulator::init(Memory *mem_in) {
//     mem = mem_in;
//     status.instr.init(mem);
//     status_next.instr.init(mem);
// }
// void Naive_Simulator::flush() {
//     status.pc = status_next.pc;
//     status.halt = status_next.halt;
//     status.regs = status_next.regs;
// }
// void Naive_Simulator::step() {
//     status.instr.pc = status.pc;
//     status.instr.process();
//     if(status.instr.instr.opt == OptType::ADDI && status.instr.instr.rd==10 && status.instr.instr.rs1==0 && status.instr.instr.imm==255){
//         status_next.halt = true;
//         flush();
//         return;
//     }
//     status.instr.pc = status.pc;
//     status.instr.instrAddr = status.pc;
//     status_next.pc = status.pc + 4;
//     switch(status.instr.instr.opt){
//         case OptType::LUI:
//             status_next.regs.registers[status.instr.instr.rd].value = status.instr.instr.imm;
//             break;
//         case OptType::AUIPC:
//             status_next.regs.registers[status.instr.instr.rd].value = status.instr.instr.imm + status.instr.instrAddr;
//             break;
//         case OptType::JAL:
//             status_next.regs.registers[status.instr.instr.rd].value = status.pc + 4;
//             status_next.pc = status.instr.instr.imm + status.pc;
//             break;
//         case OptType::JALR:
//             status_next.regs.registers[status.instr.instr.rd].value = status.pc + 4;
//             status_next.pc = status.instr.instr.imm + status.regs.registers[status.instr.instr.rs1].value;
//             break;
//         case OptType::BEQ:
//             if(status.regs.registers[status.instr.instr.rs1].value == status.regs.registers[status.instr.instr.rs2].value){
//                 status_next.pc = status.pc + status.instr.instr.imm;
//             }
//             break;
//         case OptType::BNE:
//             if(status.regs.registers[status.instr.instr.rs1].value != status.regs.registers[status.instr.instr.rs2].value){
//                 status_next.pc = status.pc + status.instr.instr.imm;
//             }
//             break;
//         case OptType::BLT:
//             if((int)status.regs.registers[status.instr.instr.rs1].value < (int)status.regs.registers[status.instr.instr.rs2].value){
//                 status_next.pc = status.pc + status.instr.instr.imm;
//             }
//             break;
//         case OptType::BGE:
//             if((int)status.regs.registers[status.instr.instr.rs1].value >= (int)status.regs.registers[status.instr.instr.rs2].value){
//                 status_next.pc = status.pc + status.instr.instr.imm;
//             }
//             break;
//         case OptType::BLTU:
//             if(status.regs.registers[status.instr.instr.rs1].value < status.regs.registers[status.instr.instr.rs2].value){
//                 status_next.pc = status.pc + status.instr.instr.imm;
//             }
//             break;
//         case OptType::BGEU:
//             if(status.regs.registers[status.instr.instr.rs1].value >= status.regs.registers[status.instr.instr.rs2].value){
//                 status_next.pc = status.pc + status.instr.instr.imm;
//             }
//             break;
//         case OptType::LB:
//             status_next.regs.registers[status.instr.instr.rd].value =
//                     mem->load_memory(status.regs.registers[status.instr.instr.rs1].value + status.instr.instr.imm, 1, true);
//             break;
//         case OptType::LH:
//             status_next.regs.registers[status.instr.instr.rd].value =
//                     mem->load_memory(status.regs.registers[status.instr.instr.rs1].value + status.instr.instr.imm, 2, true);
//             break;
//         case OptType::LW:
//             status_next.regs.registers[status.instr.instr.rd].value =
//                     mem->load_memory(status.regs.registers[status.instr.instr.rs1].value + status.instr.instr.imm, 4, true);
//             break;
//         case OptType::LBU:
//             status_next.regs.registers[status.instr.instr.rd].value =
//                     mem->load_memory(status.regs.registers[status.instr.instr.rs1].value + status.instr.instr.imm, 1, false);
//             break;
//         case OptType::LHU:
//             status_next.regs.registers[status.instr.instr.rd].value =
//                     mem->load_memory(status.regs.registers[status.instr.instr.rs1].value + status.instr.instr.imm, 2, false);
//             break;
//         case OptType::SB:
//             mem->store_memory(status.regs.registers[status.instr.instr.rs1].value + status.instr.instr.imm, status.regs.registers[status.instr.instr.rs2].value, 1);
//             break;
//         case OptType::SH:
//             mem->store_memory(status.regs.registers[status.instr.instr.rs1].value + status.instr.instr.imm, status.regs.registers[status.instr.instr.rs2].value, 2);
//             break;
//         case OptType::SW:
//             mem->store_memory(status.regs.registers[status.instr.instr.rs1].value + status.instr.instr.imm, status.regs.registers[status.instr.instr.rs2].value, 4);
//             break;
//         case OptType::ADDI:
//             status_next.regs.registers[status.instr.instr.rd].value = status.regs.registers[status.instr.instr.rs1].value + status.instr.instr.imm;
//             break;
//         case OptType::SLTI:
//             status_next.regs.registers[status.instr.instr.rd].value = ((int)status.regs.registers[status.instr.instr.rs1].value < (int)status.instr.instr.imm) ? 1 : 0;
//             break;
//         case OptType::SLTIU:
//             status_next.regs.registers[status.instr.instr.rd].value = ((uint32_t)status.regs.registers[status.instr.instr.rs1].value < (uint32_t)status.instr.instr.imm) ? 1 : 0;
//             break;
//         case OptType::XORI:
//             status_next.regs.registers[status.instr.instr.rd].value = status.regs.registers[status.instr.instr.rs1].value ^ status.instr.instr.imm;
//             break;
//         case OptType::ORI:
//             status_next.regs.registers[status.instr.instr.rd].value = status.regs.registers[status.instr.instr.rs1].value | status.instr.instr.imm;
//             break;
//         case OptType::ANDI:
//             status_next.regs.registers[status.instr.instr.rd].value = status.regs.registers[status.instr.instr.rs1].value & status.instr.instr.imm;
//             break;
//         case OptType::SLLI:
//             status_next.regs.registers[status.instr.instr.rd].value = status.regs.registers[status.instr.instr.rs1].value << status.instr.instr.imm;
//             break;
//         case OptType::SRLI:
//             status_next.regs.registers[status.instr.instr.rd].value = status.regs.registers[status.instr.instr.rs1].value >> status.instr.instr.imm;
//             break;
//         case OptType::SRAI:
//             status_next.regs.registers[status.instr.instr.rd].value = ((int)status.regs.registers[status.instr.instr.rs1].value) >> status.instr.instr.imm;
//             break;
//         case OptType::ADD:
//             status_next.regs.registers[status.instr.instr.rd].value = status.regs.registers[status.instr.instr.rs1].value + status.regs.registers[status.instr.instr.rs2].value;
//             break;
//         case OptType::SUB:
//             status_next.regs.registers[status.instr.instr.rd].value = status.regs.registers[status.instr.instr.rs1].value - status.regs.registers[status.instr.instr.rs2].value;
//             break;
//         case OptType::SLL:
//             status_next.regs.registers[status.instr.instr.rd].value = status.regs.registers[status.instr.instr.rs1].value << status.regs.registers[status.instr.instr.rs2].value;
//             break;
//         case OptType::SLT:
//             status_next.regs.registers[status.instr.instr.rd].value = ((int)status.regs.registers[status.instr.instr.rs1].value < (int)status.regs.registers[status.instr.instr.rs2].value) ? 1 : 0;
//             break;
//         case OptType::SLTU:
//             status_next.regs.registers[status.instr.instr.rd].value = ((uint32_t)status.regs.registers[status.instr.instr.rs1].value < (uint32_t)status.regs.registers[status.instr.instr.rs2].value) ? 1 : 0;
//             break;
//         case OptType::XOR:
//             status_next.regs.registers[status.instr.instr.rd].value = (status.regs.registers[status.instr.instr.rs1].value ^ status.regs.registers[status.instr.instr.rs2].value);
//             break;
//         case OptType::SRL:
//             status_next.regs.registers[status.instr.instr.rd].value = status.regs.registers[status.instr.instr.rs1].value >> status.regs.registers[status.instr.instr.rs2].value;
//             break;
//         case OptType::SRA:
//             status_next.regs.registers[status.instr.instr.rd].value = ((int)status.regs.registers[status.instr.instr.rs1].value) >> status.regs.registers[status.instr.instr.rs2].value;
//             break;
//         case OptType::OR:
//             status_next.regs.registers[status.instr.instr.rd].value = (status.regs.registers[status.instr.instr.rs1].value | status.regs.registers[status.instr.instr.rs2].value);
//             break;
//         case OptType::AND:
//             status_next.regs.registers[status.instr.instr.rd].value = (status.regs.registers[status.instr.instr.rs1].value & status.regs.registers[status.instr.instr.rs2].value);
//             break;

//         default:
//             break;
//     }
//     status_next.regs.registers[0].value = 0;
//     status.pc = status_next.pc;
//     status.halt = status_next.halt;
//     status.regs = status_next.regs;
// }


