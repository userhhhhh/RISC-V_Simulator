#include "memory.h"

//#define SHOW
void Memory::init() {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        ram[i] = 0;
    }
    std::string str;
    uint32_t addr;
    while(std::cin >> str){
        if(str[0] == '@'){
            str = str.substr(1);
            addr = Hex_To_Int(str.substr(1, 8));
        } else {
            if(str.size() != 2){
                throw "Invalid input";
            }
            ram[addr] = Hex_To_Int(str);
            addr++;
        }
    }
}
uint32_t Memory::fetch_32(int pos) {
    if (pos < 0 || pos + 3 >= MEMORY_SIZE) {
        throw "Memory out of bound";
    }
    return ram[pos] | (ram[pos + 1] << 8) | (ram[pos + 2] << 16) | (ram[pos + 3] << 24);
}
void Memory::store_memory(int addr, int source, int byte_number) {
#ifdef SHOW
    std::cout << "Memory store: " << source << " at " << addr << std::endl;
#endif
    for (int i = 0; i < byte_number; ++i) {
        ram[addr + i] = source & 0b11111111;
        source = source >> 8;
    }
}
int Memory::load_memory(int addr, int byte_number, bool sign_extended) {
    int out = 0;

    for (int i = byte_number - 1; i >= 0; --i) {
        out = out << 8;
        out = out | int(ram[addr + i]);
    }
    int ans;
    if(sign_extended) ans = num_extend(byte_number << 3, out);
    else ans = out;
#ifdef SHOW
    std::cout << "Memory load: " << ans << " at " << addr << std::endl;
#endif
    return ans;
}
