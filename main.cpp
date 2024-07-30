#include <iostream>
#include "src/memory.h"
#include "src/cpu.h"
#include <filesystem>

int main() {
     std::cout << "Current working directory: "
               << std::filesystem::current_path() << std::endl;
     freopen("/home/hqs123/class_code/RISC-V-Simulator-2024/sample/sample.data","r",stdin);
//     freopen("./testcases/superloop.data","r",stdin);

    Memory mem;
    mem.init();
    CPU cpu;
    cpu.init(&mem);
    while(!cpu.finish()) {
        cpu.execute();
        cpu.execute();
    }
}
