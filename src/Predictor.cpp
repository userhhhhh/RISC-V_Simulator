#include "Predictor.h"

void Predictor::state_up(int dest) {
    int actual = dest >> 2;
    if(state[actual] == 4) state[actual] = 4;
    else state[actual]++;
    correct++;
}
void Predictor::state_down(int dest) {
    int actual = dest >> 2;
    if(state[actual] == 0) state[actual] = 0;
    else state[actual]--;
    incorrect++;
}
bool Predictor::jump(int dest) const {
    int actual = dest >> 2;
    if(state[actual] == 0 || state[actual] == 1) return false;
    else return true;
}
