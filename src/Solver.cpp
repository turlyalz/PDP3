#include "Solver.h"

#include "base.h"

#include <algorithm>
#include <limits>
#include <iostream>
#include <chrono>

#include <omp.h>

using namespace std;

Solver::Solver()
{
    m_solution = new Solution();
}

Solver::~Solver()
{
    delete m_solution;
}

uint Solver::numberOfEdges()
{
    int counter = 0;
    for (uint row = 0; row < m_problem->n; ++row)
    {
        for (uint col = row + 1; col < m_problem->n; ++col){
            if (m_problem->graph[row][col])
            {
                ++counter;
            }
        }
    }
    return counter;
}

uint Solver::calculatePrice(const vector<uint>& state)
{
    uint price = 0;

    for (uint row = 0; row < m_problem->graph.size(); ++row)
    {
        for (uint col = row + 1; col < m_problem->graph[row].size(); ++col)
        {
            if (m_problem->graph[row][col] && (binary_search(state.cbegin(), state.cend(), row) != binary_search(state.cbegin(), state.cend(), col)))
            {
                price++;
            }
        }
    }
    return price;
}

void Solver::solveSubtree(uint n, uint r, std::vector<uint>& state)
{
    if (r == 0)
    {
        uint price = calculatePrice(state);
        if (price < m_solution->price)
        {
            #pragma omp critical
            {
                m_solution->price = price;
                m_solution->nodes = state;
            }
        }
        return;
    }

    if (n == 0)
    {
        return;
    }

    state[r - 1] = n - 1;

    // Calculate predicate price
    uint price = calculatePrice(state);
    if (price > m_solution->price)
    {
        return;
    }

    #pragma omp task shared(state)
        solveSubtree(n - 1, r - 1, state);
    #pragma omp taskwait

    #pragma omp task shared(state)
        solveSubtree(n - 1, r, state);
    #pragma omp taskwait
}

void Solver::run(const Problem* problem)
{
    m_problem = problem;
    if (!m_problem)
    {
        return;
    }
    auto start = chrono::high_resolution_clock::now();

    vector<uint> state(m_problem->a);

    for (uint i = 0; i < m_problem->a; ++i)
    {
        state[i] = 0;
    }

    m_solution->price = numberOfEdges();

    cout << "Start price: " << m_solution->price << endl;

    #pragma omp parallel num_threads(m_problem->threads)
    {
        #pragma omp single
            solveSubtree(m_problem->n, m_problem->a, state);
    }

    cout << *m_solution;

    auto stop = chrono::high_resolution_clock::now();
    using fpSeconds = chrono::duration<float, chrono::seconds::period>;
    auto elapsedTime = fpSeconds(stop - start).count();
    cout << "Elapsed time: " << elapsedTime << " seconds" << endl;
}
