#include <iostream>
#include "src/memory.h"
#include "src/cpu.h"
#include <filesystem>
#include "util/Naive_Simulator.h"

//#define DEBUG1
int main() {
//     std::cout << "Current working directory: "
//               << std::filesystem::current_path() << std::endl;
//     freopen("","r",stdin);
//     freopen("/home/hqs123/class_code/RISC-V-Simulator-2024/sample/sample.data","r",stdin);
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
#ifdef DEBUG1
        std::cout << "++++++++++++++ " << count << " ++++++++++++++" << std::endl;
        std::cout << "------- " << total << " ---------" << std::endl;
        if(count == 200) {
            std::cout << "Here" << std::endl;
        }
#endif
        cpu.flush();
        cpu.execute();
#ifdef DEBUG1
        std::cout << "++++++++++++++++++++++++++++" << std::endl;
#endif
        count++;
//        ans = sim.status.regs.registers[10].value & 255;
    }
//    std::cout << ans << std::endl;
}
