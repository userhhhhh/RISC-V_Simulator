#ifndef RISC5_MEMORY_H
#define RISC5_MEMORY_H

#include <iostream>
#include "../util/config.h"
#include "../util/utils.h"

class Memory {
private:
    uint8_t ram[MEMORY_SIZE];
public:
    Memory() = default;
    ~Memory() = default;
    void init();
    uint32_t fetch_32(int pos);
    void store_memory(int addr, int source, int byte_number);
    int load_memory(int addr, int byte_number, bool sign_extended);
};

#endif //RISC5_MEMORY_H
