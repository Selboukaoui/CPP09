#include "PmergeMe.hpp"

int main(int ac, char **av)
{
    if (ac < 2) {
        std::cerr << "Error\n";
        return 1;
    }
    try 
    {
        PmergeMe S(av);
        S.printNumbers("Before: ");

        S.sortVector();
        S.sortDeque();

        S.printNumbers("After: ");

        S.printVectorTime();
        S.printDequeTime();
    }
    catch (const std::exception &e) {
        std::cerr << e.what() << "\n";
        return 1;
    }
    return 0;
}