#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include "../include/parser.h"
#include "../include/dpllsolver.h"
#include "../include/cdclsolver.h"

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
    if (!parser.setClauses(fin)) {
        std::cout << "Error: invalid input." << std::endl;
        return -1;
    }
    CDCLSolver solver(parser);
    if (solver.solve()) {
        std::cout << "satisfiable" << std::endl;
        solver.printResult(std::cout);
        return 0;
    }
    else {
        std::cout << "unsatisfiable" << std::endl;
        return 1;
    }
}