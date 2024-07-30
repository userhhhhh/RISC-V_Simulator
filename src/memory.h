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
};

#endif //RISC5_MEMORY_H
