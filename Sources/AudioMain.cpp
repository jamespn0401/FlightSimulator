#include <iostream>
#include "Utils/utils.hpp"

using namespace std;

int main(int argc, char const *argv[])
{

    string parentheses = "{[()]}";

    if (isValid(parentheses))
    {
        cout << "Valid parentheses" << endl;
    }
    else
    {
        cout << "Invalid parentheses" << endl;
    }
    return 0;
}
