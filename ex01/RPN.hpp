#pragma once

#include <iostream>
#include <stack>
 
class RPN
{
    public:
        RPN();
        RPN(const RPN &other);
        RPN &operator=(const RPN &other);
        ~RPN();
        int evaluate(const std::string &expression);
    private:
        std::stack<int> stack;
    
        bool isOperator(char c) const;
        int applyOperator(char op, int a, int b) const;
};
 