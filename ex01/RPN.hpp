#ifndef RPN_HPP
#define RPN_HPP

#include <stack>
#include <string>
#include <sstream>
#include <stdexcept>

class RPN {
public:
    static int calculate(const std::string& expression);

private:
    static bool isOperator(const std::string& token);
    static int applyOperator(const std::string& op, int a, int b);
};

#endif
