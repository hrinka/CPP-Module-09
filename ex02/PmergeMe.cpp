#include "PmergeMe.hpp"
#include <iostream>
#include <chrono>

PmergeMe::PmergeMe() {}
PmergeMe::~PmergeMe() {}

// Ford-Johnsonソート
template <typename Container>
void PmergeMe::fordJohnsonSort(Container &container) {
    if (container.size() <= 1) return;

    Container small, large;
    typename Container::iterator it = container.begin();

    // ペアを作成してソート
    while (it != container.end()) {
        typename Container::value_type first = *it++;
        typename Container::value_type second = (it != container.end()) ? *it++ : first;

        if (first > second) std::swap(first, second);
        small.push_back(first);
        large.push_back(second);
    }

    // 再帰的にソート
    fordJohnsonSort(small);
    fordJohnsonSort(large);

    // マージ
    container.clear();
    typename Container::iterator itSmall = small.begin();
    typename Container::iterator itLarge = large.begin();

    while (itSmall != small.end() && itLarge != large.end()) {
        if (*itSmall < *itLarge) {
            container.push_back(*itSmall++);
        } else {
            container.push_back(*itLarge++);
        }
    }

    while (itSmall != small.end()) {
        container.push_back(*itSmall++);
    }

    while (itLarge != large.end()) {
        container.push_back(*itLarge++);
    }
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
