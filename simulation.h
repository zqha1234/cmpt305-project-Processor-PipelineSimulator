#pragma once

#include "instructions.h"
#include "DependancyChecker.h"
// #include <vector>
#include <string>
using namespace std;

class simulation {
public:
    // instructions ins;
    string file_name;
    int ins_start;      // isnturction start point
    int ins_size;       // insturction size (how many instructions are gonna executed)
    size_t width;       // pipeline width
    int ins_count;      // instruction count
    int clock;          // clock count
    int int_count;      // int alu instruction count
    int float_count;    // floating alu instruction count
    int branch_count;   // branch isntruction count
    int load_count;     // load instruction count
    int store_count;    // store instruction count
    bool finish;        // use to indicate if the simulation finished
    simulation(){

    }
    simulation(string f, int start, int size, size_t w){
        file_name = f;
        ins_start = start;
        ins_size = size;
        width = w;
        ins_count = 0;
        clock = 0;
        int_count = 0;
        float_count = 0;
        branch_count = 0;
        load_count = 0;
        store_count = 0;
        finish = false;
    }
    instructions createInstruction(string s);
    void startSimulation();     // start simulation
    void updateStages(DependancyChecker* dc, list<instructions>* ins_mg);    // update every stage
    void typeCount(instructions ins);      // count types
    void printStatistics();     // print statistics
    void printStages(DependancyChecker DC);     // debug use only
    ~simulation(){

    }
};