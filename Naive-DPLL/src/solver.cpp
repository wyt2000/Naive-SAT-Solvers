#include "../include/solver.h"
#include <iostream>

bool Solver::isContradicted(clauses_type clauses) {
    for (auto clause: clauses) {
        for(auto literal: clause) {
            if(clause.count(-literal)) {
                return true;
            }
        }
    }
    return false;
}

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

void Solver::unitResolute(clauses_type &clauses, int literal) {
    int nbclauses = clauses.size();
    for (int i = 0; i < nbclauses; i++) {
        if (clauses[i].count(literal)) {
            clauses[i].clear();
        }
        else if (clauses[i].count(-literal)) {
            clauses[i].erase(-literal);
        }
    }
}

bool Solver::solve(clauses_type clauses, std::set<int> assigns) {
    if (isContradicted(clauses)) {
        return false;
    }
    while (1) {
        int unitLiteral = getUnitLiteral(clauses);
        if (!unitLiteral) {
            break;
        }
        assigns.insert(unitLiteral);
        unitResolute(clauses, unitLiteral);
    }
    if (isEmpty(clauses)) {
        assignments = assigns;
        return true;
    }
    int nbclauses = clauses.size();
    int p;
    for (int i = 1; i <= nbclauses; i++) {
        if(!assigns.count(i) && !assigns.count(-i)) {
            p = i;
            break;
        }
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