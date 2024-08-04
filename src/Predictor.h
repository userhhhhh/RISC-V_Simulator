#ifndef RISC5_PREDICTOR_H
#define RISC5_PREDICTOR_H

#include "../util/config.h"

class Predictor {
public:
    int state[PREDICTOR_SIZE];
    int correct;
    int incorrect;
public:
    void state_up(int dest);
    void state_down(int dest);
    bool jump(int dest) const;
};

#endif //RISC5_PREDICTOR_H
