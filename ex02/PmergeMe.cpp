#include "PmergeMe.hpp" 

void PmergeMe::parse(char **av)
{
    for (int i = 1; av[i]; ++i) 
    {
        const std::string s(av[i]);

        if (s.empty())
            throw std::runtime_error("Parsing Error");

        for (size_t j = 0; j < s.size(); ++j)
            if (!std::isdigit(static_cast<unsigned char>(s[j])))
                throw std::runtime_error("Parsing Error");

        const long n = std::atol(s.c_str());
        if (n < 0 || n > 2147483647L)
            throw std::runtime_error("Parsing Error");

        vec.push_back(static_cast<int>(n));
        deq.push_back(static_cast<int>(n));
    }

    if (vec.empty())
        throw std::runtime_error("Parsing Error");
}

PmergeMe::PmergeMe(char **av) : vStart(0), vEnd(0), dStart(0), dEnd(0) { parse(av); }


void PmergeMe::printNumbers(const std::string &label) const
{
    std::cout << label;
    for (size_t i = 0; i < vec.size(); ++i)
        std::cout << vec[i] << (i + 1 < vec.size() ? " " : "\n");
}
 

//   A B C ..  | C =  B + 2A
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

// Example  m=5  → [ 0, 2, 1, 4, 3 ]
// Example  m=11 → [ 0, 2, 1, 4, 3, 10, 9, 8, 7, 6, 5 ]

static std::vector<size_t> insertionOrder(size_t m)
{
    std::vector<size_t> order;
    size_t prev = 0;           // upper bound of the previous Jacobsthal group 

    for (size_t k = 0; prev < m; ++k) 
    {
        const size_t j   = jacobsthal(k);
        const size_t cur = (j < m) ? j : m;

        for (size_t i = cur; i > prev; --i)
            order.push_back(i - 1);

        prev = j;
        if (j >= m)
            break;
    }
    return order;
}

void PmergeMe::sortVec(std::vector<int> &v)
{
    if (v.size() <= 1)
        return;

    // 1: save the straggler if the sequence is odd 

    const bool hasOdd    = (v.size() % 2 != 0);
    const int  straggler = hasOdd ? v.back() : 0;

    // 2: form pairs 
    std::vector<Pair> pairs;
    std::vector<int>  bigs;

    const size_t sizee = v.size() - (hasOdd ? 1 : 0);

    for (size_t i = 0; i < sizee; i += 2) {
        Pair p;
        if (v[i] >= v[i + 1]) 
        {
            p.big = v[i];     
            p.small = v[i + 1]; 
        }
        else                  
        { 
            p.big = v[i + 1]; 
            p.small = v[i];     
        }
        pairs.push_back(p);
        bigs.push_back(p.big);
    }

    //  3: recursively sort the larger elements (Ford-Johnson) 

    sortVec(bigs);

    //  4: rebuild pairs in the order the recursion established 

    std::vector<Pair> newPairsOrder; 
    newPairsOrder.reserve(pairs.size());
    std::vector<bool> used(pairs.size(), false);

    for (size_t i = 0; i < bigs.size(); ++i)
    {
        for (size_t j = 0; j < pairs.size(); ++j) {
            if (!used[j] && pairs[j].big == bigs[i]) {
                newPairsOrder.push_back(pairs[j]);
                used[j] = true;
                break;
            }
        }
    }
    pairs.swap(newPairsOrder);

    //  5: build main chain [ b1, a1, a2, ...]
 
    std::vector<int> mainChain;

    mainChain.reserve(v.size());
    mainChain.push_back(pairs[0].small);           // b1 goes first because it's small than a1 and a1 the biggest nbr in the chain

    for (size_t i = 0; i < pairs.size(); ++i)
        mainChain.push_back(pairs[i].big);          // a1 … an

    //  6: collect pend = [ b2, … ] ( + straggler)

    std::vector<int> pend;

    for (size_t i = 1; i < pairs.size(); ++i) 
        pend.push_back(pairs[i].small);
    if (hasOdd)
        pend.push_back(straggler);

    //  7: insert pend in Jacobsthal order
    const std::vector<size_t> order = insertionOrder(pend.size()); 

    for (size_t i = 0; i < order.size(); ++i) 
    {
        const size_t idx   = order[i];
        const int    value = pend[idx];

        std::vector<int>::iterator bound;

        if (idx + 1 < pairs.size())
            bound = std::upper_bound(mainChain.begin(), mainChain.end(), pairs[idx + 1].big); 
        else
            bound = mainChain.end();

        const std::vector<int>::iterator pos = std::upper_bound(mainChain.begin(), bound, value);

        mainChain.insert(pos, value);
    }

    v.swap(mainChain);
}

//  SORT DEQUE


void PmergeMe::sortDeq(std::deque<int> &v)
{
    if (v.size() <= 1)
        return;

    const bool hasOdd    = (v.size() % 2 != 0);
    const int  straggler = hasOdd ? v.back() : 0;

    std::deque<Pair> pairs;
    std::deque<int>  bigs;

    const size_t sizee = v.size() - (hasOdd ? 1 : 0);
    for (size_t i = 0; i < sizee; i += 2) 
    {
        Pair p;
        if (v[i] >= v[i + 1]) 
        { 
            p.big = v[i];     
            p.small = v[i + 1]; 
        }
        else                   
        { 
            p.big = v[i + 1]; 
            p.small = v[i];     
        }
        pairs.push_back(p);
        bigs.push_back(p.big);
    }

    sortDeq(bigs);

    std::deque<Pair> newPairsOrder;
    std::deque<bool> used(pairs.size(), false);

    for (size_t i = 0; i < bigs.size(); ++i) {
        for (size_t j = 0; j < pairs.size(); ++j) {
            if (!used[j] && pairs[j].big == bigs[i]) {
                newPairsOrder.push_back(pairs[j]);
                used[j] = true;
                break;
            }
        }
    }
    pairs.swap(newPairsOrder);

    std::deque<int> mainChain;
    mainChain.push_back(pairs[0].small);
    for (size_t i = 0; i < pairs.size(); ++i)
        mainChain.push_back(pairs[i].big);

    std::deque<int> pend;
    for (size_t i = 1; i < pairs.size(); ++i)
        pend.push_back(pairs[i].small);
    if (hasOdd)
        pend.push_back(straggler);

    const std::vector<size_t> order = insertionOrder(pend.size());

    for (size_t i = 0; i < order.size(); ++i) {
        const size_t idx   = order[i];
        const int    value = pend[idx];

        std::deque<int>::iterator bound;
        if (idx + 1 < pairs.size())
            bound = std::upper_bound(mainChain.begin(), mainChain.end(), pairs[idx + 1].big);
        else
            bound = mainChain.end();

        const std::deque<int>::iterator pos = std::upper_bound(mainChain.begin(), bound, value);

        mainChain.insert(pos, value);
    }

    v.swap(mainChain);
}


void PmergeMe::sortVector()
{
    vStart = clock();
    sortVec(vec);
    vEnd = clock();
}

void PmergeMe::sortDeque()
{
    dStart = clock();
    sortDeq(deq);
    dEnd = clock();
}

void PmergeMe::printVectorTime() const
{
    const double t = static_cast<double>(vEnd - vStart) / CLOCKS_PER_SEC * 1e6;
    std::cout << "Time to process a range of " << vec.size() << " elements with std::vector : " << std::fixed << std::setprecision(5) << t << " us\n";
}

void PmergeMe::printDequeTime() const
{
    const double t = static_cast<double>(dEnd - dStart) / CLOCKS_PER_SEC * 1e6;
    std::cout << "Time to process a range of " << deq.size() << " elements with std::deque  : " << std::fixed << std::setprecision(5) << t << " us\n";
}
