#include "../include/input.h"

// this function takes in the BLIF text from the user and returns the corresponding boolean expression
BooleanExpression getInputCircuit()
{
    BooleanExpression expression;
    string next;
    while (true)
    {
        getline(cin, next);
        if (next == ".end")
        {
            break;
        }

        if (next.substr(0, 7) == ".model ")
        {
            string modelName = next.substr(7);
            cout << "Model name: " << modelName << endl;
        }

        if (next.substr(0, 8) == ".inputs ")
        {
            string inputs = next.substr(8);
            stringstream ss(inputs);
            string token;
            while (getline(ss, token, ' '))
            {
                if (!token.empty())
                {
                    expression.variables.push_back(token[0]); // assuming single character variable names- may have to change
                }
            }
        }

        if (next.substr(0, 9) == ".outputs ")
        {
        }

        if (next[next.length() - 1] == '1')
        {
            if (next.find('-') != string::npos)
            {
                string minterm0 = next.substr(0, next.length() - 1);
                string minterm1 = next.substr(0, next.length() - 1);
                for (int i = 0; i < minterm0.length(); i++)
                {
                    if (minterm0[i] == '-')
                    {
                        minterm0[i] = '0';
                        minterm1[i] = '1';
                    }
                }
                expression.minterms.push_back(stoi(minterm0, nullptr, 2));
                expression.minterms.push_back(stoi(minterm1, nullptr, 2));
            }
            else
            {
                string minterm = next.substr(0, next.length() - 1);
                expression.minterms.push_back(stoi(minterm, nullptr, 2));
            }
        }
    }
    return expression;
}

BooleanExpression parseBoolean(const string &input)
{
    BooleanExpression expression;
    size_t start = input.find('(') + 1;
    size_t end = input.find(')');

    string varSection = input.substr(start, end - start);
    stringstream varStream(varSection);
    char var;
    while (varStream >> var)
    {
        if (isalpha(var))
        {
            expression.variables.push_back(var);
        }
        varStream.ignore(); // Skip other characters
    }

    start = input.find('(', end) + 1;
    end = input.find(')', start);
    string mintermSection = input.substr(start, end - start);
    stringstream mintermStream(mintermSection);
    int minterm;
    while (mintermStream >> minterm)
    {
        expression.minterms.push_back(minterm);
        mintermStream.ignore(); // Skip other characters
    }

    expression.maxterms = getMaxterms(expression);

    return expression;
}
