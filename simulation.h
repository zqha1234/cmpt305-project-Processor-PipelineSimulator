#pragma once

#include "instructions.h"
// #include <vector>
#include <string>
using namespace std;

class simulation {
public:
    // instructions ins;
    string file_name;
    int ins_start;
    int ins_size;
    short int width;
    int ins_count;
    int clock;
    bool finish; // use to indicate if the simulation finished
    simulation(){

    }
    simulation(string f, int start, int size, short int w){
        file_name = f;
        ins_start = start;
        ins_size = size;
        width = w;
        ins_count = 0;
        clock = 0;
        finish = false;
    }
    instructions createInstruction(string s);
    void startSimulation();
    ~simulation(){

    }
};