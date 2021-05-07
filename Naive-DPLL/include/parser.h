#ifndef PARSER_H
#define PARSER_H
#include <iostream>
#include <vector>
#include <set>
using clauses_type = std::set<std::set<int>>;
/*  Input Format: DIMACS
*   The file can start with comments, that is lines begining with the character c. 
*   Right after the comments, there is the line: p cnf nbvar nbclauses 
*   Then comes the clauses ending with zero. 
*   The positive numbers mean positive literals, and the negative numbers mean their negations.
*   Source: http://www.satcompetition.org/2009/format-benchmarks2009.html
*/
class Parser {
private:
    int nbvar;
    int nbclauses;
    clauses_type clauses;
public:
    bool setClauses(std::istream &input);
    clauses_type getClauses();
    void showClauses(std::ostream &output);
};


#endif