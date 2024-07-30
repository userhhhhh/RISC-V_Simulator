#include "utils.h"

int fragment(int start, int end, int num){
    num = num >> end;
    int mask = (1 << (start - end + 1)) - 1;
    num = num & mask;
    return num;
}

int num_extend(int len, int num) {
    if (len == 32) return num;
    if (num >> (len - 1)) {
        int mask = -(1 << len);
        num = num | mask;
    }
    return num;
}

uint32_t Hex_To_Int(const std::string &str){
    uint32_t num = 0;
    for (int i = 0; i < str.size(); i++) {
        num = num << 4;
        if (str[i] >= '0' && str[i] <= '9') {
            num += str[i] - '0';
        } else if (str[i] >= 'A' && str[i] <= 'F') {
            num += str[i] - 'A' + 10;
        } else {
            std::cout << "error" << std::endl;
        }
    }
    return num;
}
