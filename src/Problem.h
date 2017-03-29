#ifndef PROBLEM_H
#define PROBLEM_H

#include <vector>

typedef unsigned int uint;

struct Problem
{
    uint a;
    uint n;
    uint threads;
    std::vector<std::vector<bool>> graph;
};

#endif // PROBLEM_H
