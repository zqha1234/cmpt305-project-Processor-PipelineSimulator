#pragma once
#include "instructions.h"
#include <list>
using namespace std;

class pipelineManager {
public:

    list<instructions> if_list;
    list<instructions> id_list;
    list<instructions> ex_list;
    list<instructions> mem_list;
    list<instructions> wb_list;
    
    pipelineManager() {

    }
    ~pipelineManager() {

    }
};