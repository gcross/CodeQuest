//@+leo-ver=4-thin
//@+node:gcross.20081119221421.5:@thin codequest.cc
//@@language c

#include <string>
#include <iostream>
#include <vector>

#include <tclap/CmdLine.h>

using namespace std;
using namespace TCLAP;

extern int main_dense_1d(string filename, bool compute_weights_flag);
extern int main_sparse(string filename, bool compute_weights_flag);

int main(int argc, char** argv) {

try {
	CmdLine cmd("codequest description message", ' ', "1.0");

    vector<string> allowed_formats;
    allowed_formats.push_back("dense");
    allowed_formats.push_back("sparse");
    ValuesConstraint<string> allowedVals( allowed_formats );
    ValueArg<string> formatArg("f","format","Input and output format of quantum operators;  if no format is specified, then defaults to dense.",false,"dense",&allowedVals,cmd);

	SwitchArg weightsSwitch("w","weights","Compute the optimal minimum distances for the logical qubits (very expensive!).  Note that this will result in a different (but equivalent) set of logical operators.", false);
    cmd.add(weightsSwitch);

    UnlabeledValueArg<string> filenameArg("filename","Input file with quantum operators;  if no file is specified, then defaults to standard input.",false,"","filename",cmd);

	cmd.parse( argc, argv );

    if(formatArg.getValue().compare("sparse") == 0) {
        return main_sparse(filenameArg.getValue(),weightsSwitch.getValue());
    } else {
        return main_dense_1d(filenameArg.getValue(),weightsSwitch.getValue());
    }
} catch (ArgException &e)  // catch any exceptions
	{ cerr << "error: " << e.error() << " for arg " << e.argId() << endl; }
}
//@nonl
//@-node:gcross.20081119221421.5:@thin codequest.cc
//@-leo
