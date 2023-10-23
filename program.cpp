#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

struct BooleanExpression
{
    vector<int> minterms;
    vector<char> variables;
    vector<int> maxterms;
};

struct Term
{
    int value;
    string binaryRepresentation;
    bool isCombined;
};

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

vector<int> getMaxterms(BooleanExpression expression)
{
    vector<int> maxterms;
    for (int i = 0; i < pow(2, expression.variables.size()); i++)
    {
        if (find(expression.minterms.begin(), expression.minterms.end(), i) == expression.minterms.end())
        {
            maxterms.push_back(i);
        }
    }
    return maxterms;
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

string decimalToBinary(int n, int numVars)
{
    string binary = bitset<16>(n).to_string();
    return binary.substr(16 - numVars);
}

int countOnes(string bin)
{
    return count(bin.begin(), bin.end(), '1');
}

int countZeros(string bin)
{
    return count(bin.begin(), bin.end(), '0');
}

bool differByOneBit(string a, string b)
{
    int diff = 0;
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] != b[i])
        {
            diff++;
        }
    }
    return diff == 1;
}

string combineMinterms(string a, string b)
{
    string result = a;
    for (int i = 0; i < a.size(); ++i)
    {
        if (a[i] != b[i])
        {
            result[i] = 'X';
        }
    }
    return result;
}

string combineMaxterms(string a, string b)
{
    string result = a;
    for (int i = 0; i < a.size(); ++i)
    {
        if (a[i] != b[i])
        {
            result[i] = 'X';
        }
    }
    return result;
}

bool isCovered(string minterm, string primeImplicant)
{
    for (int i = 0; i < minterm.length(); ++i)
    {
        if (primeImplicant[i] != '-' && minterm[i] != primeImplicant[i])
        {
            return false;
        }
    }
    return true;
}

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
}

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

// This function returns a vector of prime implicants that cover the uncovered minterms with the least literals.
vector<string> selectMinimumLiterals(vector<int> uncoveredMinterms, vector<string> primeImplicants)
{
    vector<string> minimumLiterals;
    int minLiterals = 1000000;
    for (int i = 0; i < pow(2, uncoveredMinterms.size()); i++)
    {
        string binary = bitset<16>(i).to_string();
        binary = binary.substr(16 - uncoveredMinterms.size(), uncoveredMinterms.size());
        vector<string> literals;
        for (int j = 0; j < binary.size(); j++)
        {
            if (binary[j] == '1')
            {
                literals.push_back(primeImplicants[j]);
            }
        }
        set<int> coveredMinterms;
        for (const string &prime : literals)
        {
            for (int term : uncoveredMinterms)
            {
                if (isCovered(decimalToBinary(term, literals[0].size()), prime))
                {
                    coveredMinterms.insert(term);
                }
            }
        }
        if (coveredMinterms.size() == uncoveredMinterms.size() && literals.size() < minLiterals)
        {
            minLiterals = literals.size();
            minimumLiterals = literals;
        }
    }
    return minimumLiterals;
}

vector<string> minimizedSOP(BooleanExpression expression)
{
    string sop = "";

    int numLiterals = expression.minterms.size() * expression.variables.size();
    int numLiteralsNew;

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

            if (optimizedSOP[i][j] == '0')
            {
                numLiteralsNew++;
                cout << '!' << expression.variables[j];
            }
            else if (optimizedSOP[i][j] == '1')
            {
                numLiteralsNew++;
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

vector<string> primeImplicantsForMaxterms(BooleanExpression expression)
{
    vector<string> maxtermsBinary;
    vector<string> primeImplicants;

    for (int term : expression.maxterms)
    {
        maxtermsBinary.push_back(decimalToBinary(term, expression.variables.size()));
    }

    map<int, vector<string>> groupedMaxterms;
    for (const string &binTerm : maxtermsBinary)
    {
        groupedMaxterms[countZeros(binTerm)].push_back(binTerm);
    }

    bool changed = true;
    while (changed)
    {
        changed = false;
        map<int, vector<string>> newGroupedMaxterms;

        for (auto &group : groupedMaxterms)
        {
            for (const string &term1 : group.second)
            {
                bool combined = false;
                if (groupedMaxterms.find(group.first + 1) != groupedMaxterms.end())
                {
                    for (const string &term2 : groupedMaxterms[group.first + 1])
                    {
                        if (differByOneBit(term1, term2))
                        {
                            changed = true;
                            combined = true;
                            newGroupedMaxterms[group.first].push_back(combineMaxterms(term1, term2));
                        }
                    }
                }
                if (!combined)
                {
                    primeImplicants.push_back(term1);
                }
            }
        }
        groupedMaxterms = newGroupedMaxterms;
    }

    return primeImplicants;
}

bool isCoveredByMaxterm(string maxterm, string primeImplicant)
{
    cout << "Maxterm: " << maxterm << ", Prime Implicant: " << primeImplicant << endl;

    for (int i = 0; i < maxterm.length(); ++i)
    {
        if (primeImplicant[i] != '-' && maxterm[i] != primeImplicant[i])
        {
            return false;
        }
    }
    return true;
}

// This function returns a vector of prime implicants that cover the uncovered maxterms with the least literals.
vector<string> selectMinimumLiteralsMaxterm(vector<int> uncoveredMaxterms, vector<string> primeImplicants)
{
    vector<string> minimumLiterals;
    int minLiterals = 1000000;
    for (int i = 0; i < pow(2, uncoveredMaxterms.size()); i++)
    {
        string binary = bitset<16>(i).to_string();
        binary = binary.substr(16 - uncoveredMaxterms.size(), uncoveredMaxterms.size());
        vector<string> literals;
        for (int j = 0; j < binary.size(); j++)
        {
            if (binary[j] == '1')
            {
                literals.push_back(primeImplicants[j]);
            }
        }
        set<int> coveredMaxterms;
        for (const string &prime : literals)
        {
            for (int term : uncoveredMaxterms)
            {
                cout << "Current literals: ";
                for (const auto &literal : literals)
                {
                    cout << literal << ", ";
                }
                cout << endl;

                if (isCoveredByMaxterm(decimalToBinary(term, literals[0].size()), prime))
                {
                    coveredMaxterms.insert(term);
                }
            }
        }

        if (coveredMaxterms.size() == uncoveredMaxterms.size() && literals.size() < minLiterals)
        {
            cout << "Updating minLiterals to " << literals.size() << endl;

            minLiterals = literals.size();
            minimumLiterals = literals;
        }
    }
    return minimumLiterals;
}

// Function to return a minimized number of literals representation in POS
vector<string> minimizedPOS(BooleanExpression expression)
{
    string pos = "";

    int numLiterals = expression.maxterms.size() * expression.variables.size();
    int numLiteralsNew;

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

int main()
{
    BooleanExpression expression;
    map<string, pair<string, vector<string>>> circuit;

    cout << "What would you like to do?\n";
    cout << "1. Input a Boolean algebraic function\n";
    cout << "2. Input a digital combinational logic circuit\n";
    cout << "Enter choice (1 or 2): ";

    int choice;
    cin >> choice;
    cin.ignore();

    string inputData;
    if (choice == 1)
    {
        cout << "Enter the Boolean algebraic function: ";
        getline(cin, inputData);
        expression = parseBoolean(inputData);
    }
    else if (choice == 2)
    {
        expression = getInputCircuit();
    }
    else
    {
        cerr << "Invalid choice. Please run the program again." << endl;
        return 1;
    }

    /*prompt the user to select what they want to do from the following list:
1. Return the design as a canonical SOP
2. Return the design as a canonical POS
3. Return the design INVERSE as a canonical SOP
4. Return the design INVERSE as a canonical POS
5. Return a minimized number of literals representation in SOP
a. Report on the number of saved literals vs. the canonical version
6. Return a minimized number of literals representation in POS
a. Report on the number of saved literals vs. the canonical version
7. Report the number of Prime Implicants
8. Report the number of Essential Prime Implicants
9. Report the number of ON-Set minterms
10. Report the number of ON-Set maxterms
*/
    cout << "\nWhat would you like to do with this data?\n";
    cout << "1. Return the design as a canonical SOP\n";
    cout << "2. Return the design as a canonical POS\n";
    cout << "3. Return the design INVERSE as a canonical SOP\n";
    cout << "4. Return the design INVERSE as a canonical POS\n";
    cout << "5. Return a minimized number of literals representation in SOP\n";
    cout << "6. Return a minimized number of literals representation in POS\n";
    cout << "7. Report the number of Prime Implicants\n";
    cout << "8. Report the number of Essential Prime Implicants\n";
    cout << "9. Report the number of ON-Set minterms\n";
    cout << "10. Report the number of ON-Set maxterms\n";
    cout << "\nEnter choice (1-10): ";

    int choice2;
    cin >> choice2;
    cin.ignore();
    vector<string> PI = primeImplicants(expression);

    switch (choice2)
    {
    case 1:
        canonicalSOP(expression);
        break;
    case 2:
        canonicalPOS(expression);
        break;
    case 3:
        inverseCanonicalSOP(expression);
        break;
    case 4:
        inverseCanonicalPOS(expression);
        break;
    case 5:
        minimizedSOP(expression);
        break;
    case 6:
        minimizedPOS(expression);
        break;
    case 7:
        for (const string &prime : PI)
        {
            cout << "Prime Implicant: " << prime << endl;
        }
        cout << "Number of Prime Implicants: " << PI.size() << endl;
        break;
    case 8:
        essentialPrimeImplicants(expression);
        break;
    case 9:
        onSetMinterms(expression);
        break;
    case 10:
        onSetMaxterms(expression);
        break;
    default:
        cerr << "Invalid choice. Please try again." << endl;
        return 1;
    }

    return 0;
}
