#ifndef LUT_H
#define LUT_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "../include/tools.h"

using namespace std;

class LUT
{
public:
    // Constructor for internal LUT
    LUT(int bit_size, const string &name);

    // Constructor for external input/output
    LUT(bool is_input, const string &name);

    // Destructor
    ~LUT();

    // Method to set the boolean expression for the LUT
    void setBooleanExpression(const string &expression);

    void setTruthTable(const vector<bool> &truth_table);

    // Method to evaluate the LUT based on input
    bool evaluate(const vector<bool> &inputs) const;

    // Method to connect an input to this LUT
    void connectInput(int input_index, const LUT *source_LUT);

    // Method to connect the output of this LUT to another
    void connectOutput(LUT *target_LUT);

    // Method to print connected inputs and ouputs
    void printConnections() const;

    // Method to get size of lut
    int getBitSize() const;

    // Method to get truth table
    vector<bool> getTruthTable() const;

    // Method to get input connections
    vector<const LUT *> getInputConnections() const;

    // Method to get output connection
    LUT *getOutputConnection() const;

    // Method to get name of lut
    const string &getName() const;

    bool getIsInput() const;

    bool getIsOutput() const;

private:
    string name;
    int bit_size;
    bool is_input;                         // flag indicating if its an external input
    bool is_output;                        // flag indicating if its an external output
    vector<bool> truth_table;              // truth table stored in LUT
    vector<const LUT *> input_connections; // vector of input LUTs/external inputs
    LUT *output_connection;                // output LUT/external output
    vector<string> input_names;
};

#endif // LUT_H