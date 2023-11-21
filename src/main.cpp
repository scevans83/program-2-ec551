#include <iostream>
#include <vector>
#include <string>
#include "../include/LUT.h"
#include "../include/FPGA.h"

using namespace std;

int main()
{
    int decision;
    cout << "Generate Manually or from Bitstream?" << endl;
    cout << "(1/2)";
    cin >> decision;
    FPGA fpga(4,4);
    if (decision == 1){
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

        //create input variables vector from user input
        string input_variables;
        cout << "Enter comma-separated input variables: ";
        getline(cin, input_variables);

        vector<string> inputs;
        string value1;

        for(char c: input_variables){
            if(c == ',' || c == '\n'){
                inputs.push_back(value1);

                value1.clear();
            } else{
                value1 += c;
            }
        }

        //create output variables vector from user input
        string output_variables;
        cout << "Enter comma-separated output variables: ";
        getline(cin, output_variables);

        vector<string> outputs;
        string value2;

        for(char c: output_variables){
            if(c == ',' || c == '\n'){
                outputs.push_back(value2);

                value2.clear();
            } else{
                value2 += c;
            }
        }


        cout << "Creating FPGA with " << num_Luts << " fully connected " << lut_size << "-bit LUTs with " << num_inputs << " inputs and " << num_outputs << " outputs..." << endl;
        FPGA fpga_input(num_Luts, lut_size);

        fpga_input.makeFPGAFromTxt();
        fpga = fpga_input;

    }
    if(decision == 2){
        FPGA fpga_bit(4,4); 
        cout << "Input Bitstream file Name: " << endl;
        string bitstream_file;
        cin >> bitstream_file;

        fpga_bit.readBitstreamFromFile(bitstream_file);
        fpga = fpga_bit;

    }
    else{
        cout << "invalid input!" << endl;
    }


    cout << endl;
    cout << "--Functions assigned to the LUTs--" << endl;
    vector<LUT *> luts = fpga.getLUTs();
    for(int i = 0; i < luts.size(); ++i){
        cout << "Expression assigned to LUT " << i << ": " << luts[i]->getName();
    }

    cout << endl;
    cout << "--Internal Connections--" << endl;
    fpga.printConnections();

    cout << endl;
    cout <<"--External Input Assignments--" << endl;
    vector<LUT *> external_inputs = fpga.getInputLuts();
    for(int i = 0; i < external_inputs.size(); ++i){
        cout << "Input " << i << " is connected to variable " << external_inputs[i]->getName();
    }

    cout << endl;
    cout <<"--External Output Assignments--" << endl;
    vector<LUT *> external_outputs = fpga.getOutputLuts();
    for(int i = 0; i < external_outputs.size(); ++i){
        cout << "Output " << i << " is connected to variable " << external_outputs[i]->getName();
    }

    cout << endl;
    cout <<"--Bitstream Written to File--" << endl;
    fpga.writeBitstreamToFile();

    cout << endl;
    cout <<"--Resource Allocation Data--" << endl;
    fpga.resourseAllocation();
    
    
    return 0;
}