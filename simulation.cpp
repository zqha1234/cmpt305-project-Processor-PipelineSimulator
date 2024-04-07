
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

// create and return instruction
instructions simulation::createInstruction(string str) {
    str += ",";                    // so it is easy to use find "," to split the tring
    size_t s = 0;                  // start of the element of the line
    size_t e = str.find(",");      // end of the element
    short int ele_count = 1;       // element count
    unsigned long long hex_add;
    short int type;
    list<unsigned long long> dependencies;
    while (e != str.npos) {
        if (ele_count == 1) {
            string subs = str.substr(s, e - s);
            char c[subs.length()];
            strcpy(c, subs.c_str());
            hex_add = stoul(c, NULL, 16);
        } else if (ele_count == 2) {
            string subs = str.substr(s, e - s);
            type = stoi(subs);
        } else {
            string subs = str.substr(s, e - s);
            char c[subs.length()];
            strcpy(c, subs.c_str());
            unsigned long long hex_add_dep = stoul(c, NULL, 16);
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

// update every stage 
void simulation::updateStages(DependancyChecker* dc, list<instructions>* ins_mg){
    
    // wb stage
    while (!dc->pipeline.wb_list.empty()) {
        dc->pipeline.wb_list.pop_front();
    }
    while (!dc->pipeline.mem_list.empty()) {
        dc->pipeline.wb_list.push_back(dc->pipeline.mem_list.front());
        dc->pipeline.mem_list.pop_front();
    }
    // mem stage
    for (size_t i = 0; i < width; i++) {
        if (dc->pipeline.ex_list.empty()) {
            break;
        }
        bool flag = dc->MEMClear(dc->pipeline.ex_list.front());
        if (flag) {
            dc->pipeline.mem_list.push_back(dc->pipeline.ex_list.front());
            dc->pipeline.ex_list.pop_front();
        } else {    // if an instruction cannot get into mem stage from ex stage, 
            break;  // it will block all following instructions
        }
    }
    // ex stage
    for (size_t i = 0; i < width; i++) {
        if (dc->pipeline.id_list.empty()) {
            break;
        }
        bool flag = dc->EXClear(dc->pipeline.id_list.front());
        if (flag) {
            dc->pipeline.ex_list.push_back(dc->pipeline.id_list.front());
            dc->pipeline.id_list.pop_front();
        } else {    // if an instruction cannot get into ex stage from id stage, 
            break;  // it will block all following instructions
        }
    }
    // id stage
    for (size_t i = 0; i < width; i++) {
        if (dc->pipeline.if_list.empty()) {
            break;
        }
        bool flag = dc->IDClear(dc->pipeline.if_list.front());
        if (flag) {
            dc->pipeline.id_list.push_back(dc->pipeline.if_list.front());
            dc->pipeline.if_list.pop_front();
        } else {    // if an instruction cannot get into id stage from if stage, 
            break;  // it will block all following instructions
        }
    }
    // if stage
    for (size_t i = 0; i < width; i++) {
        if (ins_mg->empty()) {
            break;
        }
        bool flag = dc->IFClear(ins_mg->front());
        if (flag) {
            dc->pipeline.if_list.push_back(ins_mg->front());
            typeCount(ins_mg->front());
            ins_mg->pop_front();
        } else { // if an instruction cannot get into if stage, it will block all following instructions 
            break;
        }
    }   
}

// type count
void simulation::typeCount(instructions ins) {
    if (ins.type == 1) {
        int_count++;
    } else if (ins.type == 2) {
        float_count++;
    } else if (ins.type == 3) {
        branch_count++;
    } else if (ins.type == 4) {
        load_count++;
    } else if (ins.type == 5) {
        store_count++;
    }
}

// print statistics
void simulation::printStatistics() {
    double int_percentage = int_count * 1.0 / ins_size * 100.0; 
    cout << "integer instructions: " << int_percentage << "%" << endl;
    double float_percentage = float_count * 1.0 / ins_size * 100.0; 
    cout << "floating point instructions: " << float_percentage << "%" << endl;
    double branch_percentage = branch_count * 1.0 / ins_size * 100.0; 
    cout << "branch instructions: " << branch_percentage << "%" << endl;
    double load_percentage = load_count * 1.0 / ins_size * 100.0; 
    cout << "load instructions: " << load_percentage << "%" << endl;
    double store_percentage = store_count * 1.0 / ins_size * 100.0; 
    cout << "store instructions: " << store_percentage << "%" << endl;
}

// debug use only
void simulation::printStages(DependancyChecker DC) { // debug use only
    cout << "-================================-" << endl;
    cout << "=>Clock: " << clock << endl;
    cout << "-================================-" << endl;
    cout << "IF Stage: " << endl;
    for (instructions i : DC.pipeline.if_list) { 
        cout << hex << i.hex_add << "; ";
    }
    cout << endl;
    cout << "ID Stage: " << endl;
    for (instructions i : DC.pipeline.id_list) { 
        cout << hex << i.hex_add << "; ";
    }
    cout << endl;
    cout << "EX Stage: " << endl;
    for (instructions i : DC.pipeline.ex_list) { 
        cout << hex << i.hex_add << "; ";
    }
    cout << endl;
    cout << "MEM Stage: " << endl;
    for (instructions i : DC.pipeline.mem_list) { 
        cout << hex << i.hex_add << "; ";
    }
    cout << endl;
    cout << "WB Stage: " << endl;
    for (instructions i : DC.pipeline.wb_list) { 
        cout << hex << i.hex_add << "; ";
    }
    cout << endl;
}

// start simulation
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
    DependancyChecker dc(pipe_mg, width);
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
            // list<unsigned long long>::iterator it;
            // for (it = ins.dependencies.begin(); it != ins.dependencies.end(); it++) {
            //     cout << hex << *it << endl;
            // }
        }

        // update every stage
        updateStages(&dc, &ins_mg);

        printStages(dc); // debug use only print every stage's hex_add
        clock++;

        // when finishing all the required instructions, break the loop
        if (ins_count >= (ins_start + ins_size - 1)) {
            // if all the pipelines's stages are empty, then finish = true;
            if (dc.pipeline.if_list.empty() && dc.pipeline.id_list.empty() && dc.pipeline.ex_list.empty() 
                && dc.pipeline.mem_list.empty() && dc.pipeline.wb_list.empty()) {
                finish = true;
            }
        }
    }
    printStatistics();
    // close file
    file.close();
}

