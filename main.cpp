#include <iostream>
#include "src/memory.h"
#include "src/cpu.h"
#include <filesystem>

int main() {
    //备注：借鉴了 wankupi
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

    while(!cpu.finish()) {
        std::cout << "++++++++++++++ " << count << " ++++++++++++++" << std::endl;
        if(count == 8) {
            std::cout << "Here" << std::endl;
        }
        cpu.flush();
        cpu.execute();
        std::cout << "++++++++++++++++++++++++++++" << std::endl;
        count++;
    }
}
