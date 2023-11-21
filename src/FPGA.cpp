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
        cout << "LUT number " << i << "'s connections: " << endl;
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

string FPGA::generateBitstream()
{
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

    // iterate over each lut
    // compare its inputs' names to the lut names and external input names
    // where they match add the index and then covert all '0' at those indexes to '1'

    for (int i = 0; i < luts.size(); ++i)
    {
        // Convert TT to string and add to bitstream
        string truth_table_string = convertBoolVecToString(luts[i]->getTruthTable());
        bit_stream += truth_table_string;

        // Convert inputs to string of 1's located at connections (LUTs then external inputs)
        string input_connections = string(input_connections_size, '0');
        vector<int> input_connection_indexes;

        vector<const LUT *> lut_i_input_connections = luts[i]->getInputConnections();

        for (int j = 0; j < lut_i_input_connections.size(); ++j)
        {
            // check if input matches any in LUTs
            for (int k = 0; k < luts.size(); ++k)
            {
                if (lut_i_input_connections[j]->getName() == luts[k]->getName())
                {
                    input_connection_indexes.push_back(k);
                }
            }
            // check if input matches any inputs
            for (int k = 0; k < input_luts.size(); ++k)
            {
                if (lut_i_input_connections[j]->getName() == input_luts[k]->getName())
                {
                    input_connection_indexes.push_back(k + luts.size()); // add size of luts since luts come before external signals
                }
            }
        }

        // replace '0's with '1' at indexes
        for (int j = 0; j < input_connection_indexes.size(); ++j)
        {
            input_connections[input_connection_indexes[j]] = '1';
        }

        // add input encoding to bitstream
        bit_stream += input_connections;

        // convert output to string of 0's with 1 located at connection point
        string output_connections = string(output_connections_size, '0');
        int output_connection_index;

        LUT *lut_i_output_connection = luts[i]->getOutputConnection();

        // Check if connection is in luts and add index
        for (int j = 0; j < luts.size(); ++j)
        {
            if (lut_i_output_connection->getName() == luts[j]->getName())
            {
                output_connection_index = j;
            }
        }
        cout << "debug: this is the part where it adds indices for external inputs" << endl;
        // check if connection is an output and add index
        for (int j = 0; j < output_luts.size(); ++j)
        {
            if (lut_i_output_connection->getName() == output_luts[j]->getName())
            {
                output_connection_index = (j + luts.size()); // add size of luts since luts come before external signals
            }
        }

        output_connections[output_connection_index] = '1';

        // add output encoding to bitstream
        bit_stream += output_connections;
    }

    cout << bit_stream << endl;
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

string FPGA::intToBinaryString(int num)
{
    string binary_string;

    // Determine num of bits needed
    int num_bits = sizeof(int) * 8;

    // Extract bits
    for (int i = num_bits - 1; i >= 0; --i)
    {
        binary_string += ((num >> i) & 1) ? '1' : '0';
    }

    return binary_string;
}

void FPGA::padWithZeros(string &str, int max_size)
{
    if (str.size() < max_size)
    {
        // calc number of zeros to add
        int zeros_to_add = max_size - str.size();

        // insert zeros to start of string
        str.insert(0, zeros_to_add, '0');
    }
}

void FPGA::readBitstreamFromFile(const string &filename)
{
    ifstream input(filename);
    string bitstream;

    if (input)
    {
        // read the entire file content into a string
        stringstream buffer;
        buffer << input.rdbuf();
        bitstream = buffer.str();

        // clear current FPGA state just in case
        for (auto lut : luts)
        {
            delete lut;
        }
        luts.clear();
        input_luts.clear();
        output_luts.clear();
        external_inputs.clear();
        external_outputs.clear();

        size_t pos = 0;

        // extract bit size, number of LUTs, inputs, and outputs
        int bitSize = stoi(bitstream.substr(pos, 3), nullptr, 2);
        pos += 3;
        int numLUTs = stoi(bitstream.substr(pos, 10), nullptr, 2);
        pos += 10;
        int numInputs = stoi(bitstream.substr(pos, 10), nullptr, 2);
        pos += 10;
        int numOutputs = stoi(bitstream.substr(pos, 10), nullptr, 2);
        pos += 10;

        // construct LUTs and connections
        for (int i = 0; i < numLUTs; i++)
        {
            LUT *lut = new LUT(bitSize, "LUT" + to_string(i));
            luts.push_back(lut);
        }

        for (int i = 0; i < numLUTs; ++i)
        {
            // construct truth table
            vector<bool> truthTable;
            for (int j = 0; j < pow(2, bitSize); ++j)
            {
                truthTable.push_back(bitstream[pos] == '1');
                ++pos;
            }
            luts[i]->setTruthTable(truthTable);
        }

        // construct connections between LUTs
    }
    else
    {
        cerr << "Failed to open bitstream file: " + filename << endl;
    }
}

void FPGA::setLUTBooleanExpression(int lut_index, const string &expression)
{
    if (lut_index >= 0 && lut_index < luts.size())
    {
        luts[lut_index]->setBooleanExpression(expression);
    }
    else
    {
        cerr << "Invalid LUT index" << endl;
    }
}

bool FPGA::evaluateLUT(int lut_index, const vector<bool> &inputs) const
{
    if (lut_index >= 0 && lut_index < luts.size())
    {
        return luts[lut_index]->evaluate(inputs);
    }
    else
    {
        cerr << "Invalid LUT index" << endl;
        return false;
    }
}

void FPGA::setExternalInputs(const vector<bool> &inputs)
{
    external_inputs = inputs;
}

vector<bool> FPGA::evaluateFPGA(const vector<bool> &externalInputs)
{
    setExternalInputs(externalInputs);

    // initialize a vector to store the outputs of each LUT
    vector<bool> outputs(luts.size());

    // evaluate each LUT based on its input connections
    for (int i = 0; i < luts.size(); ++i)
    {
        vector<bool> lutInputs;

        // gather inputs for this LUT from external inputs or other LUTs
        for (const auto &inputConnection : luts[i]->getInputConnections())
        {
            // determine if the connection is external or from another LUT and add the corresponding value to 'lutInputs'
            if (inputConnection->getIsInput())
            {
                lutInputs.push_back(external_inputs[inputConnection->getName()[3] - '0']);
            }
            else
            {
                lutInputs.push_back(outputs[inputConnection->getName()[3] - '0']);
            }
        }

        // Evaluate the LUT with its inputs and store the output
        outputs[i] = luts[i]->evaluate(lutInputs);
    }

    return outputs;
}

LUT *FPGA::getLUT(int lut_index) const
{
    if (lut_index >= 0 && lut_index < luts.size())
    {
        return luts[lut_index];
    }
    else
    {
        cerr << "Invalid LUT index" << endl;
        return nullptr;
    }
}

void FPGA::addInputs(int bit_size)
{
    for (int i = 0; i < bit_size; ++i)
    {
        input_luts.emplace_back(new LUT(true, "IN" + to_string(i)));
    }
}

LUT *FPGA::getInputLUT(int input_index) const
{
    if (input_index >= 0 && input_index < input_luts.size())
    {
        return input_luts[input_index];
    }
    else
    {
        cerr << "Invalid input index" << endl;
        return nullptr;
    }
}

LUT *FPGA::getOutputLUT(int output_index) const
{
    if (output_index >= 0 && output_index < output_luts.size())
    {
        return output_luts[output_index];
    }
    else
    {
        cerr << "Invalid output index" << endl;
        return nullptr;
    }
}

void FPGA::addOutputs(int bit_size)
{
    for (int i = 0; i < bit_size; ++i)
    {
        output_luts.emplace_back(new LUT(false, "OUT" + to_string(i)));
    }
}

void FPGA::makeConnections(vector<LUT *> luts){
    for(int i = 0; i < luts.size(); ++i){

        //Make input connections for lut[i]
        vector <char> lut_input_names = luts[i]->getInputNames();
        //iterate over input names
        for(int j = 0; j < lut_input_names.size(); ++j){
            //check if name is in LUT vector
            for(int k = 0; k < luts.size(); ++k){ 
                //if equal make the connection, make sure lut is not connecting to itself
                if((lut_input_names[j] == luts[k]->getName()[0]) && (i != k)){
                    //connect lut at index k, to lut[i] at lut[i]'s input port j
                    connectInputToLUT(i, j, luts[k]);
                }
            }
        }
        //repeat for input_luts vector
        for(int j = 0; j < lut_input_names.size(); ++j){
            //check if name is in input LUT vector
            for(int k = 0; k < input_luts.size(); ++k){ 
                //if equal make the connection
                if((lut_input_names[j] == input_luts[k]->getName()[0])){
                    //connect lut at index k, to lut[i] at lut[i]'s input port j
                    connectInputToLUT(i, j, input_luts[k]);
                }
            }
        }

        //Make output connection
        for(int j = 0; j < luts.size(); ++j){ 
            //lut name should be output variable, comapre with lut names
            if((luts[i]->getName() == luts[j]->getName()) && (i != j)){
                connectOutputOfLUT(i, luts[j]);
            }
        }

        //repeat for output_luts vector
        for(int j = 0; j < output_luts.size(); ++j){
            if((luts[i]->getName() == output_luts[j]->getName())){
                connectOutputOfLUT(i, output_luts[j]);
            }
        }

    }
}