#pragma once

// #include <vector>
#include<list>
#include <cstdint>
using namespace std;
class instructions {
public:
    // instruction's variables
    unsigned long long hex_add;
    short int type;
    list<unsigned long long> dependencies; // using vector because vector doesn't need to define its size when defining a vector
    short int stage;
    bool getEX;

    
    instructions() { }
    instructions(unsigned long long add, short int t, list<unsigned long long> deped) {
        hex_add = add;
        type = t;
        for (unsigned long long dependency : deped) {
            dependencies.push_back(dependency);
        }
        stage = 0;
        getEX = false;
    }
    ~instructions() {

    }
};
