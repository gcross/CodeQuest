//@+leo-ver=4-thin
//@+node:gmc.20080826191619.10:@thin codequery-sparse.cc
//@@language cplusplus

//@<< Headers >>
//@+node:gmc.20080826191619.11:<< Headers >>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <fstream>
#include <blitz/array.h>
#include <map>
#include <iomanip>
#include <string>
#include <cctype>
#include <sys/times.h>

#include "codelib.hpp"

using namespace std;
using namespace boost;
using namespace boost::algorithm;
using namespace blitz;
using namespace __gnu_cxx;
//@-node:gmc.20080826191619.11:<< Headers >>
//@nl

#define quantum_operator dynamic_quantum_operator
#define qec qec<dynamic_quantum_operator>

//@<< Declarations >>
//@+node:gmc.20080826191619.12:<< Declarations >>
ostream& operator<<(ostream& out, const quantum_operator& op);
istream& operator>>(istream& in, quantum_operator& op);

ostream& operator<<(ostream& out, qec& code);

typedef TinyVector<int,2> CoordinateVector;
//@-node:gmc.20080826191619.12:<< Declarations >>
//@nl

//@+others
//@+node:gmc.20080826191619.14:Functions
//@+node:gmc.20080826191619.15:read_in_operators
typedef pair<bool,bool> pauli_pair;

int read_in_operators(vector<dynamic_quantum_operator>& operators, vector<string>& qubit_labels, istream& in=cin) {

    string s;
    size_t number_of_qubits = 0;
    map<string,size_t> label_to_qubit_map;
    int current_line_number = 0;

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
                        message += "' is not a valid Pauli operator.";
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
            for(map<size_t,pauli_pair>::iterator i = opmap.begin(); i != opmap.end(); i++) {
                op.X[i->first] = i->second.first;
                op.Z[i->first] = i->second.second;
            }
            operators.push_back(op);
        } catch(string& message) {
            size_t current_column_number = (charref-s.begin())+1;
            cerr
                << "Error in column " << current_column_number << " of line " << current_line_number << endl
                << endl
                << "\t" << s << endl
                << "\t";
            for(int j =  1; j < current_column_number; j++)
                cerr.put(' ');
            cerr
                << "^" << endl
                << endl
                << message << endl;
            return -1;
        }
    }

    for(vector<quantum_operator>::iterator opref = operators.begin(); opref != operators.end(); opref++)
        opref->resize(number_of_qubits);

    return 0;
}
//@-node:gmc.20080826191619.15:read_in_operators
//@+node:gmc.20080826191619.24:isntnumeric
bool isntdigit(const char c) {
    return not (isdigit(c) or (c == '-') or (c == '+'));
}

bool isntnumeric(const string& s) {
    return find_if(s.begin(),s.end(),isntdigit) != s.end();
}
//@-node:gmc.20080826191619.24:isntnumeric
//@+node:gmc.20080826191619.29:tonumeric
int tonumeric(const string &s) {
    istringstream ss(s);
    int i;
    ss >> i;
    return i;
}
//@nonl
//@-node:gmc.20080826191619.29:tonumeric
//@+node:gmc.20080826191619.25:count_coordinates_in
int count_coordinates_in(const string &s) {
    vector<string> coordinates;
    split(coordinates,s,is_any_of(","));
    for(vector<string>::iterator coordref = coordinates.begin(); coordref != coordinates.end(); coordref++) {
        trim(*coordref);
        if(isntnumeric(*coordref))
            return 0;
    }
    return coordinates.size();
}
//@-node:gmc.20080826191619.25:count_coordinates_in
//@+node:gmc.20080826191619.32:print_op
void print_op(const char* prefix, const int width, const int height, const vector<CoordinateVector>& qubit_coordinates, const quantum_operator& op, const bool skip_first_prefix=false) {
    Array<char,2> grid(width,height);
    grid = ' ';
    int i = 0;
    for(vector<CoordinateVector>::const_iterator coordref = qubit_coordinates.begin();
        coordref != qubit_coordinates.end();
        i++, coordref++
    ) {
        assert((*coordref)[0]>=0);
        assert((*coordref)[1]>=0);
        grid(*coordref) = pauli_char_from_op(op,i);
    }
    for(int y = height-1; y >= 0; y--) {
        if((not skip_first_prefix) or (y < (height-1)))
            cout << prefix;
        for(int x = 0; x < width; x++) {
            cout << grid(x,y);
        }
        cout << endl;
    }
    if(height > 1)
        cout << endl;
}
//@-node:gmc.20080826191619.32:print_op
//@+node:gmc.20080915140059.7:print_op_sparse
void print_op_sparse(const vector<string>& qubit_labels, const quantum_operator& op) {

    for(int i = 0; i < op.length(); i++) {
        char c = pauli_char_from_op(op,i);
        if(c != '.')
            cout << "(" << qubit_labels[i] << ") " << c << " ";       
    }
    cout << endl;

}
//@-node:gmc.20080915140059.7:print_op_sparse
//@-node:gmc.20080826191619.14:Functions
//@+node:gmc.20080826191619.13:main_sparse
int main_sparse(string filename, bool compute_weights_flag) {

    //@    << Read in the operators >>
    //@+node:gmc.20080826191619.21:<< Read in the operators >>
    vector<quantum_operator> operators;
    vector<string> qubit_labels;

    int result;
    if(filename.empty()) {
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

    int number_of_qubits = qubit_labels.size();
    int number_of_operators = operators.size();
    //@-node:gmc.20080826191619.21:<< Read in the operators >>
    //@nl

    //@    << Process labels into coordinates >>
    //@+node:gmc.20080826191619.22:<< Process labels into coordinates >>
    int number_of_coordinates;

    //@<< Count number of dimensions in coordinate (0 if labels aren't coordinates) >>
    //@+node:gmc.20080826191619.26:<< Count number of dimensions in coordinate (0 if labels aren't coordinates) >>
    vector<string>::const_iterator labelref = qubit_labels.begin();

    number_of_coordinates = count_coordinates_in(*labelref);

    if(number_of_coordinates != 0)
        for(labelref++; labelref != qubit_labels.end(); labelref++)
            if(count_coordinates_in(*labelref) != number_of_coordinates) {
                number_of_coordinates = 0;
                break;
            }
    //@-node:gmc.20080826191619.26:<< Count number of dimensions in coordinate (0 if labels aren't coordinates) >>
    //@nl

    vector<CoordinateVector> qubit_coordinates;

    TinyVector<int,2> mins(0,0);
    TinyVector<int,2> maxes(0,0);

    CoordinateVector coordinate(0,0);

    switch(number_of_coordinates) {
        case 1:
            //@        << Process 1D coordinates >>
            //@+node:gmc.20080826191619.28:<< Process 1D coordinates >>
            for(labelref = qubit_labels.begin(); labelref != qubit_labels.end(); labelref++) {
                int x = tonumeric(*labelref);
                if(x<mins[0])
                    mins[0] = x;
                if(x>maxes[0])
                    maxes[0] = x;
                coordinate[0] = x;
                qubit_coordinates.push_back(coordinate);
            }

            //@-node:gmc.20080826191619.28:<< Process 1D coordinates >>
            //@nl
            break;
        case 2:
            //@        << Process 2D coordinates >>
            //@+node:gmc.20080826191619.30:<< Process 2D coordinates >>
            for(labelref = qubit_labels.begin(); labelref != qubit_labels.end(); labelref++) {
                vector<string> coordinates;
                split(coordinates,*labelref,is_any_of(","));

                for(int i = 0; i < 2; i++) {
                    string& s = coordinates[i];
                    trim(s);
                    int c = tonumeric(s);
                    if(c<mins[i])
                        mins[i] = c;
                    if(c>maxes[i])
                        maxes[i] = c;
                    coordinate[i] = c;
                }
                qubit_coordinates.push_back(coordinate);
            }
            //@-node:gmc.20080826191619.30:<< Process 2D coordinates >>
            //@nl
            break;
        default:
            number_of_coordinates = 0;
            //@        << Use existing ordering of qubits as the x coordinate >>
            //@+node:gmc.20080826191619.27:<< Use existing ordering of qubits as the x coordinate >>
            for(int i = 0; i < number_of_qubits; i++) {
                coordinate[0] = i;
                qubit_coordinates.push_back(coordinate);
            }

            maxes[0] = number_of_qubits-1;
            //@-node:gmc.20080826191619.27:<< Use existing ordering of qubits as the x coordinate >>
            //@nl
            break;
    }

    int width = maxes[0]-mins[0]+1,
       height = maxes[1]-mins[1]+1;

    if(number_of_coordinates != 0) {
        //@    << Post-process coordinates >>
        //@+node:gmc.20080826191619.33:<< Post-process coordinates >>
        //@+at
        // We need to shift around the coordinates so that the min is zero.
        //@-at
        //@@c

        for(vector<CoordinateVector>::iterator coordref = qubit_coordinates.begin(); coordref != qubit_coordinates.end();  coordref++)
            for(int i = 0; i < 2; i++)
                (*coordref)[i] -= mins[i];
        //@-node:gmc.20080826191619.33:<< Post-process coordinates >>
        //@nl
    }
    //@nonl
    //@-node:gmc.20080826191619.22:<< Process labels into coordinates >>
    //@nl

    //@    << Compute code >>
    //@+node:gmc.20080916172520.8:<< Compute code >>
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
            labelref = qubit_labels.begin();
            for(int i = 1; labelref != qubit_labels.end(); i++, labelref++)
                cout << setw(8) << i << ":" << *labelref << endl;
            break;
    }

    cout << endl;

    cout << operators.size() << " operators specified:" << endl;

    for(vector<quantum_operator>::iterator opref = operators.begin(); opref != operators.end(); opref++) {
        print_op("\t",width,height,qubit_coordinates,*opref);
        if(number_of_coordinates>1)
            cout << endl;
    }

    cout << endl;

    //tms before, after;
    cout << "Computing code...";
    //times(&before);
    qec code(operators);
    //times(&after);
    cout << "done!" << endl; //  Took " << (((after.tms_utime+after.tms_stime)-(before.tms_utime+before.tms_stime)) * CLK_TCK) << " seconds." << endl << endl;

    if(compute_weights_flag)
        code.optimize_logical_qubits();
    //@-node:gmc.20080916172520.8:<< Compute code >>
    //@nl

    //@    << Print out results >>
    //@+node:gmc.20080826191619.31:<< Print out results >>


    cout << "Stabilizers:" << endl;

    for(vector<quantum_operator>::iterator opref = code.stabilizers.begin(); opref != code.stabilizers.end(); opref++) {
        print_op("\t",width,height,qubit_coordinates,*opref);
    }

    cout << endl;


    for(int i = 0; i < code.gauge_qubits.size(); i++) {
        cout << "----- GAUGE QUBIT " << (i+1) << "-----" << endl;
        cout <<  "\t| Logical X |    ";
        print_op("\t|||||||||||||    ",width,height,qubit_coordinates,code.gauge_qubits[i].X,true);
        cout <<  "\t| Logical Y |    ";
        print_op("\t|||||||||||||    ",width,height,qubit_coordinates,code.gauge_qubits[i].Y,true);
        cout <<  "\t| Logical Z |    ";
        print_op("\t|||||||||||||    ",width,height,qubit_coordinates,code.gauge_qubits[i].Z,true);
    }

    cout << endl;


    for(int i = 0; i < code.logical_qubits.size(); i++) {
        cout << "----- LOGICAL QUBIT " << (i+1) << "-----" << endl;
        cout <<  "\t| Logical X |    ";
        print_op("\t|||||||||||||    ",width,height,qubit_coordinates,code.logical_qubits[i].X,true);
        cout <<  "\t| Logical Y |    ";
        print_op("\t|||||||||||||    ",width,height,qubit_coordinates,code.logical_qubits[i].Y,true);
        cout <<  "\t| Logical Z |    ";
        print_op("\t|||||||||||||    ",width,height,qubit_coordinates,code.logical_qubits[i].Z,true);
        if(i < code.number_of_optimized_logical_qubits) {
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
        for(int i = 0; i < code.logical_qubits.size()-1; i++) cout << code.logical_qubit_error_distances[i] << ", ";
        cout << code.logical_qubit_error_distances[code.logical_qubits.size()-1] << endl;
    } else cout << endl;

    //@-node:gmc.20080826191619.31:<< Print out results >>
    //@nl

}
//@-node:gmc.20080826191619.13:main_sparse
//@-others

//@-node:gmc.20080826191619.10:@thin codequery-sparse.cc
//@-leo
