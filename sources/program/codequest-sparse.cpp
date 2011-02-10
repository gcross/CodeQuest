//@+leo-ver=5-thin
//@+node:gmc.20080826191619.10: * @thin codequest-sparse.cpp
//@@language cplusplus

//@+<< Headers >>
//@+node:gmc.20080826191619.11: ** << Headers >>
#include <boost/algorithm/string.hpp>
#include <boost/array.hpp>
#include <boost/range/irange.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <iomanip>
#include <string>
#include <cctype>
#include <sys/times.h>

#include "codequest.hpp"

using namespace CodeQuest;
using namespace std;
using namespace boost;
using namespace boost::adaptors;
using namespace boost::algorithm;
using namespace __gnu_cxx;
//@-<< Headers >>

#define quantum_operator dynamic_quantum_operator
#define qec qec<dynamic_quantum_operator>

//@+<< Declarations >>
//@+node:gmc.20080826191619.12: ** << Declarations >>
istream& operator>>(istream& in, quantum_operator& op);
typedef array<int,2> CoordinateVector;
//@-<< Declarations >>

//@+others
//@+node:gmc.20080826191619.14: ** Functions
//@+node:gmc.20080826191619.15: *3* read_in_operators
typedef pair<bool,bool> pauli_pair;

unsigned int read_in_operators(vector<dynamic_quantum_operator>& operators, vector<string>& qubit_labels, istream& in=cin) {

    string s;
    size_t number_of_qubits = 0;
    map<string,size_t> label_to_qubit_map;
    unsigned int current_line_number = 0;

    while(getline(in,s)) {
        current_line_number++;
        string::const_iterator charref = s.begin();
        while((*charref == ' ') and (charref != s.end())) charref++;
        try {
            map<size_t,pauli_pair> opmap;
            while(charref != s.end()) {
                if(*charref != '(') throw string("Expected parenthesis here.") ; // )
                charref++;
                string::const_iterator begin_label_ref = charref;
                while((*charref != ')') and (charref != s.end())) charref++;
                if(charref == s.end()) throw string("No closing parenthesis found.");
                string label(begin_label_ref,charref);
                trim(label);
                charref++;
                while((*charref == ' ') and (charref != s.end())) charref++;
                if(charref == s.end()) throw string("No Pauli operator found.");
                pauli_pair pauli(false,false);
                switch(*charref){
                    case 'x':
                    case 'X':
                        pauli.first = true;
                        break;
                    case 'y':
                    case 'Y':
                        pauli.first = pauli.second = true;
                        break;
                    case 'z':
                    case 'Z':
                        pauli.second = true;
                        break;
                    default:
                        string message("'");
                        message.push_back(*charref);
                        message += "' is not a valid Pauli operator.  Valid Pauli operators are X, Y, and Z, case insensitive.";
                        throw message;
                }
                charref++;
                map<string,size_t>::iterator i = label_to_qubit_map.find(label);
                size_t qubit_index;
                if(i == label_to_qubit_map.end()) {
                    label_to_qubit_map[label] = number_of_qubits;
                    qubit_labels.push_back(label);
                    qubit_index = number_of_qubits;
                    number_of_qubits++;
                } else {
                    qubit_index = i->second;
                }
                if(opmap.count(qubit_index)>0) {
                    string message("The label '");
                    message += label;
                    message += "' appears more than once in this operator";
                    throw message;
                }
                opmap[qubit_index] = pauli;
                while((*charref == ' ') and (charref != s.end())) charref++;
            }
            quantum_operator op(number_of_qubits);
            typedef pair<size_t,pauli_pair> entry_t;
            BOOST_FOREACH(const entry_t& entry, opmap) {
                op.X[entry.first] = entry.second.first;
                op.Z[entry.first] = entry.second.second;
            }
            operators.push_back(op);
        } catch(string& message) {
            unsigned int current_column_number = (charref-s.begin())+1;
            cerr
                << "Error in column " << current_column_number << " of line " << current_line_number << endl
                << endl
                << "\t" << s << endl
                << "\t";
            BOOST_FOREACH(const size_t j, irange(1u,current_column_number)) { cerr.put(' '); }
            cerr
                << "^" << endl
                << endl
                << message << endl;
            return -1;
        }
    }

    BOOST_FOREACH(quantum_operator& op, operators) {
        op.resize(number_of_qubits);
    }

    return 0;
}
//@+node:gmc.20080826191619.24: *3* isntnumeric
bool isntdigit(const char c) {
    return not (isdigit(c) or (c == '-') or (c == '+'));
}

bool isntnumeric(const string& s) {
    return find_if(s.begin(),s.end(),isntdigit) != s.end();
}
//@+node:gmc.20080826191619.29: *3* tonumeric
int tonumeric(const string &s) {
    istringstream ss(s);
    int i;
    ss >> i;
    return i;
}
//@+node:gmc.20080826191619.25: *3* count_coordinates_in
unsigned int count_coordinates_in(const string &s) {
    vector<string> coordinates;
    split(coordinates,s,is_any_of(","));
    BOOST_FOREACH(string& coordinate, coordinates) {
        trim(coordinate);
        if(isntnumeric(coordinate))
            return 0;
    }
    return coordinates.size();
}
//@+node:gmc.20080826191619.32: *3* print_op
void print_op(const char* prefix, const unsigned int width, const unsigned int height, const vector<CoordinateVector>& qubit_coordinates, const quantum_operator& op, const bool skip_first_prefix=false) {
    char grid[width][height];
    for (unsigned int x = 0; x < width; ++x)
        for (unsigned int y = 0; y < height; ++y)
            grid[x][y] = ' ';
    unsigned int i = 0;
    BOOST_FOREACH(const CoordinateVector& coordinates, qubit_coordinates) {
        const unsigned int x = coordinates[0];
        const unsigned int y = coordinates[1];
        assert(x>=0); assert(x<width);
        assert(y>=0); assert(y<height);
        grid[x][y] = op.pauli_char_at(i);
        ++i;
    }
    BOOST_FOREACH(const unsigned int y, irange(0u,height) | reversed) {
        if((not skip_first_prefix) or (y < (height-1)))
            cout << prefix;
        BOOST_FOREACH(const unsigned int x, irange(0u,width)) {
            cout << grid[x][y];
        }
        cout << endl;
    }
    if(height > 1)
        cout << endl;
}
//@+node:gmc.20080915140059.7: *3* print_op_sparse
void print_op_sparse(const vector<string>& qubit_labels, const quantum_operator& op) {

    BOOST_FOREACH(const size_t i, irange((size_t)0,op.length())) {
        const char c = op.pauli_char_at(i);
        if(c != '.')
            cout << "(" << qubit_labels[i] << ") " << c << " ";       
    }
    cout << endl;

}
//@+node:gmc.20080826191619.13: ** main_sparse
int main_sparse(string filename, bool compute_weights_flag) {

    //@+<< Read in the operators >>
    //@+node:gmc.20080826191619.21: *3* << Read in the operators >>
    vector<quantum_operator> operators;
    vector<string> qubit_labels;

    unsigned int result;
    if(filename.empty()) {
        cout << "CodeQuest version 1.0.  Run with --help for usage information." << endl;
        cout << "Input quantum operators:  (Ctrl-D when finished)" << endl;
        result = read_in_operators(operators,qubit_labels);
    } else {
        ifstream input_file(filename.c_str());
        result = read_in_operators(operators,qubit_labels,input_file);
    }

    if(result != 0)
        return result;

    if(operators.size() == 0) {
        cout << "No operators specified." << endl;
        return 0;
    }

    const unsigned int number_of_qubits = qubit_labels.size();
    const unsigned int number_of_operators = operators.size();
    //@-<< Read in the operators >>

    //@+<< Process labels into coordinates >>
    //@+node:gmc.20080826191619.22: *3* << Process labels into coordinates >>
    unsigned int number_of_coordinates;

    //@+<< Count number of dimensions in coordinate (0 if labels aren't coordinates) >>
    //@+node:gmc.20080826191619.26: *4* << Count number of dimensions in coordinate (0 if labels aren't coordinates) >>
    {
        vector<string>::const_iterator labelref = qubit_labels.begin();

        number_of_coordinates = count_coordinates_in(*labelref);

        if(number_of_coordinates != 0)
            for(labelref++; labelref != qubit_labels.end(); labelref++)
                if(count_coordinates_in(*labelref) != number_of_coordinates) {
                    number_of_coordinates = 0;
                    break;
                }
    }
    //@-<< Count number of dimensions in coordinate (0 if labels aren't coordinates) >>

    vector<CoordinateVector> qubit_coordinates;

    array<int,2> mins;  mins.assign(0);
    array<int,2> maxes;  maxes.assign(0);

    CoordinateVector coordinate;  coordinate.assign(0);

    switch(number_of_coordinates) {
        case 1:
            //@+<< Process 1D coordinates >>
            //@+node:gmc.20080826191619.28: *4* << Process 1D coordinates >>
            BOOST_FOREACH(const string& label, qubit_labels) {
                const int x = tonumeric(label);
                if(x<mins[0])
                    mins[0] = x;
                if(x>maxes[0])
                    maxes[0] = x;
                coordinate[0] = x;
                qubit_coordinates.push_back(coordinate);
            }

            //@-<< Process 1D coordinates >>
            break;
        case 2:
            //@+<< Process 2D coordinates >>
            //@+node:gmc.20080826191619.30: *4* << Process 2D coordinates >>
            BOOST_FOREACH(const string& label, qubit_labels) {
                vector<string> coordinates;
                split(coordinates,label,is_any_of(","));

                BOOST_FOREACH(const unsigned int i, irange(0u,2u)) {
                    string& s = coordinates[i];
                    trim(s);
                    const int c = tonumeric(s);
                    if(c<mins[i])
                        mins[i] = c;
                    if(c>maxes[i])
                        maxes[i] = c;
                    coordinate[i] = c;
                }
                qubit_coordinates.push_back(coordinate);
            }
            //@-<< Process 2D coordinates >>
            break;
        default:
            number_of_coordinates = 0;
            //@+<< Use existing ordering of qubits as the x coordinate >>
            //@+node:gmc.20080826191619.27: *4* << Use existing ordering of qubits as the x coordinate >>
            BOOST_FOREACH(const unsigned int i, irange(0u,number_of_qubits)) {
                coordinate[0] = i;
                qubit_coordinates.push_back(coordinate);
            }

            maxes[0] = number_of_qubits-1;
            //@-<< Use existing ordering of qubits as the x coordinate >>
            break;
    }

    const unsigned int  width = maxes[0]-mins[0]+1,
                       height = maxes[1]-mins[1]+1;

    if(number_of_coordinates != 0) {
        //@+<< Post-process coordinates >>
        //@+node:gmc.20080826191619.33: *4* << Post-process coordinates >>
        //@+at
        // We need to shift around the coordinates so that the min is zero.
        //@@c

        BOOST_FOREACH(CoordinateVector& coordinates, qubit_coordinates) {
            BOOST_FOREACH(unsigned int i, irange(0u,2u)) {
                coordinates[i] -= mins[i];
            }
        }
        //@-<< Post-process coordinates >>
    }
    //@-<< Process labels into coordinates >>

    //@+<< Compute code >>
    //@+node:gmc.20080916172520.8: *3* << Compute code >>
    switch(number_of_coordinates) {
        case 1:
            cout << "Labels imply 1D coordinate system of size " << width
                 << ", range [" << mins[0] << "," << maxes[0] << "]" << endl;
            break;
        case 2:
            cout << "Labels imply 2D coordinate system of size " << width << "x" << height
                 << ", range [" << mins[0] << "," << maxes[0] << "]x[" << mins[1] << "," << maxes[1] << "]" << endl;
            break;
        default:
            cout << "Label cannot be interpreted as coordinates.  Numbering of labels is as follows:" << endl;
            {
                unsigned int i = 1u;
                BOOST_FOREACH(const string& label, qubit_labels) {
                    cout << setw(8) << i++ << ":" << label << endl;
                }
            }
            break;
    }

    cout << endl;

    cout << operators.size() << " operators specified:" << endl;

    BOOST_FOREACH(const quantum_operator& op, operators) {
        print_op("\t",width,height,qubit_coordinates,op);
        if(number_of_coordinates>1)
            cout << endl;
    }

    cout << endl;

    //tms before, after;
    cout << "Computing code...";
    //times(&before);
    code code = solve(operators,compute_weights_flag);
    //times(&after);
    cout << "done!" << endl; //  Took " << (((after.tms_utime+after.tms_stime)-(before.tms_utime+before.tms_stime)) * CLK_TCK) << " seconds." << endl << endl;
    //@-<< Compute code >>

    //@+<< Print out results >>
    //@+node:gmc.20080826191619.31: *3* << Print out results >>
    cout << "Stabilizers:" << endl;

    BOOST_FOREACH(const quantum_operator& op, operators) {
        print_op("\t",width,height,qubit_coordinates,op);
    }

    cout << endl;

    BOOST_FOREACH(const size_t i, irange((size_t)0,code.gauge_qubits.size())) {
        cout << "----- GAUGE QUBIT " << (i+1) << "-----" << endl;
        cout <<  "\t| Logical X |    ";
        print_op("\t|||||||||||||    ",width,height,qubit_coordinates,code.gauge_qubits[i].X,true);
        cout <<  "\t| Logical Y |    ";
        print_op("\t|||||||||||||    ",width,height,qubit_coordinates,code.gauge_qubits[i].Y,true);
        cout <<  "\t| Logical Z |    ";
        print_op("\t|||||||||||||    ",width,height,qubit_coordinates,code.gauge_qubits[i].Z,true);
    }

    cout << endl;

    BOOST_FOREACH(const size_t i, irange((size_t)0,code.logical_qubits.size())) {
        cout << "----- LOGICAL QUBIT " << (i+1) << "-----" << endl;
        cout <<  "\t| Logical X |    ";
        print_op("\t|||||||||||||    ",width,height,qubit_coordinates,code.logical_qubits[i].X,true);
        cout <<  "\t| Logical Y |    ";
        print_op("\t|||||||||||||    ",width,height,qubit_coordinates,code.logical_qubits[i].Y,true);
        cout <<  "\t| Logical Z |    ";
        print_op("\t|||||||||||||    ",width,height,qubit_coordinates,code.logical_qubits[i].Z,true);
        if(code.optimized) {
            cout << "\t Distance: " << code.logical_qubit_error_distances[i] << endl;
            cout << "\t Minimum weight error:" << endl;
            print_op("\t       ",width,height,qubit_coordinates,code.logical_qubit_errors[i],false);
        }
    }

    cout << endl << endl;


    cout << number_of_operators << " operator";
    if(number_of_operators!=1) cout << "s";
    cout << endl;

    cout << number_of_qubits << " physical qubit";
    if(number_of_qubits!=1) cout << "s";
    cout << endl;

    cout << code.stabilizers.size() << " stabilizer";
    if(code.stabilizers.size()!=1) cout << "s";
    cout << endl;

    cout << code.gauge_qubits.size() << " gauge qubit";
    if(code.gauge_qubits.size()!=1) cout << "s";
    cout << endl;

    cout << code.logical_qubits.size() << " logical qubit";
    if(code.logical_qubits.size()!=1) cout << "s";

    if(compute_weights_flag and code.logical_qubits.size()>0) {
        cout << " with weights: ";
        BOOST_FOREACH(const size_t i, irange((size_t)0,code.logical_qubits.size()-1)) {
            cout << code.logical_qubit_error_distances[i] << ", ";
        }
        cout << code.logical_qubit_error_distances[code.logical_qubits.size()-1] << endl;
    } else cout << endl;
    //@-<< Print out results >>

}
//@-others

//@-leo
