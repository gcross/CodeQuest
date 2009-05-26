//@+leo-ver=4-thin
//@+node:gmc.20080824181205.21:@thin codelib.cc
//@@language c++

//@<< Headers >>
//@+node:gmc.20080824181205.22:<< Headers >>
#include "codelib.hpp"

using namespace std;
using namespace boost;
//@-node:gmc.20080824181205.22:<< Headers >>
//@nl

//@+others
//@+node:gmc.20080824181205.24:Class Implementations
//@+node:gmc.20080826115908.3:invalid_pauli
invalid_pauli::invalid_pauli(std::string &operator_string_, size_t column_) throw()
    : operator_string(operator_string_), column(column_) { }

invalid_pauli::~invalid_pauli() throw() {}

const char* invalid_pauli::what() throw() { return "invalid pauli operator specificed"; }
//@nonl
//@-node:gmc.20080826115908.3:invalid_pauli
//@-node:gmc.20080824181205.24:Class Implementations
//@+node:gcross.20081222162732.3:I/O
std::ostream& operator<<(std::ostream& out, const dynamic_quantum_operator& op) {
    for(int i = 0; i < op.length(); i++)
        out.put(pauli_char_from_op(op,i));
	return out;
}
//@-node:gcross.20081222162732.3:I/O
//@-others
//@-node:gmc.20080824181205.21:@thin codelib.cc
//@-leo
