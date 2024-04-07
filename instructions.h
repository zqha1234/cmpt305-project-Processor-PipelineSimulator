#pragma once

// #include <vector>
#include<list>
using namespace std;
class instructions {
public:
    // instruction's variables
    unsigned long hex_add;
    short int type;
    list<unsigned long> dependencies; // using vector because vector doesn't need to define its size when defining a vector
    short int stage;
    bool getEX;

    
    instructions() { }
    instructions(unsigned long add, short int t, list<unsigned long> deped) {
        hex_add = add;
        type = t;
        for (unsigned long dependency : deped) {
            dependencies.push_back(dependency);
        }
        stage = 0;
        getEX = false;
    }
    ~instructions() {

    }
};
