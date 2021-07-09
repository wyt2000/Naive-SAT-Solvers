#ifndef CDCLSOLVER_H
#define CDCLSOLVER_H

#include <vector>
#include <set>
#include <stack>
#include <ostream>
#include <map>
#include <iterator>
#include <random>
#include <algorithm>
#include "../include/parser.h"

#define clauses_type std::vector<std::set<int>>
#define iter_type std::set<int>::iterator

/**
 * Literal assignment values.
 */
enum VALUE {
    POSITIVE,
    NEGATIVE,
    UNDEFINED
};

/**
 * UnitPropagate return state.
 * UPDATE - new unit found.
 * STABLE - new unit not found.
 * CONTRADICTED - contradiction found.
 */
enum STATE {
    UPDATED,    
    STABLE,     
    CONTRADICTED
};

/**
 * SAT solver based on CDCL.
 */
class CDCLSolver {
private:
    /**
     * Variable number set by parser.
     * Because of clause learning, we couldn't set clause number here.
     */ 
    int nbvar;

    /**
     * Current clause set.
     */
    clauses_type clauses;

    /**
     * The assignment we has already set, include propagating and deciding.
     */
    std::vector<int> literalStack;

    /**
     * The assignment decisions we has already made.
     */
    std::stack<int> decisionStack;

    /**
     * Current variables assignments.
     */
    std::vector<VALUE> assignments;

    /**
     * The undefined variables.
     */
    std::set<int> undefinedSet;

    /**
     * The max times to restart.
     */
    int maxBackJumpingTimes = 10;

    /**
     * The times jumping back to an early decision.
     */
    int backJumpingTimes = 0;

    /**
     * Select randomly in undefinedSet.
     * @param begin Start position.
     * @param end end position.
     * @return A iterator randomly distributed from begin to end.
     */
    iter_type randomlySelect(iter_type begin, iter_type end);

    /**
     * Set the variable value which makes the literal true.
     */
    void assign(int literal);

    /**
     * Unset the variable value on the top of literalStack.
     * @return The literal on the top of literalStack.
     */
    int unassign();

    /**
     * Learn new clause by contradiction. 
     * @param unit The variable which will be assigned as opposite values in two different clauses.
     * @param i One clause.
     * @param j The other clause.
     * @return The new clause learned.
     */
    std::set<int> learn(int unit, int i, int j);

    /**
     * Find the clauses with contradicted assignments expect one unknown literal.
     * Assign variables and learn new clause.
     * @return Propagation state and new clause learned.
     */
    STATE unitPropagate(std::set<int> &learnedClause);

    /**
     * Choose an undefined variable to assign.
     * @return False if undefinedSet is empty.
     */
    bool decide();

    /**
     * Clear literalStack if backJumpingTimes reaches maxBackJumpingTimes
     */
    void restart();

    /**
     * Jump back to an early decision position according to the learned clauses.
     * @param learnedClause The new clause learned.
     * @return False if decisionStack is empty.
     */
    bool backJump(std::set<int> learnedClause);

public:
    /**
     * Initialize the solver by the parser.
     */
    CDCLSolver(Parser parser):
        nbvar(parser.nbvar),
        clauses(parser.clauses){}

    /**
     * Main function to solve SAT problem by CDCL.
     * @return True if satisfiable.
     */
    bool solve();

    void printResult(std::ostream &output);
};

#endif