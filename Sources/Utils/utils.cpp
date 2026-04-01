
#include "../Headers/utils.hpp"
#include <iostream>
#include <stack>

using namespace std;

bool isValid(string s)
{
    stack<char> inputStack;
    string lstr = "{[(";
    for (char c : s)
    {

        if (lstr.find(c) != string::npos)
        {
            cout << "Left parentheses: " << c << endl;
            inputStack.push(c);
        }
        else
        {
            cout << "Right parentheses: " << c << endl;
            if (inputStack.empty())
            {
                return false;
            }
            char top = inputStack.top();
            inputStack.pop();
            if (c == '(' && top != ')')
            {
                return false;
            }
            if (c == '[' && top != ']')
            {
                return false;
            }
            if (c == '{' && top != '}')
            {
                return false;
            }
        }
    }
    return inputStack.empty();
}