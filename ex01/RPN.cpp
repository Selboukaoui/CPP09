#include "RPN.hpp"
#include <stdexcept>
#include <sstream>

RPN::RPN() {}

RPN::RPN(const RPN &other) : stack(other.stack) {}

RPN &RPN::operator=(const RPN &other)
{
    if (this != &other)
        stack = other.stack;
    return *this;
}

RPN::~RPN() {}

bool RPN::isOperator(char c) const
{
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int RPN::applyOperator(char op, int a, int b) const
{
    switch (op)
    {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0)
                throw std::runtime_error("Error: division by zero");
            return a / b;
    }
    throw std::runtime_error("Error: unknown operator");
}

int RPN::evaluate(const std::string &expression)
{
    std::istringstream iss(expression); 
    std::string token;

    while (iss >> token)
    {
        if (token.length() == 1 && std::isdigit(token[0]))
        {
            stack.push(token[0] - '0');
        }
        else if (token.length() == 1 && isOperator(token[0]))
        {
            if (stack.size() < 2)
                throw std::runtime_error("Error: not enough operands");

            int b = stack.top(); stack.pop();
            int a = stack.top(); stack.pop();

            stack.push(applyOperator(token[0], a, b));
        }
        else
        {
            throw std::runtime_error("Error: invalid token '" + token + "'");
        }
    }

    if (stack.size() != 1)
        throw std::runtime_error("Error: invalid expression");

    return stack.top();
}