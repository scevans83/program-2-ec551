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
    int numLuts = 4;
    int bitSize = 4;
    int numInputs = 4;
    int numOutputs = 4;
    vector<string> inputs = {"a", "b", "c", "d"};
    vector<string> outputs = {"F", "G", "H", "I"};
    vector<string> expressions = {"F(a,b,c,d)=a*b+a*c+b*!c*d", "G(a,b,c,d)=a*b+a*c+b*!c*d", "H(a,b,c,d)=a*b+a*c+b*!c*d", "I(a,b,c,d)=a*b+a*c+b*!c*d"};
    // create FPGA
    FPGA fpga(4, 4);
    // set inputs and outputs
    for (auto name : inputs)
    {
        fpga.addInputs(name);
    }

    for (auto name : outputs)
    {
        fpga.addOutputs(name);
    }

    // set expressions for each lut
    for (int i = 0; i < numLuts; i++)
    {
        fpga.setLUTBooleanExpression(i, expressions[i]);
    }

    // connect inputs to luts with fpga::makeConnections
    vector<LUT *> luts = fpga.getLUTs();
    fpga.makeConnections(luts);

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
    cout << "\nTesting FPGA\n\n";
    cout << "a b c d | F G H I" << endl;
    cout << "--------+--------" << endl;
    for (const auto &inputs : testInputs)
    {
        vector<bool> result = fpga.evaluateFPGA(inputs);
        printBoolVec(inputs);
        cout << "| ";
        printBoolVec(result);
        cout << endl;
    }

    /* cout << "\nTesting LUT: " << lut.getName() << "\n\n";
    cout << "a b c d | F" << endl;
    cout << "--------+--" << endl;
    for (const auto &inputs : testInputs)
    {
        bool result = lut.evaluate(inputs);
        printBoolVec(inputs);
        cout << "| " << (result ? "1" : "0") << endl;
    }
 */

    return 0;
}
