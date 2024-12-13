#include "PmergeMe.hpp"
#include <iostream>
#include <algorithm> 
#include <cstddef>  

PmergeMe::PmergeMe() {}
PmergeMe::~PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe& other)
    : sortedVector(other.sortedVector), sortedDeque(other.sortedDeque) {}
PmergeMe& PmergeMe::operator=(const PmergeMe& other) {
    if (this != &other) {
        sortedVector = other.sortedVector;
        sortedDeque = other.sortedDeque;
    }
    return *this;
}

std::vector<int> PmergeMe::getSortedVector() const { return sortedVector; }
std::deque<int> PmergeMe::getSortedDeque() const { return sortedDeque; }

void PmergeMe::sortVector(const std::vector<int> &input) {
    sortedVector = input;
    fordJohnsonSort(sortedVector);
}

void PmergeMe::sortDeque(const std::deque<int> &input) {
    sortedDeque = input;
    fordJohnsonSort(sortedDeque);
}

std::vector<size_t> PmergeMe::generateJacobstahl(size_t n) const {
    std::vector<size_t> J;
    J.push_back((size_t)0); // J(0)=0
    if (n == 0) return J;
    J.push_back((size_t)1); // J(1)=1
    for (size_t i = 2; i <= n; ++i) {
        size_t val = J[i - 1] + 2 * J[i - 2];
        if (val > n) break;
        J.push_back(val);
    }
    return J;
}

// smallリストに挿入する位置を決定
template <typename Container, typename T>
typename Container::iterator PmergeMe::insertionPosition(Container &c, const T &val) {
    if (c.empty()) 
        return c.begin();
    
    size_t size = c.size();

    // Jacobstahl数列を使って範囲を絞る
    // 例: Jacobstahl数列J: 0, 1, 1, 3, 5, 11, ...
    // valを挿入すべき範囲をこの数列にしたがって特定する
    std::vector<size_t> J = generateJacobstahl(size - 1);
    size_t low = 0;
    size_t high = size;

    // Jacobstahlインデックスを用いて飛び飛び比較
    // 例えば、c[J[i]]とvalを比較し、valが大きければlowを更新、小さければhighを更新。
    for (size_t i = 0; i < J.size(); ++i) {
        size_t idx = J[i];
        if (idx >= size) break;

        const T &midVal = c[idx];
        if (midVal == val) {
            // 同値ならidx位置
            return c.begin() + idx;
        } else if (midVal < val) {
            low = idx + 1;
        } else {
            high = idx;
            break;
        }
    }
    // Jacobstahlによる探索でlow～highに範囲が絞られたので、
    // その範囲内で通常の二分探索を行う
    if (low < high) {
        typename Container::iterator start = c.begin() + low;
        typename Container::iterator end = c.begin() + high;
        return std::lower_bound(start, end, val);
    }

    // 万が一lowがsizeを超えてしまった場合は末尾に挿入
    return c.begin() + low;
}

// Ford-Johnsonソートアルゴリズム
// 1. ペアリング -> small(p_list)とlarge(q_list)へ分配
// 2. smallを再帰的にFord-Johnsonでソート
// 3. q_listの要素をJacobstahl数列の順序でsmallへ挿入

template <typename Container>
void PmergeMe::fordJohnsonSort(Container &container) {
    if (container.size() <= 1) return;

    Container p_list; // small
    Container q_list; // large

    typename Container::iterator it = container.begin();
    while (it != container.end()) {
        typename Container::value_type first = *it++;
        typename Container::value_type second;
        if (it != container.end()) {
            second = *it++;
        } else {
            // 奇数個の場合、最後の要素を`p_list`に追加
            p_list.push_back(first);
            break;
        }
        
        if (first > second) std::swap(first, second);
        p_list.push_back(first);
        q_list.push_back(second);
    }

    // Step 2: `p_list`を再帰的にソート
    fordJohnsonSort(p_list);

    // Step 3:  q_listをJacobstahl数列の順序で挿入
    size_t q_size = q_list.size();
    std::vector<size_t> J = generateJacobstahl(q_size);

    // `Jacobstahl`数列でインデックスを管理しつつ、要素を挿入
    std::vector<bool> inserted(q_size, false);
    for (size_t i = 0; i < J.size(); ++i) {
        size_t idx = J[i];
        if (idx < q_size && !inserted[idx]) {
            typename Container::iterator pos = insertionPosition(p_list, q_list[idx]);
            p_list.insert(pos, q_list[idx]);
            inserted[idx] = true;
        };
    }

    // 残りの要素を順に挿入
    for (size_t i = 0; i < q_size; ++i) {
        if (!inserted[i]) {
            typename Container::iterator pos = insertionPosition(p_list, q_list[i]);
            p_list.insert(pos, q_list[i]);
        }
    }

    container = p_list;
}

template void PmergeMe::fordJohnsonSort(std::vector<int>&);
template void PmergeMe::fordJohnsonSort(std::deque<int>&);

template std::vector<int>::iterator PmergeMe::insertionPosition(std::vector<int>&, const int&);
template std::deque<int>::iterator PmergeMe::insertionPosition(std::deque<int>&, const int&);
