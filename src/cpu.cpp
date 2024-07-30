#include "cpu.h"

void CPU::init(Memory *mem_in) {
    mem = mem_in;
    instr.init(mem);
    decoder.init(&reg, &rob, &instr);
    rob.init(&rs, &lsb, &reg);
    rs.init(&rob, &lsb, &alu);
    lsb.init(&rob, &rs, &reg);
    alu.init();
    reg.init(&rob);
}
void CPU::execute() {
    instr.step();
    decoder.step();
    rob.step();
    rs.step();
    lsb.step();
    alu.step();
    reg.execute(rob.flag_clear);
}
void CPU::flush() {
    instr.flush();
    decoder.flush();
    rob.flush();
    rs.flush();
    lsb.flush();
    alu.flush();
    reg.flush();
}
bool CPU::finish() {
    return false;
}