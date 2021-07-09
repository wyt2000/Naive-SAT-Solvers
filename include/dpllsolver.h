#ifndef DPLLSOLVER_H
#define DPLLSOLVER_H

#include <set>
#include <vector>
#include <ostream>

#define clauses_type std::vector<std::set<int>>

/**
 * SAT solver based on DPLL.
 */
class DPLLSolver {

private:
    /**
     * The assignments of variables.
     */
    std::set<int> assignments;

    /**
     * Test if the clauses are all empty, which means they are satisfiable.
     * @param clauses Current clause set.
     * @return True if empty.
     */
    bool isEmpty(clauses_type clauses);

    /**
     * Find the clause with only one literal.
     * @param clauses Current clause set.
     * @return The alone literal, 0 if not exists.
     */
    int getUnitLiteral(clauses_type clauses);

    /**
     * Use the alone literal to simplify the clauses.
     * @param clauses Current clause set.
     * @param literal The alone literal.
     * @return True if not contradicted.
     */
    bool unitResolute(clauses_type &clauses, int literal);

public:
    /**
     * Main function to solve SAT problem by DPLL.
     * @param clauses Current clause set.
     * @param assigns Current variable assignments.
     * @return True if satisfiable.
     */
    bool solve(clauses_type clauses, std::set<int> assigns);

    void printResult(std::ostream &output);
};

#endif
