#include "../include/cdclsolver.h"
#include <iostream>
#include <map>
#include <algorithm>

void CDCLSolver::assign(int literal) {
    literalStack.push_back(literal);
    assignments[abs(literal)] = (literal > 0) ? POSITIVE : NEGATIVE;
    undefinedSet.erase(abs(literal));
}

int CDCLSolver::unassign() {
    int literal = literalStack.back();
    literalStack.pop_back();
    assignments[abs(literal)] = UNDEFINED;
    undefinedSet.insert(abs(literal));
    return literal;
}

STATE CDCLSolver::unitPropagate(std::set<int> &learnedClause) {
    std::map<int, int> unitMap;
    int nbclauses = clauses.size();
    for (int i = 0; i < nbclauses; i++) {
        int unit = 0;
        std::set<int>::iterator it;
        for (it = clauses[i].begin(); it != clauses[i].end(); it++) {
            int literal = *it;
            VALUE value = assignments[abs(literal)];
            if (value == UNDEFINED && !unit) {
                unit = literal;
            }
            else if (!(value == POSITIVE && literal < 0 || value == NEGATIVE && literal > 0)) {
                break;
            }
        }
        if (it == clauses[i].end()) {
            if (unit) {
                unitMap.insert({unit, i});
            }
            else {
                return CONTRADICTED;
            }
        }
    }
    if(unitMap.empty()) {
        return STABLE;
    }
    else {
        for (auto tier: unitMap) {
            int unit, i;
            std::tie(unit, i) = tier;
            if (unitMap.find(-unit) != unitMap.end()) {
                int j = unitMap[-unit];
                std::set_union(clauses[i].begin(),
                                      clauses[i].end(),
                                      clauses[j].begin(),
                                      clauses[j].end(),
                                      std::inserter(learnedClause, learnedClause.begin()));
                learnedClause.erase(unit);
                learnedClause.erase(-unit);
                clauses.push_back(learnedClause);
                // TODO: Forget redundant clauses
                return CONTRADICTED;
            }
            else {
                assign(unit);
            }
        }
    }
    return UPDATED;
}

bool CDCLSolver::decide() {
    if (undefinedSet.empty()) {
        return false;
    }
    int literal = *undefinedSet.begin();
    assign(literal);
    decisionStack.push(literal);
    return true;
}

bool CDCLSolver::backJumping(std::set<int> learnedClause) {
    if (decisionStack.empty()) {
        return false;
    }
    int decision = decisionStack.top();
    decisionStack.pop();
    int jumpingPoint = decision;
    int i;
    for (i = literalStack.size() - 1; i >= 0; i--) {
        if (learnedClause.count(-literalStack[i]) && literalStack[i] != decision || decisionStack.empty() || learnedClause.empty()) {
            break;
        }
        if (literalStack[i] == decisionStack.top()) {
            jumpingPoint = literalStack[i];
            decisionStack.pop();
        }
    }
    while (unassign() != jumpingPoint);
    assign(-decision);
    return true;
}

bool CDCLSolver::solve() {
    assignments.resize(nbvar + 1, UNDEFINED);
    for (int i = 1; i <= nbvar; i++) {
        undefinedSet.insert(i);
    }
    while(1) {
        std::set<int> learnedClause;
        STATE state = unitPropagate(learnedClause);
        if (state == CONTRADICTED && !backJumping(learnedClause)) {
            return false;
        }
        else if (state == STABLE && !decide()) {
            return true;
        }            
    }
    return false;
}

void CDCLSolver::printResult(std::ostream &output) {
    for (int i = 1; i <= nbvar; i++) {
        output << ((assignments[i] == POSITIVE) ? i : -i) << ' ';
    }
    output << std::endl;
}