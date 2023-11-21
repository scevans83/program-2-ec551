#ifndef TOOLS_H
#define TOOLS_H

#include <iostream>
#include <sstream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

// Define a struct for Boolean expressions
struct BooleanExpression
{
    vector<int> minterms;   // Vector to store minterms
    vector<char> variables; // Vector to store variables
    vector<int> maxterms;   // Vector to store maxterms
};

// Define a struct for terms
struct Term
{
    int value;                   // Value of the term
    string binaryRepresentation; // Binary representation of the term
    bool isCombined;             // Boolean value to check if the term is combined
};

// utility functions
vector<int> getMaxterms(BooleanExpression expression);
string decimalToBinary(int n, int numVars);
int countOnes(string bin);
int countZeros(string bin);
bool differByOneBit(string a, string b);
string combineMinterms(string a, string b);
string combineMaxterms(string a, string b);
bool isCovered(string minterm, string primeImplicant);
vector<string> selectMinimumLiterals(vector<int> uncoveredMinterms, vector<string> primeImplicants);
vector<string> selectMinimumLiteralsMaxterm(vector<int> uncoveredMaxterms, vector<string> primeImplicants);
vector<string> primeImplicantsForMaxterms(BooleanExpression expression);
bool isCoveredByMaxterm(string maxterm, string primeImplicant);
vector<string> generateTruthTable(int numVars);
bool evaluateProductTerm(const string &term, const string &binaryInput, const map<string, int> &inputMap);
string convertBoolVecToString(const vector<bool> &vec);

#endif // TOOLS_H
