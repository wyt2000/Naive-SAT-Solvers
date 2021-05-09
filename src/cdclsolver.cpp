#include "../include/cdclsolver.h"
#include <iostream>

void CDCLSolver::assign(int literal) {
    literalStack.push(literal);
    assignments[abs(literal)] = (literal > 0) ? POSITIVE : NEGATIVE;
    undefinedSet.erase(abs(literal));
}

int CDCLSolver::unassign() {
    int literal = literalStack.top();
    literalStack.pop();
    assignments[abs(literal)] = UNDEFINED;
    undefinedSet.insert(abs(literal));
    return literal;
}

STATE CDCLSolver::unitPropagate() {
    STATE state = STABLE;
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
                assign(unit);
                state = UPDATED;
            }
            else {
                state = CONTRADICTED;
            }
        }
    }
    return state;
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

bool CDCLSolver::backJumping() {
    if (decisionStack.empty()) {
        return false;
    }
    int decision = decisionStack.top();
    decisionStack.pop();
    while (unassign() != decision);
    assign(-decision);
    return true;
}

bool CDCLSolver::solve() {
    assignments.resize(nbvar + 1, UNDEFINED);
    for (int i = 1; i <= nbvar; i++) {
        undefinedSet.insert(i);
    }
    while(1) {
        STATE state = unitPropagate();
        if (state == CONTRADICTED && !backJumping()) {
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