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

/*
General bitstream structure
3 bits: bitsize (either 4 or 6)
10 bits: number of luts in binary
10 bits: number of inputs in binary
10 bits: number of outputs in binary
Remaining is individual luts starting with luts[0]
total size = truthTable.size() + luts.size() + input_luts.size() + luts.size() + output_luts.size()
encoding for input/output connections: 1 where connection is made in relation to luts then external signal 

*/

string FPGA::generateBitstream(){
    string bit_stream = "";
    string num_Luts;
    string num_Inputs;
    string num_Outputs;
    int max_bits_num_Luts = 10;
    int max_bits_num_Inputs = 10;
    int max_bits_num_Outputs = 10;
    
    // Add the size of the LUTs in binary 4(100) or 6(110)
    bit_stream += intToBinaryString(luts[0]->getBitSize());

    // Convert num of LUTs to string
    num_Luts = intToBinaryString(luts.size());

    // Pad num of LUTs with zeros
    padWithZeros(num_Luts, max_bits_num_Luts);
    
    // Add padded num of LUTs to bitstream 
    bit_stream += num_Luts;

    // Convert num of inputs to string
    num_Inputs = intToBinaryString(external_inputs.size());

    // Pad num of inputs with zeros
    padWithZeros(num_Inputs, max_bits_num_Inputs);
    
    // Add padded num of inputs to bitstream 
    bit_stream += num_Inputs;

    // Convert num of outputs to string
    num_Outputs = intToBinaryString(external_outputs.size());

    // Pad num of outputs with zeros
    padWithZeros(num_Outputs, max_bits_num_Outputs);
    
    // Add padded num of outputs to bitstream 
    bit_stream += num_Outputs;

    // calculate number of total luts, inputs, and ouputs for mapping 
    int input_connections_size = (luts.size() + external_inputs.size());
    int output_connections_size = (luts.size() + external_outputs.size());

    //iterate over each lut
    //compare its inputs' names to the lut names and external input names
    //where they match add the index and then covert all '0' at those indexes to '1'

    for (int i = 0; i < luts.size(); ++i)
    {   
        //Convert TT to string and add to bitstream
        string truth_table_string = convertBoolVecToString(luts[i]->getTruthTable());
        bit_stream += truth_table_string;

        //Convert inputs to string of 1's located at connections (LUTs then external inputs)
        string input_connections = string(input_connections_size,'0');
        vector<int> input_connection_indexes;

        vector<const LUT*> lut_i_input_connections = luts[i]->getInputConnections();

        for(int j = 0; j < lut_i_input_connections.size(); ++j){
            //check if input matches any in LUTs
            for(int k = 0; k < luts.size(); ++k){
                if(lut_i_input_connections[j]->getName() == luts[k]->getName()){
                    input_connection_indexes.push_back(k);
                }
            }
            //check if input matches any inputs
            for(int k = 0; k < input_luts.size(); ++k){
                if(lut_i_input_connections[j]->getName() == input_luts[k]->getName()){
                    input_connection_indexes.push_back(k+luts.size()); //add size of luts since luts come before external signals
                }
            }
        }

        //replace '0's with '1' at indexes
        for(int j = 0; j < input_connection_indexes.size(); ++j){
            input_connections[input_connection_indexes[j]] = '1';
        }

        //add input encoding to bitstream
        bit_stream += input_connections;

        //convert output to string of 0's with 1 located at connection point
        string output_connections = string(output_connections_size,'0');
        int output_connection_index;

        LUT* lut_i_output_connection = luts[i]->getOutputConnection();

        //Check if connection is in luts and add index
        for(int j = 0; j < luts.size(); ++j){
            if(lut_i_output_connection->getName() == luts[j]->getName()){
                output_connection_index = j;
            }
        }

        //check if connection is an output and add index
        for(int j = 0; j < output_luts.size(); ++j){
            if(lut_i_output_connection->getName() == output_luts[j]->getName()){
                output_connection_index = (j + luts.size()); //add size of luts since luts come before external signals
            }
        }

        output_connections[output_connection_index] = '1';

        //add output encoding to bitstream
        bit_stream += output_connections;
    }

    return bit_stream;
}

void FPGA::writeBitstreamToFile()
{
    string file_name;
    string bitstream;
    
    cout << "File name:";
    cin >> file_name;

    ofstream output;

    output.open(file_name, ios::out);

    output << generateBitstream() << endl;

    output.close();

}

string FPGA::intToBinaryString(int num){
    string binary_string;

    // Determine num of bits needed
    int num_bits = sizeof(int) * 8;

    // Extract bits
    for(int i = num_bits -1; i >= 0; --i){
        binary_string += ((num >> i) & 1)? '1':'0';
    }

    return binary_string;
}

void FPGA::padWithZeros(string& str, int max_size){
    if(str.size() < max_size){
        //calc number of zeros to add
        int zeros_to_add = max_size - str.size();

        //insert zeros to start of string
        str.insert(0, zeros_to_add, '0');
    }
}