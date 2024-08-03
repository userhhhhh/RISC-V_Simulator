#include <iostream>
#include "src/memory.h"
#include "src/cpu.h"
#include <filesystem>
#include "util/Naive_Simulator.h"

int main() {
     std::cout << "Current working directory: "
               << std::filesystem::current_path() << std::endl;
     freopen("/home/hqs123/class_code/RISC-V-Simulator-2024/sample/sample.data","r",stdin);
//     freopen("./testcases/superloop.data","r",stdin);
//     freopen("C:\\Users\\31447\\CLionProjects\\RISC5\\0.out","w",stdout);

    Memory mem;
    mem.init();
    CPU cpu;
    cpu.init(&mem);
    int count = 0;
//    Naive_Simulator sim;
//    sim.init(&mem);

    int ans = 0;
    while(!cpu.finish()){
        std::cout << "++++++++++++++ " << count << " ++++++++++++++" << std::endl;
        if(count == 20) {
            std::cout << "Here" << std::endl;
        }
        cpu.flush();
        cpu.execute();
        std::cout << "++++++++++++++++++++++++++++" << std::endl;
        count++;
//        ans = sim.status.regs.registers[10].value & 255;
    }
    std::cout << ans << std::endl;
}
