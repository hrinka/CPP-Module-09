#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

class BitcoinExchange {
public:
    BitcoinExchange(const std::string& databaseFile);
    ~BitcoinExchange();

    void processInput(const std::string& inputFile) const;

private:
    std::map<std::string, float> exchangeRates;

    void loadDatabase(const std::string& databaseFile);
    float getExchangeRate(const std::string& date) const;
    bool isValidDate(const std::string& date) const;
    bool isValidValue(const std::string& valueStr, float& value) const;
};

#endif
