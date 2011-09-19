//@+leo-ver=5-thin
//@+node:gmc.20080826191619.3: * @thin codequest-dense-1d.cpp
//@@language cplusplus

//@+<< Headers >>
//@+node:gmc.20080826191619.4: ** << Headers >>
#include <boost/algorithm/string/trim.hpp>
#include <boost/range/irange.hpp>
#include <fstream>

#include "codequest.hpp"

using namespace CodeQuest;
using namespace std;
using namespace boost;
using namespace boost::algorithm;
//@-<< Headers >>

#define quantum_operator dynamic_quantum_operator
#define qec qec<dynamic_quantum_operator>

//@+<< I/O >>
//@+node:gmc.20080824181205.25: ** << I/O >>
istream& operator>>(istream& in, quantum_operator& op) {
    string s;
    getline(in,s);
    trim_right(s);
    op.resize(s.length());
    BOOST_FOREACH(const size_t i, irange((size_t)0u, s.length())) {
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
unsigned int read_in_operators(vector<quantum_operator>& operators, istream& in=cin) {

    //@+<< Read in and parse each of the operator strings >>
    //@+node:gmc.20080826191619.2: *4* << Read in and parse each of the operator strings >>
    unsigned int current_line_number = 0;

    try {

        while( in.good() ) {
            current_line_number++;
            {
                char first_column;
                in.get(first_column);
                if(first_column == '#') {
                    string junk;
                    getline(in,junk);
                    continue;
                } else {
                    in.putback(first_column);
                }
            }
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
        BOOST_FOREACH(const size_t j, irange((size_t)0, e.column)) { cerr.put(' '); }
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

    BOOST_FOREACH(const quantum_operator& op, operators) {
        if(op.length() > system_size)
            system_size = op.length();
    }

    BOOST_FOREACH(quantum_operator& op, operators) {
        op.resize(system_size);
    }
    //@-<< Post-process to make sure all operators have the same length >>

    return 0;
}
//@+node:gmc.20080824181205.29: ** main_dense_1d
unsigned int main_dense_1d(string filename, bool compute_weights_flag) {
    vector<quantum_operator> operators;

    unsigned int result;
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

    cout << "---" << endl;

    cout << "Measurement Operators:" << endl;
    BOOST_FOREACH(const quantum_operator& op, operators) {
        cout << "    - " << op << endl;
    }
    cout << endl;

    if(operators.size() > 0) {
        cout << solve(operators,compute_weights_flag) << endl;
    }

}
//@-others
//@-leo
