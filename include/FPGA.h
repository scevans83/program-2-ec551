#ifndef FPGA_H
#define FPGA_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "LUT.h"
#include "../include/tools.h"

using namespace std;

class FPGA
{
public:
    // Constructor
    FPGA(int numLUTs, int bit_size);

    // Destructor
    ~FPGA();

    // Method to add a new LUT
    void addLUT(int bit_size, const string &name);

    // Method to connect an input to a specific LUT
    void connectInputToLUT(int lut_index, int input_index, const LUT *source_LUT);

    // Method to connect the output of a specific LUT to another LUT
    void connectOutputOfLUT(int source_LUT_index, LUT *targetLUT);

    // Method to print the connections of all LUTs
    void printConnections() const;

    // Method to generate bitstream from current LUTs
    string generateBitstream();

    // Method to write bitstream to a file
    void writeBitstreamToFile();

    // Helper functions to generate bitstream
    string intToBinaryString(int num);

    void padWithZeros(string &str, int max_size);

    void readBitstreamFromFile(const string &filename);

    void setLUTBooleanExpression(int lut_index, const string &expression);

    bool evaluateLUT(int lut_index, const vector<bool> &inputs) const;

    void setExternalInputs(const vector<bool> &inputs);

    vector<bool> evaluateFPGA(const vector<bool> &externalInputs);

    LUT *getLUT(int lut_index) const;

    LUT *getInputLUT(int input_index) const;

    LUT *getOutputLUT(int output_index) const;

    void addInputs(string name);

    void addOutputs(string name);

protected:
    vector<LUT *> luts;
    vector<LUT *> input_luts;
    vector<LUT *> output_luts;
    vector<bool> external_inputs;
    vector<bool> external_outputs;
};

#endif // FPGA_H