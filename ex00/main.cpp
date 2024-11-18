#include "BitcoinExchange.hpp"

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "Error: could not open file." << std::endl;
        return 1;
    }

    BitcoinExchange exchange("data.csv"); // Replace "data.csv" with your database file
    exchange.processInput(argv[1]);

    return 0;
}
