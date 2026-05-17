#pragma once

#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <stdexcept>
#include <cstdlib>
#include <cctype>

class PmergeMe {

public:
    // ── pair type (public so the file-scope comparator in .cpp can access it) ─
    struct Pair {
        int big;
        int small;
    };

    PmergeMe(char **av);

    void printNumbers(const std::string &label) const;

    void sortVector();
    void sortDeque();

    void printVectorTime() const;
    void printDequeTime() const;

private:
    // ── data ──────────────────────────────────────────────────────────────────
    std::vector<int> vec;
    std::deque<int>  deq;

    clock_t vStart, vEnd;
    clock_t dStart, dEnd;

    // ── helpers ───────────────────────────────────────────────────────────────
    void parse(char **av);

    // ── recursive sort for each container (implemented separately) ────────────
    void sortVec(std::vector<int> &v);
    void sortDeq(std::deque<int>  &d);
};