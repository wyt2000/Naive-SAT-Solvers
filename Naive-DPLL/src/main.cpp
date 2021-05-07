#include <fstream>
#include <iostream>
#include <cstring>
#include <string>
#include "../include/parser.h"

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
    parser.setClauses(fin);
    parser.showClauses(std::cout);
    return 0;
}