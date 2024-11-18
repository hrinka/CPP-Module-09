#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange(const std::string& databaseFile) {
    loadDatabase(databaseFile);
}

BitcoinExchange::~BitcoinExchange() {}

void BitcoinExchange::loadDatabase(const std::string& databaseFile) {
    std::ifstream file(databaseFile.c_str());
    if (!file.is_open()) {
        std::cerr << "Error: could not open file." << std::endl;
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
    std::istringstream ss(valueStr);
    if (!(ss >> value)) {
        return false;
    }
    if (value < 0){
        std::cerr << "Error: not a positive number." << std::endl;
        return false;
    }
    if (value > 1000) {
        std::cerr << "Error: too large a number." << std::endl;
        return false;
    }
    return true;
}

void BitcoinExchange::processInput(const std::string& inputFile) const {
    std::ifstream file(inputFile.c_str());
    if (!file.is_open()) {
        std::cerr << "Error: could not open file." << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string date, separator, valueStr;
        float value;

        if (!(std::getline(ss, date, '|') && ss >> valueStr)) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        date = date.substr(0, date.find_last_not_of(" ") + 1);
        valueStr = valueStr.substr(valueStr.find_first_not_of(" "));

        if (!isValidDate(date)) {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        if (!isValidValue(valueStr, value)) {
            continue;
        }

        float rate = getExchangeRate(date);
        if (rate < 0) {
            std::cerr << "Error: date not found => " << date << std::endl;
        } else {
            std::cout << date << " => " << value << " = " << value * rate << std::endl;
        }
    }
    file.close();
}
