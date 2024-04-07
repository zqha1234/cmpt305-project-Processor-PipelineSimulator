
#include "simulation.h"
#include "pipelineManager.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <list>
#include <string.h>

using namespace std;

instructions simulation::createInstruction(string str) {
    // int start = strtol(argv[2], NULL, 16);
    str += ",";                    // so it is easy to use find "," to split the tring
    size_t s = 0;                  // start of the element of the line
    size_t e = str.find(",");      // end of the element
    short int ele_count = 1;       // element count
    unsigned long hex_add;
    short int type;
    list<unsigned long> dependencies;
    while (e != str.npos) {
        if (ele_count == 1) {
            string subs = str.substr(s, e - s);
            char c[subs.length()];
            strcpy(c, subs.c_str());
            hex_add = strtol(c, NULL, 16);
        } else if (ele_count == 2) {
            string subs = str.substr(s, e - s);
            type = stoi(subs);
        } else {
            string subs = str.substr(s, e - s);
            char c[subs.length()];
            strcpy(c, subs.c_str());
            unsigned long hex_add_dep = strtol(c, NULL, 16);
            dependencies.push_back(hex_add_dep);
        }
        ele_count++;
        s = e + 1;
        e = str.find(",", s);
    }
    // instructions* ins = new instructions(hex_add, type, dependencies);
    instructions ins(hex_add, type, dependencies);
    return ins;
}

void simulation::startSimulation(){
    ifstream file(file_name);
    if(!file.is_open()) {
        perror("Failed opening the file");
    }
    string line;
    while (ins_count < ins_start - 1) {
        getline(file, line);
        ins_count++;
    }
    // cout << ins_count << endl; // debug use only
    list<instructions> ins_mg; // instruction manager
    pipelineManager pipe_mg;
    while (!finish) {
        while (ins_mg.size() < 4 && ins_count < (ins_start + ins_size - 1)) {
            getline(file, line);
            ins_count++;
            // cout << line << endl; // debug use only
            instructions ins = createInstruction(line);
            ins_mg.push_back(ins);
            /* debug use only*/
            // cout << hex << ins.hex_add << endl;
            // cout << ins.type << endl;
            // list<unsigned long>::iterator it;
            // for (it = ins.dependencies.begin(); it != ins.dependencies.end(); it++) {
            //     cout << hex << *it << endl;
            // }
        }
        // wb stage, width is pipeline's width (from 1 to 4)
        for (int i = 0; i < width; i++) {

        }
        // mem stage
        for (int i = 0; i < width; i++) {

        }
        // ex stage
        for (int i = 0; i < width; i++) {

        }
        // id stage
        for (int i = 0; i < width; i++) {

        }
        // if stage
        for (int i = 0; i < width; i++) {

        }

        clock++;
        // when finishing all the required instructions, break the loop
        if (ins_count >= (ins_start + ins_size - 1)) {
            // add code: if all the pipelines's stages are empty, then finish = true;
            finish = true;
        }
    }
    file.close();
}

