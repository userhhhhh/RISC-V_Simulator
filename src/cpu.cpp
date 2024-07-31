#include "cpu.h"

#define DEBUG

void CPU::init(Memory *mem_in) {
    mem = mem_in;
    instr.init(mem);
    decoder.init(&reg, &rob, &instr, &lsb, &rs);
    rob.init(&rs, &lsb, &reg);
    rs.init(&rob, &lsb, &alu);
    lsb.init(&rob, &rs, &reg);
    alu.init();
    reg.init(&rob);
}
void CPU::execute() {
    instr.decoder(decoder.ready_next, rob.PC, decoder.pc, rob.flag_clear, decoder.flag);
#ifdef DEBUG
    instr.display();
#endif
    decoder.step();
#ifdef DEBUG
    decoder.display();
#endif
    rob.step();
#ifdef DEBUG
    rob.display();
#endif
    rs.step();
    lsb.step();
#ifdef DEBUG
//    lsb.display();
#endif
    alu.step();
    reg.execute(rob.flag_clear);
#ifdef DEBUG
    reg.display();
#endif
}
void CPU::flush() {
    instr.flush(decoder.ready_next);
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