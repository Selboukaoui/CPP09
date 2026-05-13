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
    struct Pair {
        int big;
        int small;
    };

    PmergeMe(char **av);

    void printNumbers(const std::string &label) const;

    void sortVector();
    void sortDeque();

    void printVectorTime();
    void printDequeTime();

private:
    std::vector<int> vec;
    std::deque<int>  deq;

    clock_t vStart, vEnd;
    clock_t dStart, dEnd;

    void parse(char **av);

    void sortVec(std::vector<int> &v);
    void sortDeq(std::deque<int>  &d);
};