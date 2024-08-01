#pragma once
#ifndef RISC5_CIRCULARQUEUE_H
#define RISC5_CIRCULARQUEUE_H

#include <iostream>

// 容纳 size-1 个元素的循环队列
template<typename type, int size>
class queue{
public:
    // head为第一个元素的前一个位置，tail为最后一个元素的位置
    type data[size];
    int head = -1, tail = -1;

public:
    queue();
    bool isEmpty();
    bool isFull();
    int getSize();
    void push(type x);
    type pop();
    type front();
    type back();
    void clear();
//    void print();
    queue& operator= (const queue<type, size>& q);

public:
    class iterator {
    public:
        queue<type, size> *to_queue;
        int ptr;
    public:
        iterator(queue<type, size> *to_queue_, int ptr_) {
            to_queue = to_queue_;
            ptr = ptr_;
        }
        iterator &operator++() {
            ptr = (ptr + 1) % size;
            return *this;
        }
        bool operator!=(const iterator &obj) {
            return ptr != obj.ptr;
        }
        type* operator->() {
            return &(to_queue->data[ptr]);
        }
        type& operator*() {
            return to_queue->data[ptr];
        }
    };
    iterator begin() {
        int place = (this->head + 1) % size;
        return iterator(this, place);
    }
    iterator end() {
        int place = (this->tail + 1) % size;
        return iterator(this, place);
    }
    type &operator[](int x) {
        return data[x];
    }
};


#endif //RISC5_CIRCULARQUEUE_H
