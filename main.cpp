#include <iostream>
#include <vector>
#include <string>
#include "include/LUT.h"
#include "include/FPGA.h"

using namespace std;

int main()
{
    cout << "How many inputs/outputs?" << endl;
    int num_inputs;
    int num_outputs;

    cout << "Inputs: ";
    cin >> num_inputs;
    cout << "Outputs: ";
    cin >> num_outputs;

    cout << "How many LUTs?" << endl;
    int num_Luts;

    cout << "LUTs: ";
    cin >> num_Luts;

    cout << "4-bit or 6-bit LUTs?" << endl;
    int lut_size;
    cout << "LUT size: ";
    cin >> lut_size;

    int valid_entry = 0;
    if (lut_size == 4 || lut_size == 6)
    {
        valid_entry = 1;
    }
    if (valid_entry != 1)
    {
        cout << "Invalid LUT size entered!";
        return -1;
    }

    cout << "Creating FPGA with " << num_Luts << " fully connected " << lut_size << "-bit LUTs with " << num_inputs << " inputs and " << num_outputs << " outputs..." << endl;

    return 0;
}