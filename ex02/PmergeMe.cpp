#include "PmergeMe.hpp"

// PARSING
void PmergeMe::parse(char **av) {
    for (int i = 1; av[i]; i++) {
        std::string s(av[i]);

        for (size_t j = 0; j < s.size(); j++)
            if (!isdigit(s[j]))
                throw std::runtime_error("Error");

        long n = std::atol(s.c_str());
        if (n < 0)
            throw std::runtime_error("Error");

        _vec.push_back((int)n);
        _deq.push_back((int)n);
    }
    if (_vec.empty())
        throw std::runtime_error("Error");
}

PmergeMe::PmergeMe(char **av) { parse(av); }

// PRINT
void PmergeMe::printNumbers(std::string state) {
    std::cout << state;
    for (size_t i = 0; i < _vec.size(); i++)
        std::cout << _vec[i] << (i + 1 < _vec.size() ? " " : "");
    std::cout << std::endl;
}


// SORT (VECTOR)

void PmergeMe::sortVec(std::vector<int> &v) 
{
    if (v.size() <= 1)
        return;

// Step 1: make pairs (a = max, b = min)
    std::vector <std::pair<int,int> > pairs;
    size_t i = 0;

    for (; i + 1 < v.size(); i += 2) 
    {
        int a = v[i];
        int b = v[i+1];
        if (a < b) std::swap(a, b);
        pairs.push_back(std::make_pair(a, b));
    }

    bool odd = (v.size() % 2);
    int last = odd ? v.back() : 0;

    // Step 2: insertion sort pairs by 'a'
    for (size_t i = 1; i < pairs.size(); i++) {
        std::pair<int,int> key = pairs[i];
        int j = i - 1;
        while (j >= 0 && pairs[j].first > key.first) {
            pairs[j+1] = pairs[j];
            j--;
        }
        pairs[j+1] = key;
    }

    // Step 3: main chain (a's + first b)
    std::vector<int> main;
    main.push_back(pairs[0].second);
    main.push_back(pairs[0].first);

    for (size_t i = 1; i < pairs.size(); i++)
        main.push_back(pairs[i].first);

    // Step 4: insert b's
    for (size_t i = 1; i < pairs.size(); i++) {
        int val = pairs[i].second;
        std::vector<int>::iterator pos =
            std::upper_bound(main.begin(), main.end(), val);
        main.insert(pos, val);
    }

    // Step 5: insert odd
    if (odd) {
        std::vector<int>::iterator pos =
            std::upper_bound(main.begin(), main.end(), last);
        main.insert(pos, last);
    }

    v = main;
}

// CORE SORT (DEQUE)
void PmergeMe::sortDeq(std::deque<int> &d) {
    if (d.size() <= 1)
        return;

    std::deque< std::pair<int,int> > pairs;
    size_t i = 0;

    for (; i + 1 < d.size(); i += 2) {
        int a = d[i];
        int b = d[i+1];
        if (a < b) std::swap(a, b);
        pairs.push_back(std::make_pair(a, b));
    }

    bool odd = (d.size() % 2);
    int last = odd ? d.back() : 0;

    for (size_t i = 1; i < pairs.size(); i++) {
        std::pair<int,int> key = pairs[i];
        int j = i - 1;
        while (j >= 0 && pairs[j].first > key.first) {
            pairs[j+1] = pairs[j];
            j--;
        }
        pairs[j+1] = key;
    }

    std::deque<int> main;
    main.push_back(pairs[0].second);
    main.push_back(pairs[0].first);

    for (size_t i = 1; i < pairs.size(); i++)
        main.push_back(pairs[i].first);

    for (size_t i = 1; i < pairs.size(); i++) {
        int val = pairs[i].second;
        std::deque<int>::iterator pos =
            std::upper_bound(main.begin(), main.end(), val);
        main.insert(pos, val);
    }

    if (odd) {
        std::deque<int>::iterator pos =
            std::upper_bound(main.begin(), main.end(), last);
        main.insert(pos, last);
    }

    d = main;
}

// WRAPPERS
void PmergeMe::sortVector() {
    _vStart = clock();
    sortVec(_vec);
    _vEnd = clock();
}

void PmergeMe::sortDeque() {
    _dStart = clock();
    sortDeq(_deq);
    _dEnd = clock();
}

void PmergeMe::printVectorTime() {
    double t = (double)(_vEnd - _vStart) / CLOCKS_PER_SEC * 1e6;
    std::cout << "Time to process a range of "
              << _vec.size()
              << " elements with std::vector : "
              << std::fixed << std::setprecision(5)
              << t << " us\n";
}

void PmergeMe::printDequeTime() {
    double t = (double)(_dEnd - _dStart) / CLOCKS_PER_SEC * 1e6;
    std::cout << "Time to process a range of "
              << _deq.size()
              << " elements with std::deque : "
              << std::fixed << std::setprecision(5)
              << t << " us\n";
}
