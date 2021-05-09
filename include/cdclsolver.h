#ifndef CDCLSOLVER_H
#define CDCLSOLVER_H

#include <vector>
#include <set>
#include <stack>
#include <ostream>
#include "../include/parser.h"
using clauses_type = std::vector<std::set<int>>;

enum VALUE {
    POSITIVE,
    NEGATIVE,
    UNDEFINED
};

enum STATE {
    UPDATED,                // new unit found
    STABLE,                 // new unit not found
    CONTRADICTED            // contradiction found
};

class CDCLSolver {
private:
    int nbvar;
    int nbclauses;
    clauses_type clauses;
    std::stack<int> literalStack;
    std::stack<int> decisionStack;
    std::vector<VALUE> assignments;
    std::set<int> undefinedSet;
    void assign(int literal);
    int unassign();
    STATE unitPropagate(std::set<int> &learnedClause);
    bool decide();
    bool backJumping(std::set<int> learnedClause);
public:
    CDCLSolver(Parser parser):
        nbvar(parser.nbvar),
        nbclauses(parser.nbclauses),
        clauses(parser.clauses){}
    bool solve();
    void printResult(std::ostream &output);
};

#endif