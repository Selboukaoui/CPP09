#include "PmergeMe.hpp"

// ─── PARSING ───

void PmergeMe::parse(char **av)
{
    for (int i = 1; av[i]; ++i) {
        const std::string s(av[i]);

        if (s.empty())
            throw std::runtime_error("Error");

        for (size_t j = 0; j < s.size(); ++j)
            if (!std::isdigit(static_cast<unsigned char>(s[j])))
                throw std::runtime_error("Error");

        const long n = std::atol(s.c_str());
        if (n < 0 || n > 2147483647L)
            throw std::runtime_error("Error");

        _vec.push_back(static_cast<int>(n));
        _deq.push_back(static_cast<int>(n));
    }

    if (_vec.empty())
        throw std::runtime_error("Error");
}

PmergeMe::PmergeMe(char **av) : _vStart(0), _vEnd(0), _dStart(0), _dEnd(0)
{
    parse(av);
}

// ─── PRINT ─────

void PmergeMe::printNumbers(const std::string &label) const
{
    std::cout << label;
    for (size_t i = 0; i < _vec.size(); ++i)
        std::cout << _vec[i] << (i + 1 < _vec.size() ? " " : "");
    std::cout << "\n";
}

// ─── JACOBSTHAL HELPERS ──────

// k-th element of the Ford-Johnson insertion sequence:
//   J(0)=1, J(1)=3, J(n)=J(n-1)+2*J(n-2)  →  1, 3, 5, 11, 21, 43, 85, …
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
        const size_t cur = (j < m) ? j : m; // cap to m (still 1-indexed)

        // Insert this group from cur down to prev+1 (convert to 0-indexed).
        for (size_t i = cur; i > prev; --i)
            order.push_back(i - 1);

        prev = j;
        if (j >= m)
            break;
    }
    return order;
}

// ─── SORT (VECTOR) ────
//
// Ford-Johnson / merge-insertion sort for std::vector<int>.
//
// Step 1  Pair adjacent elements so pair.big >= pair.small.
// Step 2  Recursively sort the sequence of larger elements (pair.big).
// Step 3  Rebuild pairs in the order the recursion established:
//           walk the sorted bigs left-to-right; for each value find the
//           first unused original pair whose .big matches and append it.
//           A 'used' flag handles duplicate values correctly.
// Step 4  Build the main chain: [ b1, a1, a2, …, an ].
// Step 5  Collect pend = [ b2, b3, …, bn ] (plus the straggler when odd).
// Step 6  Insert pend elements in Jacobsthal order; use a bounded binary
//         search (upper bound = position of the paired big already in the
//         chain) to keep the comparison count minimal.

void PmergeMe::sortVec(std::vector<int> &v)
{
    if (v.size() <= 1)
        return;

    // ── step 1: save the straggler if the sequence is odd ────
    const bool hasOdd    = (v.size() % 2 != 0);
    const int  straggler = hasOdd ? v.back() : 0;

    // ── step 2: form pairs 
    std::vector<Pair> pairs;
    std::vector<int>  bigs;

    const size_t limit = v.size() - (hasOdd ? 1 : 0);

    for (size_t i = 0; i < limit; i += 2) {
        Pair p;
        if (v[i] >= v[i + 1]) { p.big = v[i];     p.small = v[i + 1]; }
        else                  { p.big = v[i + 1]; p.small = v[i];     }
        pairs.push_back(p);
        bigs.push_back(p.big);
    }

    // ── step 3: recursively sort the larger elements (Ford-Johnson) 

    sortVec(bigs);

    // ── step 4: rebuild pairs in the order the recursion established 

    std::vector<Pair> orderedPairs;
    orderedPairs.reserve(pairs.size());
    std::vector<bool> used(pairs.size(), false);

    for (size_t i = 0; i < bigs.size(); ++i) {
        for (size_t j = 0; j < pairs.size(); ++j) {
            if (!used[j] && pairs[j].big == bigs[i]) {
                orderedPairs.push_back(pairs[j]);
                used[j] = true;
                break;
            }
        }
    }
    pairs.swap(orderedPairs);

    // ── step 5: build main chain [ b1, a1, a2, …, an ] ──────
    std::vector<int> mainChain;
    mainChain.reserve(v.size());
    mainChain.push_back(pairs[0].small);           // b1 goes first
    for (size_t i = 0; i < pairs.size(); ++i)
        mainChain.push_back(pairs[i].big);          // a1 … an

    // ── step 6: collect pend = [ b2, … ] (+ straggler) ──────
    std::vector<int> pend;
    for (size_t i = 1; i < pairs.size(); ++i)
        pend.push_back(pairs[i].small);
    if (hasOdd)
        pend.push_back(straggler);

    // ── step 7: insert pend in Jacobsthal order
    const std::vector<size_t> order = insertionOrder(pend.size());

    for (size_t i = 0; i < order.size(); ++i) {
        const size_t idx   = order[i];
        const int    value = pend[idx];

        // The binary search only needs to reach as far as the paired big
        // element (pairs[idx+1].big), because we already know value is
        // smaller than that element.
        std::vector<int>::iterator bound;

        if (idx + 1 < pairs.size())
            bound = std::upper_bound(mainChain.begin(), mainChain.end(),pairs[idx + 1].big);
        else
            bound = mainChain.end();

        const std::vector<int>::iterator pos = std::upper_bound(mainChain.begin(), bound, value);
        
        mainChain.insert(pos, value);
    }

    v.swap(mainChain);
}

// ─── SORT (DEQUE) ─────
//
// Identical algorithm using std::deque.  Kept separate as required by the
// subject (no generic function shared between the two containers).

void PmergeMe::sortDeq(std::deque<int> &v)
{
    if (v.size() <= 1)
        return;

    const bool hasOdd    = (v.size() % 2 != 0);
    const int  straggler = hasOdd ? v.back() : 0;

    std::deque<Pair> pairs;
    std::deque<int>  bigs;

    const size_t limit = v.size() - (hasOdd ? 1 : 0);
    for (size_t i = 0; i < limit; i += 2) {
        Pair p;
        if (v[i] >= v[i + 1]) { p.big = v[i];     p.small = v[i + 1]; }
        else                   { p.big = v[i + 1]; p.small = v[i];     }
        pairs.push_back(p);
        bigs.push_back(p.big);
    }

    sortDeq(bigs);

    // Rebuild pairs in the order the recursion established.
    std::deque<Pair> orderedPairs;
    std::deque<bool> used(pairs.size(), false);

    for (size_t i = 0; i < bigs.size(); ++i) {
        for (size_t j = 0; j < pairs.size(); ++j) {
            if (!used[j] && pairs[j].big == bigs[i]) {
                orderedPairs.push_back(pairs[j]);
                used[j] = true;
                break;
            }
        }
    }
    pairs.swap(orderedPairs);

    std::deque<int> mainChain;
    mainChain.push_back(pairs[0].small);
    for (size_t i = 0; i < pairs.size(); ++i)
        mainChain.push_back(pairs[i].big);

    std::deque<int> pend;
    for (size_t i = 1; i < pairs.size(); ++i)
        pend.push_back(pairs[i].small);
    if (hasOdd)
        pend.push_back(straggler);

    // insertionOrder returns std::vector; we only iterate, so the type
    // difference between containers does not matter.
    const std::vector<size_t> order = insertionOrder(pend.size());

    for (size_t i = 0; i < order.size(); ++i) {
        const size_t idx   = order[i];
        const int    value = pend[idx];

        std::deque<int>::iterator bound;
        if (idx + 1 < pairs.size())
            bound = std::upper_bound(mainChain.begin(), mainChain.end(),
                                     pairs[idx + 1].big);
        else
            bound = mainChain.end();

        const std::deque<int>::iterator pos =
            std::upper_bound(mainChain.begin(), bound, value);
        mainChain.insert(pos, value);
    }

    v.swap(mainChain);
}

// ─── PUBLIC WRAPPERS ──

void PmergeMe::sortVector()
{
    _vStart = clock();
    sortVec(_vec);
    _vEnd = clock();
}

void PmergeMe::sortDeque()
{
    _dStart = clock();
    sortDeq(_deq);
    _dEnd = clock();
}

void PmergeMe::printVectorTime() const
{
    const double t =
        static_cast<double>(_vEnd - _vStart) / CLOCKS_PER_SEC * 1e6;
    std::cout << "Time to process a range of "
              << _vec.size()
              << " elements with std::vector : "
              << std::fixed << std::setprecision(5) << t << " us\n";
}

void PmergeMe::printDequeTime() const
{
    const double t =
        static_cast<double>(_dEnd - _dStart) / CLOCKS_PER_SEC * 1e6;
    std::cout << "Time to process a range of "
              << _deq.size()
              << " elements with std::deque  : "
              << std::fixed << std::setprecision(5) << t << " us\n";
}
