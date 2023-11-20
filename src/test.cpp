#include <iostream>
#include "../include/LUT.h"
#include "../include/FPGA.h"
#include "../include/tools.h"

using namespace std;

// Function to print a vector of booleans (for displaying test results)
void printBoolVec(const vector<bool> &vec)
{
    for (bool b : vec)
    {
        cout << (b ? "1" : "0") << " ";
    }
    // cout << endl;
}

int main()
{
    // create LUT
    LUT lut(4, "testLUT"); // 4 inputs, name = "testLUT"

    // it should work for 6 inputs also

    // set boolean expression
    // example: F(a,b,c,d)=a*b+a*c+b*!c*d
    string expression;
    cout << "Enter a 4-variable boolean expression: ";
    cin >> expression;

    lut.setBooleanExpression(expression);

    // get all input combinations
    vector<vector<bool>> testInputs = {
        {0, 0, 0, 0},
        {0, 0, 0, 1},
        {0, 0, 1, 0},
        {0, 0, 1, 1},
        {0, 1, 0, 0},
        {0, 1, 0, 1},
        {0, 1, 1, 0},
        {0, 1, 1, 1},
        {1, 0, 0, 0},
        {1, 0, 0, 1},
        {1, 0, 1, 0},
        {1, 0, 1, 1},
        {1, 1, 0, 0},
        {1, 1, 0, 1},
        {1, 1, 1, 0},
        {1, 1, 1, 1}};

    // evaluate for all input combinations and print truth table
    cout << "\nTesting LUT: " << lut.getName() << "\n\n";
    cout << "a b c d | F" << endl;
    cout << "--------+--" << endl;
    for (const auto &inputs : testInputs)
    {
        bool result = lut.evaluate(inputs);
        printBoolVec(inputs);
        cout << "| " << (result ? "1" : "0") << endl;
    }

    
    return 0;
}
