//@+leo-ver=5-thin
//@+node:gmc.20080826191619.3: * @thin codequest-dense-1d.cc
//@@language cplusplus

//@+<< Headers >>
//@+node:gmc.20080826191619.4: ** << Headers >>
#include <fstream>

#include "codequest.hpp"

using namespace CodeQuest;
using namespace std;
using namespace boost;
//@-<< Headers >>

#define quantum_operator dynamic_quantum_operator
#define qec qec<dynamic_quantum_operator>

//@+<< I/O >>
//@+node:gmc.20080824181205.25: ** << I/O >>
istream& operator>>(istream& in, quantum_operator& op) {
    string s;
    getline(in,s);
    op.resize(s.length());
    for(int i = 0;  i < s.length(); i++) {
        switch(s[i]) {
            case 'x':
            case 'X':
                op.X.set(i);
                break;
            case 'y':
            case 'Y':
                op.X.set(i);
                op.Z.set(i);
                break;
            case 'z':
            case 'Z':
                op.Z.set(i);
                break;
            case '.':
            case ' ':
            case 'i':
            case 'I':
                break;
            default:
                throw invalid_pauli(s,i);
        }
    }
	return in;
}
//@-<< I/O >>

//@+others
//@+node:gmc.20080826191619.5: ** Functions
//@+node:gmc.20080824181205.30: *3* read_in_operators
int read_in_operators(vector<quantum_operator>& operators, istream& in=cin) {

    //@+<< Read in and parse each of the operator strings >>
    //@+node:gmc.20080826191619.2: *4* << Read in and parse each of the operator strings >>
    int current_line_number = 0;

    try {

        while( in.good() ) {
            current_line_number++;
            quantum_operator op;
            in >> op;
            if(!op.is_identity())
                operators.push_back(op);
        }

    } catch(const invalid_pauli& e) {
        cerr
            << "Error in column " << (e.column+1) << " of line " << current_line_number << endl
            << endl
            << "\t" << e.operator_string << endl
            << "\t";
        for(int j =  0; j < e.column; j++)
            cerr.put(' ');
        cerr
            << "^" << endl
            << endl
            << "Invalid Pauli operator '" << e.operator_string[e.column] << "'." << endl;
        return -1;
    }
    //@-<< Read in and parse each of the operator strings >>

    //@+<< Post-process to make sure all operators have the same length >>
    //@+node:gmc.20080824181205.32: *4* << Post-process to make sure all operators have the same length >>
    size_t system_size = 0;

    for(vector<quantum_operator>::iterator opref = operators.begin(); opref != operators.end(); opref++)
        if(opref->length() > system_size)
            system_size = opref->length();

    for(vector<quantum_operator>::iterator opref = operators.begin(); opref != operators.end(); opref++)
        opref->resize(system_size);
    //@-<< Post-process to make sure all operators have the same length >>

    return 0;
}
//@+node:gmc.20080824181205.29: ** main_dense_1d
int main_dense_1d(string filename, bool compute_weights_flag, bool yaml_flag) {
    vector<quantum_operator> operators;

    int result;
    if(filename.empty()) {
        cout << "CodeQuest version 1.0.  Run with --help for usage information." << endl;
        cout << "Input quantum operators:  (Ctrl-D when finished)" << endl;
        result = read_in_operators(operators);
    } else {
        ifstream input_file(filename.c_str());
        result = read_in_operators(operators,input_file);
    }
    if(result != 0)
        return result;

    if(yaml_flag) {
        //@+<< YAML output >>
        //@+node:gcross.20101123222425.2068: *3* << YAML output >>
        cout << "---" << endl;

        cout << "Measurement Operators:" << endl;
        for(vector<quantum_operator>::iterator opref = operators.begin(); opref != operators.end(); opref++)
            cout << "    - " << (*opref) << endl;

        if(operators.size() > 0) {
            qec code(operators);
            if(compute_weights_flag) code.optimize_logical_qubits(false);
            code.writeYAML(cout);
        }
        //@-<< YAML output >>
    } else {
        //@+<< Verbose output >>
        //@+node:gcross.20101123222425.2067: *3* << Verbose output >>
        if(operators.size() == 0) {
            cout << "No operators specified." << endl;
            return 0;
        }

        cout << operators.size() << " operators specified:" << endl;

        for(vector<quantum_operator>::iterator opref = operators.begin(); opref != operators.end(); opref++)
            cout << "\t" << (*opref) << endl;

        cout << endl;

        qec code(operators);
        if(compute_weights_flag) code.optimize_logical_qubits();

        cout << code;

        cout << endl;
        //@-<< Verbose output >>
    }

}
//@-others
//@-leo
