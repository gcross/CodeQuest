//@+leo-ver=5-thin
//@+node:gcross.20081119221421.5: * @thin codequest.cc
//@@language cplusplus

#include <boost/function.hpp>
#include <string>
#include <iostream>
#include <vector>

#include <tclap/CmdLine.h>

using namespace boost;
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

    SwitchArg weightsSwitch("o","optimize","Optimize the subsytem code (very expensive!) and output the optimal logical qubit distances.  Note that this will result in a different set of logical operators compared to running codequest without this option.", false);
    cmd.add(weightsSwitch);

    UnlabeledValueArg<string> filenameArg("filename","Input file with quantum operators;  if no file is specified, then defaults to standard input.",false,"","filename",cmd);

    cmd.parse( argc, argv );

    function<int (string filename, bool compute_weights_flag)> nextMain =
        formatArg.getValue().compare("sparse") == 0
            ? main_sparse
            : main_dense_1d
    ;

    return nextMain(filenameArg.getValue(),weightsSwitch.getValue());
} catch (ArgException &e)  // catch any exceptions
	{ cerr << "error: " << e.error() << " for arg " << e.argId() << endl; }
}
//@-leo
