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
    for (auto it = clauses.begin(); it != clauses.end();) {
        if (it->count(literal)) {
            it = clauses.erase(it);
        }
        else if (it->count(-literal)) {
            if(it->size() == 1) {
                return false;
            }
            it->erase(-literal);
            if (it->empty()) {
                it = clauses.erase(it);
            }
            else {
                it++;
            }
        }
        else {
            it++;
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
    int p = *(*clauses.begin()).begin();
    clauses_type positive = clauses;
    clauses_type negative = clauses;
    positive.emplace_back(std::set<int>{p});
    negative.emplace_back(std::set<int>{-p});
    return solve(positive, assigns) || solve(negative, assigns);
}

void Solver::printResult(std::ostream &output) {
    for(auto assignment: assignments) {
        output << assignment << ' ';
    }
    output << std::endl;
}