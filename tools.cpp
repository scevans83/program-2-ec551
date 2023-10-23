#include "tools.h"

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
