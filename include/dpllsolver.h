#ifndef DPLLSOLVER_H
#define DPLLSOLVER_H
#include <set>
#include <vector>
#include <ostream>
using clauses_type = std::vector<std::set<int>>;

class DPLLSolver {
private:
    std::set<int> assignments;
    bool isEmpty(clauses_type clauses);
    int getUnitLiteral(clauses_type clauses);
    bool unitResolute(clauses_type &clauses, int literal);
public:
    bool solve(clauses_type clauses, std::set<int> assigns);
    void printResult(std::ostream &output);
};

#endif
