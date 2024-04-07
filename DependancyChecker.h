
#include "pipelineManager.h"

#ifndef DEPENDANCYCHECKER_H_
#define DEPENDANCY_H_

/**
 * DependancyChecker Class.
 * 
 * A set of methods to check the dependancies and hazards of an instruction for each stage.
 * It will return false if there is a dependancy or hazard, and true if there is not
**/

class DependancyChecker {
    public:

        pipelineManager pipeline;
        size_t width;

        /*
        * Constructor for the DependancyChecker class.
        */
        DependancyChecker(pipelineManager pipeline, int width) {
            this->pipeline = pipeline;
            this->width = width;
        };

        /*
        * IFClear Method.
        * Checks dependancies for the IF stage.
        *
        * @param instructions ins - the instruction to check dependancies for.
        * @return bool - true if there are no dependancies, false if there are.
        */
        bool IFClear(instructions ins) {
            
            // Checks for available spot
            if (pipeline.if_list.size() == width) { // If the list is full
                return false;
            }

            // Checks for control dependancy
            for (instructions i : pipeline.if_list) { // Checks for branch in the IF stage
                if (i.type == 3) {
                    return false;
                }
            }
            for (instructions i : pipeline.id_list) { // Checks for branch in the ID stage
                if (i.type == 3) {
                    return false;
                }
            }
            for (instructions i : pipeline.ex_list) { // Checks for branch in the EX stage
                if (i.type == 3 && i.getEX == false) { // If the instruction is a branch and has not been executed
                    return false;
                }
            }

            return true;

        };

        /*
        * IDClear Method.
        * Checks dependancies for the ID stage.
        *
        * @param instructions ins - the instruction to check dependancies for.
        * @return bool - true if there are no dependancies, false if there are.
        */
        bool IDClear(instructions ins) {
            
            // Checks for available spot
            if (pipeline.id_list.size() == width) { // If the list is full
                return false;
            }

            // Checks for control dependancy
            // if an instruction is in ID stage, that means there are no branch ahead of it
            // assuem if there is a branch (hasn't get executed) ahead of it, then this insturction
            // would get blocked when it tries to get into IF stage 
            // for (instructions i : pipeline.id_list) { // Checks for branch in the ID stage
            //     if (i.type == 3) {
            //         return false;
            //     }
            // }
            // for (instructions i : pipeline.ex_list) { // Checks for branch in the EX stage
            //     if (i.getEX == 3 && i.getEX == false) { // If the instruction is a branch and has not been executed
            //         return false;
            //     }
            // }

            return true;

        };

        /*
        * EXClear Method.
        * Checks dependancies for the EX stage.
        *
        * @param instructions ins - the instruction to check dependancies for.
        * @return bool - true if there are no dependancies, false if there are.
        */
        bool EXClear(instructions ins) {

            // Checks for available spot
            if (pipeline.ex_list.size() == width) { // If the list is full
                return false;
            }

            // Checks for structural dependancy
            // add code to check i.getEX
            for (instructions i : pipeline.ex_list) { 
                if (i.type == ins.type && i.getEX == false && (ins.type == 1 || ins.type == 2 || ins.type == 3)) { // Integer, Float, or Branch
                    return false;
                }
            }
            
            // Checks for control dependency
            // EX stage don't need to check control dependency (same reason as ID stage)
            // for (instructions i : pipeline.ex_list) {
            //     if (i.getEX == 3 && i.getEX == false) { // If the instruction is a branch and has not been executed
            //         return false;
            //     }
            // }

            // Checks for data dependancy
            for (unsigned long dep : ins.dependencies) {
                for (instructions i : pipeline.ex_list) { // Checks for dependancies in the EX stage
                    if (i.hex_add == dep
                        && i.getEX == false 
                        && (i.type == 1 || i.type == 2)) { // || i.type == 4 || i.type == 5)) { // Integer, Float, Load, or Store
                        return false;
                    }
                    // if the dependency is load or store and that dependency is in EX stage, that means it hasn't finished
                    // load or store
                    if (i.hex_add == dep && (i.type == 4 || i.type == 5)) {
                        return false;
                    }
                }
                for (instructions i : pipeline.mem_list) { // Checks for dependancies in the MEM stage
                    if (i.hex_add == dep 
                        // && i.getMEM == false 
                        && (i.type == 4 || i.type == 5)) { // Load or Store
                        return false;
                    }
                }
            }

            return true;

        };

        /*
        * MEMClear Method.
        * Checks dependancies for the MEM stage.
        * 
        * @param instructions ins - the instruction to check dependancies for.
        * @return bool - true if there are no dependancies, false if there are.
        */
        bool MEMClear(instructions ins) {

            // Checks for available spot
            if (pipeline.mem_list.size() == width) { // If the list is full
                return false;
            }

            // Checks for structural dependancy
            for (instructions i : pipeline.mem_list) {
                if (i.type == ins.type && (ins.type == 4 || ins.type == 5)) { // Load or Store
                    return false;
                }
            }

            return true;
            
        };

        /*
        * WBClear Method.
        * Checks dependancies for the WB stage.
        *
        * @param instructions ins - the instruction to check dependancies for.
        * @return bool - true if there are no dependancies, false if there are.
        */
        bool WBClear(instructions ins) {
            return true; //Always true 
        };
        
        /*
        * Destructor for the DependancyChecker class.
        */
        ~DependancyChecker(){
            
        }

};

#endif