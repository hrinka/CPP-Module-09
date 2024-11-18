#include "PmergeMe.hpp"
#include <iostream>
#include <chrono>

PmergeMe::PmergeMe() {}
PmergeMe::~PmergeMe() {}

// Ford-Johnsonソートの実装
template <typename Container>
void PmergeMe::fordJohnsonSort(Container &container) {
    if (container.size() <= 1) return;

    Container pairs;
    typename Container::iterator it = container.begin();

    // ペアを作成してソート
    while (it != container.end()) {
        typename Container::value_type first = *it++;
        typename Container::value_type second = (it != container.end()) ? *it++ : first;

        if (first > second) std::swap(first, second);
        pairs.push_back(first);
        pairs.push_back(second);
    }

    // 再帰的に小さい値をソート
    Container smaller;
    for (size_t i = 0; i < pairs.size(); i += 2) {
        smaller.push_back(pairs[i]);
    }
    fordJohnsonSort(smaller);

    // 大きい値を挿入
    for (size_t i = 1; i < pairs.size(); i += 2) {
        typename Container::iterator pos = std::upper_bound(smaller.begin(), smaller.end(), pairs[i]);
        smaller.insert(pos, pairs[i]);
    }

    container = smaller;
}

// vector用のソート
void PmergeMe::sortVector(const std::vector<int> &input) {
    sortedVector = input;
    fordJohnsonSort(sortedVector);
}

// deque用のソート
void PmergeMe::sortDeque(const std::deque<int> &input) {
    sortedDeque = input;
    fordJohnsonSort(sortedDeque);
}

// ソート結果の取得
std::vector<int> PmergeMe::getSortedVector() const {
    return sortedVector;
}

std::deque<int> PmergeMe::getSortedDeque() const {
    return sortedDeque;
}
