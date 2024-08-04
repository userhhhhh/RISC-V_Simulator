#include "cpu.h"

//#define DEBUG
//#define PC_STOP

void CPU::init(Memory *mem_in) {
    mem = mem_in;
    instr.init(mem, &programCounter);
    decoder.init(&reg, &rob, &instr, &lsb, &rs, &programCounter, &predictor);
    rob.init(&rs, &lsb, &reg, &predictor, &programCounter);
    rs.init(&rob, &lsb, &alu);
    lsb.init(&rob, &rs, &reg, mem);
    alu.init();
    reg.init(&rob);
}
void CPU::execute() {
#ifdef PC_STOP
    programCounter.display();
#endif
#ifdef DEBUG
    programCounter.display();
#endif
    if(!programCounter.stop && !rob.buffer.isFull() && !rs.isFull() && !lsb.buffer.isFull()){
        instr.decoder();
        decoder.step();
    }
    if(programCounter.stop){
        programCounter.stop = true;
    } else {
        if(rob.buffer.isFull() || rs.isFull() || lsb.buffer.isFull()){
            programCounter.wait = true;
        }
    }
#ifdef DEBUG
    instr.display();
    decoder.display();
#endif
    rob.step(to_be_cleared);
#ifdef DEBUG
    rob.display();
#endif
    rs.step();
#ifdef DEBUG
    rs.display();
#endif
    lsb.step();
#ifdef DEBUG
    lsb.display();
#endif
    alu.step();
    reg.execute(rob.flag_clear);
#ifdef DEBUG
    reg.display();
#endif
}
void CPU::flush() {
    if(!programCounter.stop){
        instr.flush();
        decoder.flush();
    }
    rob.flush();
    rs.flush();
    lsb.flush();
    alu.flush();
    reg.flush();
    if(!programCounter.wait){
        programCounter.flush();
    }
    if(programCounter.wait){
        programCounter.wait = false;
    }
}
bool CPU::finish() {
    return false;
}