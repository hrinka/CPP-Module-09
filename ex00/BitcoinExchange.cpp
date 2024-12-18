#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange(const std::string& databaseFile) {
    loadDatabase(databaseFile);
}

BitcoinExchange::~BitcoinExchange() {}

void BitcoinExchange::loadDatabase(const std::string& databaseFile) {
    std::ifstream file(databaseFile.c_str());
    if (!file.is_open()) {
        std::cout << "Error: could not open file." << std::endl;
        exit(1);
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string date;
        float rate;
        if (std::getline(ss, date, ',') && ss >> rate) {
            exchangeRates[date] = rate;
        }
    }
    file.close();
}

float BitcoinExchange::getExchangeRate(const std::string& date) const {
    std::map<std::string, float>::const_iterator it = exchangeRates.lower_bound(date);

    if (it == exchangeRates.begin() && it->first != date) {
        return -1.0f; // No valid date found
    }

    if (it == exchangeRates.end() || it->first != date) {
        --it;
    }
    return it->second;
}

bool BitcoinExchange::isValidDate(const std::string& date) const {
    if (date.size() != 10 || date[4] != '-' || date[7] != '-') return false;

    int year, month, day;
    std::istringstream ss(date);
    char dash1, dash2;
    ss >> year >> dash1 >> month >> dash2 >> day;

    if (dash1 != '-' || dash2 != '-' || ss.fail()) return false;
    if (month < 1 || month > 12 || day < 1 || day > 31) return false;
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return false;
    if (month == 2) // 閏年のチェック
    {
        bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (day > 29 || (day == 29 && !isLeap)) return false;
    }
    return true;
}

bool BitcoinExchange::isValidValue(const std::string& valueStr, float& value) const {
    char* endPtr;
    const char* str = valueStr.c_str();
    value = std::strtof(str, &endPtr);

    // strtofの結果が不正な場合や、全ての文字を消費していない場合
    if (*endPtr != '\0' || endPtr == str) {
        std::cout << "Error: invalid number format => " << valueStr << std::endl;
        return false;
    }

    if (value < 0) {
        std::cout << "Error: not a positive number." << std::endl;
        return false;
    }
    if (value > 1000) {
        std::cout << "Error: too large a number." << std::endl;
        return false;
    }

    return true;
}

void BitcoinExchange::processInput(const std::string& inputFile) const {
    std::ifstream file(inputFile.c_str());
    if (!file.is_open()) {
        std::cout << "Error: could not open file." << std::endl;
        return;
    }

    std::string line;
    // 1行目を読み飛ばす
    if (!std::getline(file, line)) {
        std::cout << "Error: could not read the first line." << std::endl;
        return;
    }
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string date, separator, valueStr;
        float value;

        if (!(std::getline(ss, date, '|') && ss >> valueStr)) {
            std::cout << "Error: bad input => " << line << std::endl;
            continue;
        }

        date = date.substr(0, date.find_last_not_of(" ") + 1);
        valueStr = valueStr.substr(valueStr.find_first_not_of(" "));

        if (!isValidDate(date)) {
            std::cout << "Error: bad input => " << line << std::endl;
            continue;
        }

        if (!isValidValue(valueStr, value)) {
            continue;
        }

        float rate = getExchangeRate(date);
        if (rate < 0) {
            std::cout << "Error: date not found => " << date << std::endl;
        } else {
            std::cout << date << " => " << value << " = " << value * rate << std::endl;
        }
    }
    file.close();
}
