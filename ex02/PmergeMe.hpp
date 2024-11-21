#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <vector>
#include <deque>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <iterator>

class PmergeMe {
public:
    PmergeMe();
    ~PmergeMe();
    PmergeMe(const PmergeMe& other);
    PmergeMe& operator=(const PmergeMe& other);

    void sortVector(const std::vector<int> &input);
    void sortDeque(const std::deque<int> &input);

    std::vector<int> getSortedVector() const;
    std::deque<int> getSortedDeque() const;

private:
    std::vector<int> sortedVector;
    std::deque<int> sortedDeque;

    template <typename Container>
    void fordJohnsonSort(Container &container);
};

#endif  
