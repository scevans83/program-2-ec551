#include "../include/FPGA.h"

FPGA::FPGA(int num_LUTs, int bit_size)
{
    for (int i = 0; i < num_LUTs; ++i)
    {
        addLUT(bit_size, "LUT" + to_string(i));
    }
}

FPGA::~FPGA()
{
    for (auto lut : luts)
    {
        delete lut;
    }
}

void FPGA::addLUT(int bit_size, const string &name)
{
    luts.emplace_back(new LUT(bit_size, name));
}

void FPGA::connectInputToLUT(int lut_index, int input_index, const LUT *source_LUT)
{
    if (lut_index >= 0 && lut_index < luts.size())
    {
        luts[lut_index]->connectInput(input_index, source_LUT);
    }
    else
    {
        cerr << "Invalid LUT input connection" << endl;
    }
}

void FPGA::connectOutputOfLUT(int source_LUT_index, LUT *target_LUT)
{
    if (source_LUT_index >= 0 && source_LUT_index < luts.size())
    {
        luts[source_LUT_index]->connectOutput(target_LUT);
    }
    else
    {
        cerr << "Invalid LUT output connection";
    }
}

void FPGA::printConnections() const
{
    cout << "FPGA Connections:" << endl;

    for (size_t i = 0; i < luts.size(); ++i)
    {
        luts[i]->printConnections();
    }
}