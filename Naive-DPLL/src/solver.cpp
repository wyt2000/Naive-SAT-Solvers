#include "../include/solver.h"

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

int Solver::getUnitLiteral(clauses_type clauses) {
    for (auto clause: clauses) {
        if (clause.size() == 1) {
            return *clause.begin();
        }
    }
    return 0;
}

void Solver::unitResolute(clauses_type &clauses, int literal) {
    for (auto clause: clauses) {
        if (clause.count(literal)) {
            clauses.erase(clause);
        }
        else if (clause.count(-literal)) {
            clauses.erase(clause);
            clause.erase(-literal);
            if (!clause.empty()) {
                clauses.insert(clause);
            }
        }
    }
}

bool Solver::solve(clauses_type clauses) {
    if (isContradicted(clauses)) {
        return false;
    }
    while (1) {
        int unitLiteral = getUnitLiteral(clauses);
        if (!unitLiteral) {
            break;
        }
        unitResolute(clauses, unitLiteral);
    }
    if (clauses.empty()) {
        return true;
    }
    int p = *(*clauses.begin()).begin();
    clauses_type positive = clauses;
    clauses_type negative = clauses;
    positive.insert(std::set<int>{p});
    negative.insert(std::set<int>{-p});
    return solve(positive) || solve(negative);
}