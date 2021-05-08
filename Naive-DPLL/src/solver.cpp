#include "../include/solver.h"
#include <iostream>

bool Solver::isEmpty(clauses_type clauses) {
    for (auto clause: clauses) {
        if (!clause.empty()) {
            return false;
        }
    }
    return true;
}

int Solver::getUnitLiteral(clauses_type clauses) {
    for (auto clause: clauses) {
        if (clause.size() == 1) {
            return *clause.begin();
        }
    }
    return 0;
}

bool Solver::unitResolute(clauses_type &clauses, int literal) {
    int nbclauses = clauses.size();
    for (int i = 0; i < nbclauses; i++) {
        if (clauses[i].count(literal)) {
            clauses[i].clear();
        }
        else if (clauses[i].count(-literal)) {
            if(clauses[i].size() == 1) {
                return false;
            }
            clauses[i].erase(-literal);
        }
    }
    return true;
}

bool Solver::solve(clauses_type clauses, std::set<int> assigns) {
    while (1) {
        int unitLiteral = getUnitLiteral(clauses);
        if (!unitLiteral) {
            break;
        }
        assigns.insert(unitLiteral);
        if (!unitResolute(clauses, unitLiteral)) {
            return false;
        }
    }
    if (isEmpty(clauses)) {
        assignments = assigns;
        return true;
    }
    int nbclauses = clauses.size();
    int p = 0;
    for (int i = 1; i <= nbclauses; i++) {
        if(!assigns.count(i) && !assigns.count(-i)) {
            p = i;
            break;
        }
    }
    if (!p) {
        return false;
    }
    clauses_type positive = clauses;
    clauses_type negative = clauses;
    positive.emplace_back(std::set<int>{p});
    negative.emplace_back(std::set<int>{-p});
    return solve(positive, assigns) || solve(negative, assigns);
}

void Solver::printResult() {
    for(auto assignment: assignments) {
        std::cout << assignment << ' ';
    }
    std::cout << std::endl;
}