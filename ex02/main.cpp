#include "PmergeMe.hpp"

int main(int ac, char **av) {
    if (ac < 2) {
        std::cerr << "Error\n";
        return 1;
    }


    try 
    {
        PmergeMe p(av);

        p.printNumbers("before: ");

        p.sortVector();
        p.sortDeque();

        p.printNumbers("After: ");

        p.printVectorTime();
        p.printDequeTime();
    }

    catch (...) {
        std::cerr << "Error\n";
    }
}

