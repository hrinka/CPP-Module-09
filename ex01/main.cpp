#include "RPN.hpp"
#include <iostream>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: ./RPN \"expression\"" << std::endl;
        return 1;
    }
    try 
    {
        int result = RPN::calculate(argv[1]);
        std::cout << result << std::endl;
    } 
    catch (const std::exception& e) 
    {
        std::cout << e.what() << std::endl;
        return 1;
    }
    return 0;
}
