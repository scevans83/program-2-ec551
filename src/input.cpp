#include "../include/input.h"

using namespace std;

BooleanExpression getInputCircuit()
{
    BooleanExpression expression;
    string next;
    string modelName = "";
    vector<string> truthTable;

    getline(cin, next);

    while (true)
    {
        // debug
        // cout << "Debug: Value of 'next' is: " << next << endl;

        if (next.substr(0, 8) == ".inputs ")
        {
            // cout << "Debug: Inside .inputs block" << endl;

            string inputs = next.substr(8);
            stringstream ss(inputs);
            string token;

            while (getline(ss, token, ' '))
            {
                if (!token.empty())
                {
                    expression.variables.push_back(token[0]);
                }
            }

            // debug
            /* cout << "Debug: Variables parsed: ";
            for (char var : expression.variables)
            {
                cout << var << " ";
            }
            cout << endl; */

            truthTable = generateTruthTable(expression.variables.size());

            // debug
            /* cout << "Debug: Truth table generated: ";
            for (const string &row : truthTable)
            {
                cout << row << " ";
            }
            cout << endl; */
        }
        else
        {
            // cout << "Debug: Skipped .inputs block" << endl;
        }
        if (next == ".end")
        {
            break;
        }

        if (next.substr(0, 7) == ".model ")
        {
            modelName = next.substr(7);
        }

        if (next.substr(0, 7) == ".names ")
        {
            string names = next.substr(7);
            stringstream ss(names);
            string token;
            vector<char> varsInNames;
            while (getline(ss, token, ' '))
            {
                if (!token.empty())
                {
                    varsInNames.push_back(token[0]);
                }
            }
            while (true)
            {
                getline(cin, next); // Read the minterm or maxterm line
                // if line is blank break
                if (next.empty())
                {
                    break;
                }
                for (const auto &row : truthTable)
                {
                    // cout << "Debug: Checking row " << row << " against " << next << endl;
                    bool match = true;
                    string mintermValues = next.substr(0, next.length() - 2);
                    for (int i = 0; i < varsInNames.size(); ++i)
                    {
                        int pos = find(expression.variables.begin(), expression.variables.end(), varsInNames[i]) - expression.variables.begin();
                        if (row[pos] != mintermValues[i])
                        {
                            // debug
                            // cout << "Debug: " << row[pos] << " != " << mintermValues[i] << endl;
                            match = false;
                            break;
                        }
                    }

                    if (match && next[next.length() - 1] == '1')
                    {
                        // debug
                        // cout << "Debug: Matched " << row << " to " << next << "  adding as minterm" << endl;

                        expression.minterms.push_back(stoi(row, nullptr, 2));
                    }
                    else if (match && next[next.length() - 1] == '0')
                    {
                        // debug
                        // cout << "Debug: Matched " << row << " to " << next << "  adding as maxterm" << endl;
                        expression.maxterms.push_back(stoi(row, nullptr, 2));
                    }
                }
            }
        }
        /*         // debug
                for (int i = 0; i < expression.minterms.size(); i++)
                {
                    cout << expression.minterms[i] << " ";
                } */
        getline(cin, next);
    }

    if (!modelName.empty())
    {
        cout << endl
             << "Circuit Name: " << modelName << endl;
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
