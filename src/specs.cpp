#include "../include/specs.h"

// Function to return the design as a canonical SOP
void canonicalSOP(BooleanExpression expression)
{
    unsigned int numVars = expression.variables.size();

    for (unsigned int i = 0; i < expression.minterms.size(); i++)
    {
        int minterm = expression.minterms[i];
        string binary = bitset<16>(minterm).to_string();
        binary = binary.substr(16 - numVars);

        for (unsigned int j = 0; j < binary.size(); j++)
        {
            if (binary[j] == '0')
            {
                cout << "!" << expression.variables[j];
            }
            else
            {
                cout << expression.variables[j];
            }
        }

        if (i != expression.minterms.size() - 1)
        {
            cout << " + ";
        }
    }
    cout << endl;
}

// Function to return the design as a canonical POS
void canonicalPOS(BooleanExpression expression)
{
    unsigned int numVars = expression.variables.size();

    for (unsigned int i = 0; i < pow(2, numVars); i++)
    {
        // Check if i is not a minterm
        if (find(expression.minterms.begin(), expression.minterms.end(), i) == expression.minterms.end())
        {
            string binary = bitset<16>(i).to_string();
            binary = binary.substr(16 - numVars);

            cout << '(';
            for (unsigned int j = 0; j < binary.size(); j++)
            {
                if (binary[j] == '0')
                {
                    cout << expression.variables[j];
                    if (j != binary.size() - 1)
                        cout << " + ";
                }
                else
                {
                    cout << "!" << expression.variables[j];
                    if (j != binary.size() - 1)
                        cout << " + ";
                }
            }
            cout << ")";
            if (i != pow(2, numVars) - 1)
            {
                cout << " * ";
            }
        }
    }
    cout << endl;
}

// Function to return the design INVERSE as a canonical SOP
void inverseCanonicalSOP(BooleanExpression expression)
{
    unsigned int numVars = expression.variables.size();

    for (unsigned int i = 0; i < pow(2, numVars); i++)
    {

        if (find(expression.minterms.begin(), expression.minterms.end(), i) == expression.minterms.end())
        {
            string binary = bitset<16>(i).to_string();
            binary = binary.substr(16 - numVars);

            for (unsigned int j = 0; j < binary.size(); j++)
            {
                if (binary[j] == '1')
                {
                    cout << expression.variables[j];
                }
                else
                {
                    cout << "!" << expression.variables[j];
                }
            }

            if (i != pow(2, numVars) - 1)
            {
                cout << " + ";
            }
        }
    }
    cout << endl;
}

// Function to return the design INVERSE as a canonical POS
void inverseCanonicalPOS(BooleanExpression expression)
{
    unsigned int numVars = expression.variables.size();

    for (unsigned int i = 0; i < expression.minterms.size(); i++)
    {
        int minterm = expression.minterms[i];
        string binary = bitset<16>(minterm).to_string();
        binary = binary.substr(16 - numVars, numVars);

        cout << '(';
        for (unsigned int j = 0; j < binary.size(); j++)
        {
            if (binary[j] == '0')
            {
                cout << expression.variables[j];
                if (j != binary.size() - 1)
                    cout << " + ";
            }
            else
            {
                cout << "!" << expression.variables[j];
                if (j != binary.size() - 1)
                    cout << " + ";
            }
        }
        cout << ")";

        if (i != pow(2, numVars) - 1)
        {
            cout << " * ";
        }
    }
    cout << endl;
}

// Function to return a minimized number of literals representation in SOP
vector<string> minimizedSOP(BooleanExpression expression)
{
    string sop = "";

    int numLiterals = expression.minterms.size() * expression.variables.size();
    int numLiteralsNew = 0;

    // Get essential prime implicants and all prime implicants
    vector<string> essentialPIs = essentialPrimeImplicants(expression);
    vector<string> allPIs = primeImplicants(expression);

    vector<string> optimizedSOP = essentialPIs;
    set<int> coveredMinterms;

    vector<int> uncoveredMinterms;
    for (int term : expression.minterms)
    {
        bool covered = false;
        for (const string &prime : essentialPIs)
        {
            if (isCovered(decimalToBinary(term, expression.variables.size()), prime))
            {
                covered = true;
                break;
            }
        }
        if (!covered)
        {
            uncoveredMinterms.push_back(term);
        }
    }

    vector<string> additionalPrimes = selectMinimumLiterals(uncoveredMinterms, allPIs);
    optimizedSOP.insert(optimizedSOP.end(), additionalPrimes.begin(), additionalPrimes.end());

    // check each minterm against all others, if any are 1 bit different, combine them
    for (int i = 0; i < optimizedSOP.size(); i++)
    {
        for (int j = i + 1; j < optimizedSOP.size(); j++)
        {
            if (differByOneBit(optimizedSOP[i], optimizedSOP[j]))
            {
                optimizedSOP[i] = combineMinterms(optimizedSOP[i], optimizedSOP[j]);
                optimizedSOP.erase(optimizedSOP.begin() + j);
                j--;
            }
        }
    }

    // print the optimized SOP as variables
    for (int i = 0; i < optimizedSOP.size(); i++)
    {
        for (int j = 0; j < optimizedSOP[i].size(); j++)
        {
            numLiteralsNew++;
            if (optimizedSOP[i][j] == '0')
            {
                // numLiteralsNew++;
                cout << '!' << expression.variables[j];
            }
            else if (optimizedSOP[i][j] == '1')
            {
                // numLiteralsNew++;
                cout << expression.variables[j];
            }
        }
        if (i != optimizedSOP.size() - 1)
            cout << " + ";
        else
            cout << endl;
    }

    cout << "Number of literals in canonical SOP: " << numLiterals << endl;
    cout << "Number of literals in minimized SOP: " << numLiteralsNew << endl;
    cout << "Number of literals saved: " << numLiterals - numLiteralsNew << endl;

    return optimizedSOP;
}

// Function to return a minimized number of literals representation in POS
vector<string> minimizedPOS(BooleanExpression expression)
{
    string pos = "";

    int numLiterals = expression.maxterms.size() * expression.variables.size();
    int numLiteralsNew = 0;

    vector<int> maxterms = getMaxterms(expression);

    vector<string> allPIs = primeImplicantsForMaxterms(expression);

    // Identify uncovered maxterms
    vector<int> uncoveredMaxterms;
    for (int term : maxterms)
    {
        bool covered = false;
        for (const string &prime : allPIs)
        {
            if (isCoveredByMaxterm(decimalToBinary(term, expression.variables.size()), prime))
            {
                covered = true;
                break;
            }
        }
        if (!covered)
        {
            uncoveredMaxterms.push_back(term);
        }
    }

    vector<string> additionalPrimes = selectMinimumLiteralsMaxterm(uncoveredMaxterms, allPIs);

    vector<string> optimizedPOS = additionalPrimes;

    for (int i = 0; i < optimizedPOS.size(); i++)
    {
        for (int j = i + 1; j < optimizedPOS.size(); j++)
        {
            if (differByOneBit(optimizedPOS[i], optimizedPOS[j]))
            {
                optimizedPOS[i] = combineMaxterms(optimizedPOS[i], optimizedPOS[j]);
                optimizedPOS.erase(optimizedPOS.begin() + j);
                j--;
            }
        }
    }

    for (int i = 0; i < optimizedPOS.size(); i++)
    {
        cout << "(";
        for (int j = 0; j < optimizedPOS[i].size(); j++)
        {
            if (optimizedPOS[i][j] == '0')
            {
                numLiteralsNew++;
                cout << expression.variables[j];
            }
            else if (optimizedPOS[i][j] == '1')
            {
                numLiteralsNew++;
                cout << '!' << expression.variables[j];
            }
            if (j != optimizedPOS[i].size() - 1)
            {
                cout << " + ";
            }
        }
        cout << ")";
        if (i != optimizedPOS.size() - 1)
        {
            cout << " ";
        }
        else
        {
            cout << endl;
        }
    }

    cout << "Number of literals in canonical POS: " << numLiterals << endl;
    cout << "Number of literals in minimized POS: " << numLiteralsNew << endl;
    cout << "Number of literals saved: " << numLiterals - numLiteralsNew << endl;

    return optimizedPOS;
}

// Function to report the number of Prime Implicants
vector<string> primeImplicants(BooleanExpression expression)
{
    vector<string> mintermsBinary;
    vector<string> primeImplicants;
    for (int term : expression.minterms)
    {
        mintermsBinary.push_back(decimalToBinary(term, expression.variables.size()));
    }

    // group by number of 1s
    map<int, vector<string>> groupedMinterms;
    for (const string &binTerm : mintermsBinary)
    {
        groupedMinterms[countOnes(binTerm)].push_back(binTerm);
    }

    // combine sdjacent groups
    bool changed = true;
    while (changed)
    {
        changed = false;
        map<int, vector<string>> newGroupedMinterms;

        for (auto &group : groupedMinterms)
        {
            for (const string &term1 : group.second)
            {
                bool combined = false;
                if (groupedMinterms.find(group.first + 1) != groupedMinterms.end())
                {
                    for (const string &term2 : groupedMinterms[group.first + 1])
                    {
                        if (differByOneBit(term1, term2))
                        {
                            changed = true;
                            combined = true;
                            newGroupedMinterms[group.first].push_back(combineMinterms(term1, term2));
                        }
                    }
                }
                if (!combined)
                {
                    primeImplicants.push_back(term1);
                }
            }
        }
        groupedMinterms = newGroupedMinterms;
    }

    return primeImplicants;
}

// Function to report the number of Essential Prime Implicants
vector<string> essentialPrimeImplicants(BooleanExpression expression)
{
    int essentialCount = 0;
    map<int, int> mintermCount;
    vector<string> exprPI = primeImplicants(expression);
    vector<string> essentialPrimeImplicants;

    // initialize counter for each minterm
    for (int term : expression.minterms)
    {
        mintermCount[term] = 0;
    }

    // Count how many prime implicants cover each minterm
    for (const string &prime : exprPI)
    {
        for (int term : expression.minterms)
        {
            if (isCovered(decimalToBinary(term, expression.variables.size()), prime))
            {
                mintermCount[term]++;
            }
        }
    }

    // Identify essential prime implicants
    for (const string &prime : exprPI)
    {
        for (int term : expression.minterms)
        {
            if (mintermCount[term] == 1 && isCovered(decimalToBinary(term, expression.variables.size()), prime))
            {
                essentialCount++;
                essentialPrimeImplicants.push_back(prime);
                break;
            }
        }
    }

    cout << "Number of Essential Prime Implicants: " << essentialCount << endl;
    return exprPI;
}

// Function to report the number of ON-Set minterms
void onSetMinterms(BooleanExpression expression)
{
    cout << "number of ON-Set minterms: " << expression.minterms.size() << endl;
}

// Function to report the number of ON-Set maxterms
void onSetMaxterms(BooleanExpression expression)
{
    cout << "number of ON-Set maxterms: " << expression.maxterms.size() << endl;
}

// function to show ascii SOP circuit diagram
void asciiSOP(BooleanExpression expression)
{
    vector<vector<string>> lines(expression.minterms.size(), vector<string>(expression.variables.size(), ""));

    for (int m = 0; m < expression.minterms.size(); ++m)
    {
        int minterm = expression.minterms[m];
        string binary = decimalToBinary(minterm, expression.variables.size());

        bool first = true;
        for (int i = 0; i < binary.size(); ++i)
        {
            if (binary[i] == '1')
            {
                lines[m][i] += "-------";
            }
            else
            {
                lines[m][i] += "--|>o--";
            }
            lines[m][i] += "|&&";
            if (first)
            {
                lines[m][i] += "-------|OR";
                first = false;
            }
            else
                lines[m][i] += "       |OR";
        }
    }

    // append "-------|OR" to the end of each line
    bool result = true;
    // Print the lines and append the OR gate
    for (int m = 0; m < expression.minterms.size(); ++m)
    {
        for (int i = 0; i < expression.variables.size(); ++i)
        {
            cout << expression.variables[i] << ": " << lines[m][i];
            cout << endl;
        }
        // if this line is halfway through the minterm, print the or gate and expression
        if (result)
        {
            result = false;
            cout << "                    |OR----- ";
            for (unsigned int i = 0; i < expression.minterms.size(); i++)
            {
                int minterm = expression.minterms[i];
                string binary = bitset<16>(minterm).to_string();
                binary = binary.substr(16 - expression.variables.size());

                for (unsigned int j = 0; j < binary.size(); j++)
                {
                    if (binary[j] == '0')
                    {
                        cout << "!" << expression.variables[j];
                    }
                    else
                    {
                        cout << expression.variables[j];
                    }
                }

                if (i != expression.minterms.size() - 1)
                {
                    cout << " + ";
                }
            }
            cout << endl;
        }
        else
        {
            cout << "                    |OR" << endl;
        }
    }
}

// function to show ascii POS circuit diagram
void asciiPOS(BooleanExpression expression)
{
    vector<vector<string>> lines(expression.maxterms.size(), vector<string>(expression.variables.size(), ""));

    for (int m = 0; m < expression.maxterms.size(); ++m)
    {
        int maxterm = expression.maxterms[m];
        string binary = decimalToBinary(maxterm, expression.variables.size());

        bool first = true;
        for (int i = 0; i < binary.size(); ++i)
        {
            if (binary[i] == '0')
            {
                lines[m][i] += "-------";
            }
            else
            {
                lines[m][i] += "--|>o--";
            }
            lines[m][i] += "|OR";
            if (first)
            {
                lines[m][i] += "-------|&&";
                first = false;
            }
            else
                lines[m][i] += "       |&&";
        }
    }

    // append "-------|&&" to the end of each line
    bool result = true;
    // Print the lines and append the AND gate
    for (int m = 0; m < expression.maxterms.size(); ++m)
    {
        for (int i = 0; i < expression.variables.size(); ++i)
        {
            cout << expression.variables[i] << ": " << lines[m][i];
            cout << endl;
        }
        // if this line is halfway through the minterm, print the or gate and expression
        if (result)
        {
            result = false;
            cout << "                    |&&----- ";
            for (unsigned int i = 0; i < pow(2, expression.variables.size()); i++)
            {
                // Check if i is not a minterm
                if (find(expression.minterms.begin(), expression.minterms.end(), i) == expression.minterms.end())
                {
                    string binary = bitset<16>(i).to_string();
                    binary = binary.substr(16 - expression.variables.size());

                    cout << '(';
                    for (unsigned int j = 0; j < binary.size(); j++)
                    {
                        if (binary[j] == '0')
                        {
                            cout << expression.variables[j];
                            if (j != binary.size() - 1)
                                cout << " + ";
                        }
                        else
                        {
                            cout << "!" << expression.variables[j];
                            if (j != binary.size() - 1)
                                cout << " + ";
                        }
                    }
                    cout << ")";
                    if (i != pow(2, expression.variables.size()) - 1)
                    {
                        cout << " * ";
                    }
                }
            }
            cout << endl;
        }
        else
        {
            cout << "                    |&&" << endl;
        }
    }
}
