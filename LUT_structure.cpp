#include <iostream>
#include <vector>
#include <string>

using namespace std;

class LUT{
public:
    //Constructor for internal LUT
    LUT(int bit_size, const string& name);

    //Constructor for external input/output
    LUT(bool is_input, const string& name);

    //Destructor
    ~LUT();

    //Method to set the boolean expression for the LUT
    void setBooleanExpression(const string& expression);

    //Method to evaluate the LUT based on input 
    bool evaluate(const vector<bool> & inputs) const;

    //Method to connect an input to this LUT
    void connectInput(int input_index, const LUT* source_LUT);

    //Method to connect the output of this LUT to another
    void connectOutput(LUT* target_LUT);

    //Method to print connected inputs and ouputs
    void printConnections() const;

    //Method to get size of lut
    int getBitSize() const;

    //Method to get name of lut
    const string& getName() const;

private:
    string name;
    int bit_size;
    bool is_input;                           //flag indicating if its an external input
    bool is_output;                          //flag indicating if its an external output
    vector<bool> truth_table;                //truth table stored in LUT  
    vector<const LUT*> input_connections;    //vector of input LUTs/external inputs
    LUT* output_connection;                  //output LUT/external output

};

class FPGA{
public:
    //Constructor
    FPGA(int numLUTs, int bit_size);

    //Destructor
    ~FPGA();

    //Method to add a new LUT
    void addLUT(int bit_size, const string& name);

    //Method to connect an input to a specific LUT
    void connectInputToLUT(int lut_index, int input_index, const LUT* source_LUT);

    //Method to connect the output of a specific LUT to another LUT
    void connectOutputOfLUT(int source_LUT_index, LUT* targetLUT);

    //Method to print the connections of all LUTs 
    void printConnections() const;
private:
    vector<LUT*> luts;
    vector<bool> external_inputs;
    vector<bool> external_outputs;
};

LUT::LUT(int bit_size, const string& name)
    : bit_size(bit_size), name(name), is_input(false), is_output(false), output_connection(nullptr) {
    input_connections.resize(bit_size, nullptr);
}

LUT::LUT(bool is_input, const string&name)
    : bit_size(1), name(name), is_input(is_input), is_output(!is_input), output_connection(nullptr) {
    input_connections.resize(1, nullptr);
}

LUT::~LUT(){

}

void LUT::setBooleanExpression(const string& expression){

}

bool LUT::evaluate(const vector<bool> & inputs) const{

}

void LUT::connectInput(int input_index, const LUT* source_LUT){
    if(input_index >= 0 && input_index < bit_size){
        input_connections[input_index] = source_LUT;
    }
    else{
        cerr << "Invalid input connection" << endl;
    }
}

void LUT::connectOutput(LUT* target_LUT){
    output_connection = target_LUT;
}

void LUT::printConnections() const{
    cout << "Inputs: " << endl;
    for(int i = 0; i < bit_size; ++i){
        if(input_connections[i]){
            cout << input_connections[i] << " "; //print out what is connected there may need to name LUTs/Inputs
        }
        else{
            cout << "X "; //print X if no connection
        }
    }
    cout << endl;

    if(output_connection){
        cout << "Output: " << output_connection << endl;
    }
    else{
        cout << "Output: X" << endl;
    }
}

int LUT::getBitSize() const{
    return bit_size;
}

const string& LUT::getName() const{
    return name;
}


FPGA::FPGA(int num_LUTs, int bit_size){
    for(int i = 0; i < num_LUTs; ++i){
        addLUT(bit_size, "LUT" + to_string(i));
    }
}

FPGA::~FPGA(){
    for(auto lut : luts){
        delete lut;
    }
}

void FPGA::addLUT(int bit_size, const string& name){
    luts.emplace_back(new LUT(bit_size, name));
}

void FPGA::connectInputToLUT(int lut_index, int input_index, const LUT* source_LUT){
    if(lut_index >= 0 && lut_index < luts.size()){
        luts[lut_index]->connectInput(input_index, source_LUT);
    }
    else{
        cerr << "Invalid LUT input connection" << endl;
    }
}

void FPGA::connectOutputOfLUT(int source_LUT_index, LUT* target_LUT){
    if(source_LUT_index >= 0 && source_LUT_index < luts.size()){
        luts[source_LUT_index]->connectOutput(target_LUT);
    }
    else{
        cerr << "Invalid LUT output connection";
    }
}

void FPGA::printConnections() const{
    cout << "FPGA Connections:" << endl;

    for(size_t i = 0; i < luts.size(); ++i){
        luts[i]->printConnections();
    }
}

int main(){
    cout << "How many inputs/outputs?" << endl;
    int num_inputs;
    int num_outputs;

    cout << "Inputs: ";
    cin >> num_inputs;
    cout << "Outputs: ";
    cin >> num_outputs;

    cout << "How many LUTs?" << endl;
    int num_Luts;

    cout <<"LUTs: ";
    cin >> num_Luts;

    cout << "4-bit or 6-bit LUTs?" << endl;
    int lut_size;
    cout << "LUT size: ";
    cin >> lut_size;

    
    int valid_entry = 0;
    if(lut_size == 4 || lut_size == 6){
        valid_entry = 1;
    }
    if(valid_entry != 1){
        cout << "Invalid LUT size entered!";
        return -1;
    }

    cout << "Creating FPGA with " << num_Luts << " fully connected " << lut_size << "-bit LUTs with " << num_inputs << " inputs and " << num_outputs << " outputs..." << endl;


    return 0; 

}