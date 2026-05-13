// #include "PmergeMe.hpp"

// void PmergeMe::parse(char **av){
//     for (int i = 1; av[i]; i++)
//     {
//         std::string s(av[i]);
//         if (s.empty())
//             throw std::runtime_error("Error in nput");
        
//         for (int j = 0; j < s.size(); j++)
//             if (!std::isdigit(s[j]))
//                 throw std::runtime_error("Error in nput");
    
//         long n = std::atol(s.c_str());
//         if (n < 0 || n > 2147483647L)
//             throw std::runtime_error("Error in nput");
        
//         vec.push_back(n);
//         deq.push_back(n);
//     }
//     if (vec.empty() || deq.empty())
//         throw std::runtime_error("Error in nput");
// }

// PmergeMe::PmergeMe(char **av):  vStart(0), vEnd(0),dStart(0), dEnd(0)
// {
//     parse(av);
// }

// // void PmergeMe::printNumbers(const std::string &label) const
// // {
// //     std::cout << label;
// //     for (int i = 0; i < vec.size(); i++)
// //         std::cout << vec[i] << " ";
// //     std::cout << "\n";
// // }

// // size_t jacobsthal(size_t k)
// // {
// //     if (k == 0) return 1;
// //     if (k == 1) return 3;
// //     size_t a = 1, b = 3;
// //     for (size_t i = 2; i <= k; i++)
// //     {
// //         size_t c = b + 2 * a;
// //         a = b;
// //         b = c;
// //     }
// //     return b;
// // }

// void PmergeMe::printVectorTime() 
// {
//     vEnd = std::clock();

//     double duration;
//     duration = static_cast<double>(vEnd - vStart) / CLOCKS_PER_SEC;

//     std::cout << "Sorting using Vector spend: " << duration << " microseconds\n";
// }

// void PmergeMe::printDequeTime() 
// {
//     vEnd = std::clock();

//     double duration;
//     duration = static_cast<double>(vEnd - vStart) / CLOCKS_PER_SEC;

//     std::cout << "Sorting using Deque spend: " << duration << " microseconds\n";
// }




// void PmergeMe::sortVec(std::vector<int> &v)
// {
//     if (v.size() <= 1)
//         return ;
//     std::vector<Pair> pairs;
//     std::vector<int> bigs;
//     int straggler = -1;
    
    
//     if (v.size() % 2 != 0)
//     {
//         straggler = v.back();
//         // v.pop_back();
//     }
//     int l = v.size() - (straggler != -1 ? 1 : 0);


//     for (int i = 0; i < l; i += 2)
//     {
//         Pair p;
//         if (v[i] > v[i + 1])
//         {
//             p.big = v[i]; p.small = v[i + 1];
//         }else {
//             p.small = v[i]; p.big = v[i + 1];
//         }
//         bigs.push_back(p.big);
//         pairs.push_back(p);
//     }


//     sortVec(bigs);


//     std::vector<Pair> newOrder;
//     newOrder.reserve(pairs.size());
//     std::vector<bool> isUsed(pairs.size(), false);

//     for (int i = 0; i <  bigs.size(); i++)
//     {

//         for (int j = 0; j < pairs.size(); j++)
//         {
//             if (!isUsed[j] && pairs[j].big == bigs[i])
//             {
//                 newOrder.push_back(pairs[j]);
//                 isUsed[j] = true;
//                 break ;
//             }
//         }
//     }
//     pairs.swap(newOrder);

//     std::vector<int> mainChain;
//     mainChain.reserve(v.size());
//     mainChain.push_back(pairs[0].small);
//     for (int i = 0; i < pairs.size(); i++)
//     {
//         mainChain.push_back(pairs[i].big);
//     }


//     std::vector<int> pendChain;
//     pendChain.reserve(pairs.size() + (straggler == -1 ? 0 : 1));
    
//     for (int i = 1; i < pairs.size(); i++)
//     {
//         pendChain.push_back(pairs[i].small);
//     }
//     if (straggler != -1) pendChain.push_back(straggler);



// }


#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <ctime>
#include <iomanip>
#include <stdexcept>
#include <cstdlib>
#include <cctype>


static size_t jacobsthal(size_t k)
{
    if (k == 0) return 1;
    if (k == 1) return 3;
    size_t a = 1, b = 3;
    for (size_t i = 2; i <= k; ++i) {
        size_t c = b + 2 * a;
        a = b;
        b = c;
    }
    return b;
}

// Produces the Ford-Johnson insertion order for a pend array of size m.
// Within each Jacobsthal group elements are inserted from the highest index
// down to the lowest, which keeps the binary-search upper bound tight and
// minimises the total number of comparisons.
//
// Example  m=5  → [ 0, 2, 1, 4, 3 ]
// Example  m=11 → [ 0, 2, 1, 4, 3, 10, 9, 8, 7, 6, 5 ]

static std::vector<size_t> insertionOrder(size_t m)
{
    std::vector<size_t> order;
    size_t prev = 0; // upper bound of the previous Jacobsthal group (1-indexed)

    for (size_t k = 0; prev < m; ++k) {

        const size_t j   = jacobsthal(k);
        std::cout << "J =  " << j <<std::endl;
        const size_t cur = (j < m) ? j : m; // cap to m (still 1-indexed)

        // Insert this group from cur down to prev+1 (convert to 0-indexed).
        std::cout << "order:  " ;
        for (size_t i = cur; i > prev; --i)
        {
            order.push_back(i - 1);
            std::cout << order.back() << " ";
        }
        std::cout << " \n" << std::endl;
        prev = j;
        if (j >= m)
            break;
    }
    return order;
}


int main ()
{

    const std::vector<size_t> order = insertionOrder(30);

    std::cout << "\n\n ";
    for (int i =0; i < order.size(); ++i)
        std::cout << order[i] << " ";
}