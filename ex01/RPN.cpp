#include "RPN.hpp"
#include <iostream>

bool RPN::isOperator(const std::string& token)
{
    return (token == "+" || token == "-" || token == "*" || token == "/");
}

int RPN::applyOperator(const std::string& op, int a, int b) 
{
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") {
        if (b == 0) 
            throw std::runtime_error("Error: division by zero.");
        return a / b;
    }
    throw std::runtime_error("Error: unknown operator.");
}

int RPN::calculate(const std::string& expression) 
{
    std::stack<int> stack;
    std::istringstream iss(expression);
    std::string token;

    while (iss >> token) {
        if (isdigit(token[0])) {  // 数字の場合
            stack.push(std::atoi(token.c_str()));
        } 
        else if (isOperator(token)) 
        {  // 演算子の場合
            if (stack.size() < 2) 
                throw std::runtime_error("Error: insufficient values.");
            int b = stack.top(); stack.pop();
            int a = stack.top(); stack.pop();
            stack.push(applyOperator(token, a, b));
        } 
        else
        {
            throw std::runtime_error("Error: invalid token.");
        }
    }

    if (stack.size() != 1) 
        throw std::runtime_error("Error: invalid expression.");
    return stack.top();
}
