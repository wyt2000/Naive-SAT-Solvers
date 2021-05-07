#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include "../include/parser.h"
#include "../include/solver.h"

int main(int argc, char **argv) {
    if (argc == 1 || strcmp(argv[1], "-in") || argc != 3) {
        std::cout << "Usage: -in <cnfpath>" << std::endl;
        return -1;
    }
    std::ifstream fin(argv[2]);
    if (!fin) {
        std::cout << "Error: couldn't find " + std::string(argv[2]) << std::endl;
        return -1;
    }
    Parser parser;
    Solver solver;
    parser.setClauses(fin);
    if (solver.solve(parser.getClauses())) {
        std::cout << "satisfiable" << std::endl;
    }
    else {
        std::cout << "unsatisfiable" << std::endl;
    }
    return 0;
}