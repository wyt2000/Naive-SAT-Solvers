#ifndef SOLVER_H
#define SOLVER_H
#include <set>
using clauses_type = std::set<std::set<int>>;

class Solver {
private:
    bool isContradicted(clauses_type clauses);
    int getUnitLiteral(clauses_type clauses);
    void unitResolute(clauses_type& clauses, int literal);
public:
    bool solve(clauses_type clauses);
};

#endif
