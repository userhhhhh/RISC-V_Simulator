#include "CircularQueue.h"
#include "../src/Reorder_buffer.h"

template<typename type, int size>
queue<type, size>::queue() {
    head = -1;
    tail = -1;
    for(int i = 0; i < size; i++) {
        data[i] = type();
    }
}

template<typename type, int size>
bool queue<type, size>::isEmpty() {
    return head == tail;
}

template<typename type, int size>
bool queue<type, size>::isFull() {
    if((head - tail + size) % size == 1) {
        return true;
    }
    return false;
}

template<typename type, int size>
int queue<type, size>::getSize() {
    return (tail - head + size) % size;
}

template<typename type, int size>
void queue<type, size>::push(type x) {
    if (isFull()) {

        return;
    }
    tail = (tail + 1) % size;
    data[tail] = x;
}

template<typename type, int size>
type queue<type, size>::pop() {
    if (isEmpty()) {
//        std::cout << "Error: use pop() while queue is empty" << std::endl;
        return data[0];
    }
    head = (head + 1) % size;
    return data[head];
}

template<typename type, int size>
type queue<type, size>::front() {
    if (isEmpty()) {
//        std::cout << "Error: use front() while queue is empty" << std::endl;
        return data[0];
    }
    return data[(head + 1) % size];
}

template<typename type, int size>
type queue<type, size>::back() {
    if (isEmpty()) {
//        std::cout << "Error: use back() while queue is empty" << std::endl;
        return data[0];
    }
    return data[tail];
}

template<typename type, int size>
void queue<type, size>::clear() {
    head = tail = -1;
}

//template<typename type, int size>
//void queue<type, size>::print() {
//    if (isEmpty()) {
//        std::cout << "queue is empty" << std::endl;
//        return;
//    }
//    int i = (head + 1) % size;
//    while (i != tail) {
//        std::cout << data[i] << " ";
//        i = (i + 1) % size;
//    }
//    std::cout << data[tail] << std::endl;
//}

template<typename type, int size>
queue<type, size> &queue<type, size>::operator=(const queue<type, size> &q) {
    if (this == &q) return *this;
    head = q.head;
    tail = q.tail;
    for (int i = 0; i < size; i++) {
        data[i] = q.data[i];
    }
    return *this;
}

template class queue<Rob_Entry, ROB_SIZE>;
template class queue<LSB_Entry, LSB_SIZE>;
