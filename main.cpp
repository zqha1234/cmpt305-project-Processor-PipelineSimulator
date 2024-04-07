#include "simulation.h"
#include <iostream>
 
using namespace std;


int main(int argc, char *argv[]) {
    if (argc >= 5) {
        string f_name = argv[1];
        int start = stoi(argv[2]);
        int size = stoi(argv[3]);
        short int w = stoi(argv[4]);
        if (start < 0 || size <= 0 || w <1) {
            cout << "Wrong Input!\n" << endl;
            exit(1);
        }
        simulation sim(f_name, start, size, w);
        sim.startSimulation();
    } else {
        cout << "Insufficient number of arguments provided!\n" << endl;
    }
    return 0;
}