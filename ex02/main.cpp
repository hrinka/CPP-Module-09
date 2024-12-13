#include "PmergeMe.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <deque>
#include <ctime>
#include <stdexcept>
#include <climits>
#include <cstdlib>


int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Usage: ./PmergeMe <positive integers>" << std::endl;
        return 1;
    }

    try {
        std::vector<int> vecInput;
        std::deque<int> deqInput;

        for (int i = 1; i < argc; ++i) {
            char *endptr;
            long num = std::strtol(argv[i], &endptr, 10);

            if (*endptr != '\0') {
                throw std::invalid_argument(std::string("Error: Invalid number: ") + argv[i]);
            }

            if (num > INT_MAX || num < INT_MIN) {
                throw std::out_of_range(std::string("Error: Number out of range: ") + argv[i]);
            }

            if (num <= 0) {
                throw std::invalid_argument("Error: All numbers must be positive");
            }
            vecInput.push_back(num);
            deqInput.push_back(num);
        }

        PmergeMe sorter;

        // vectorソートの時間測定
        std::clock_t start = std::clock();
        sorter.sortVector(vecInput);
        std::clock_t end = std::clock();
        double vectorDuration = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1e6; // マイクロ秒

        // dequeソートの時間測定
        start = std::clock();
        sorter.sortDeque(deqInput);
        end = std::clock();
        double dequeDuration = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1e6;

        std::cout << "Before: ";
        for (size_t i = 0; i < vecInput.size(); ++i) {
            std::cout << vecInput[i] << " ";
        }
        std::cout << "\nAfter (vector): ";
        for (size_t i = 0; i < sorter.getSortedVector().size(); ++i) {
            std::cout << sorter.getSortedVector()[i] << " ";
        }
        std::cout << "\nAfter (deque): ";
        for (size_t i = 0; i < sorter.getSortedDeque().size(); ++i) {
            std::cout << sorter.getSortedDeque()[i] << " ";
        }

        std::cout << "\nTime to process with std::vector: " << vectorDuration << " us" << std::endl;
        std::cout << "Time to process with std::deque: " << dequeDuration << " us" << std::endl;

    } catch (const std::exception &e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
