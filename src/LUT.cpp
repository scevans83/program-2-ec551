#include "../include/LUT.h"

LUT::LUT(int bit_size, const string &name)
    : bit_size(bit_size), name(name), is_input(false), is_output(false), output_connection(nullptr)
{
    input_connections.resize(bit_size, nullptr);
}

LUT::LUT(bool is_input, const string &name)
    : bit_size(1), name(name), is_input(is_input), is_output(!is_input), output_connection(nullptr)
{
    input_connections.resize(1, nullptr);
}

LUT::~LUT()
{
}

void LUT::setBooleanExpression(const string &expression)
{
    // note: expecting expression in F(a,b,c,d) = a*b+c*d+... format
    // get vector of input variables from the part of the expression inside the first ()
    vector<char> inputVars;
    string inputVarsString = expression.substr(expression.find("(") + 1, expression.find(")") - expression.find("(") - 1);
    for (char c : inputVarsString)
    {
        if (c != ',')
        {
            inputVars.push_back(c);
        }
    }
    // create map to associate input variables with index
    map<char, int> inputMap;
    for (int i = 0; i < inputVars.size(); i++)
    {
        inputMap[inputVars[i]] = i;
    }

    // remove everything up to the =
    string exp = expression.substr(expression.find("=") + 1);

    // separate by + into the product terms
    stringstream ss(exp);
    string productTerm;
    vector<string> productTerms;

    while (getline(ss, productTerm, '+'))
    {
        productTerms.push_back(productTerm);
    }

    // evaluate for every possible input combo
    vector<string> binaryTruthTable = generateTruthTable(bit_size);

    for (const string &binaryInput : binaryTruthTable)
    {
        bool result = false; // result starts as false because it is an OR operation

        // evaluate each product term and OR it with the result so far
        for (const string &term : productTerms)
        {
            bool termResult = evaluateProductTerm(term, binaryInput, inputMap);
            result = result || termResult;
        }

        truth_table.push_back(result);
    }
}

bool LUT::evaluate(const vector<bool> &inputs) const
{
    // convert vector<bool> to string
    string binaryInput = convertBoolVecToString(inputs);

    // convert binary string to index
    int index = stoi(binaryInput, nullptr, 2);

    // check if index is within range of truth_table
    if (index < 0 || index >= truth_table.size())
    {
        cerr << "Index out of range" << endl;
        return false;
    }

    // return result at that index
    return truth_table[index];
}

void LUT::connectInput(int input_index, const LUT *source_LUT)
{
    if (input_index >= 0 && input_index < bit_size)
    {
        input_connections[input_index] = source_LUT;
    }
    else
    {
        cerr << "Invalid input connection" << endl;
    }
}

void LUT::connectOutput(LUT *target_LUT)
{
    output_connection = target_LUT;
}

void LUT::printConnections() const
{
    cout << "Inputs: " << endl;
    for (int i = 0; i < bit_size; ++i)
    {
        if (input_connections[i])
        {
            cout << input_connections[i] << " "; // print out what is connected there may need to name LUTs/Inputs
        }
        else
        {
            cout << "X "; // print X if no connection
        }
    }
    cout << endl;

    if (output_connection)
    {
        cout << "Output: " << output_connection << endl;
    }
    else
    {
        cout << "Output: X" << endl;
    }
}

int LUT::getBitSize() const
{
    return bit_size;
}

const string &LUT::getName() const
{
    return name;
}