#include <string>
#include <sstream>
#include "../include/parser.h"
bool Parser::setClauses(std::istream &input) {
    std::string dummy;
    std::stringstream ss;
    int literal;
    clauses.clear();

    while (input.peek() == 'c') {
        getline(input, dummy);
    }
    if (input.get() != 'p') {
        return false;
    }
    getline(input, dummy);
    ss << dummy;
    ss >> dummy;
    if (dummy != "cnf") {
        return false;
    }
    if (!(ss >> nbvar >> nbclauses)) {
        return false;
    }

    for (int i = 0; i < nbclauses; i++) {
        std::set<int> clause;
        while (1) {
            if (!(input >> literal)) {
                return false;
            }
            if (literal == 0) {
                break;
            }
            if (literal > nbvar || literal < -nbvar) {
                return false;
            }
            clause.insert(literal);
        }
        clauses.insert(clause);
    }

    return true;
}

clauses_type Parser::getClauses() {
    return clauses;
}

void Parser::showClauses(std::ostream &output) {
    output << "=========== show clauses begin ===========" <<  std::endl;
    output << "nbvar: " << nbvar << "," << "nbclauses: " << nbclauses << std::endl;
    for (auto clause: clauses) {
        for (auto literal: clause) {
            output << literal << ' ';
        }
        output << std::endl;
    }
    output << "=========== show clauses end   ===========" <<  std::endl;
}