#pragma once

#include <iostream>
#include <vector>
#include <deque>
#include <sstream>
#include <algorithm>
#include <ctime>
#include <iomanip>

class PmergeMe {
    private:
        std::vector<int> _vec;
        std::deque<int>  _deq;

        clock_t _vStart, _vEnd;
        clock_t _dStart, _dEnd;

    public:
        PmergeMe(char **av);

        void printNumbers(std::string state);

        void sortVector();
        void sortDeque();

        void printVectorTime();
        void printDequeTime();

    private:
        void parse(char **av);

        void sortVec(std::vector<int> &v);
        void sortDeq(std::deque<int> &d);
};

