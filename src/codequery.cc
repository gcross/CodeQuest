//@+leo-ver=4-thin
//@+node:gcross.20081119221421.5:@thin codequery.cc
//@@language cplusplus

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
	CmdLine cmd("codequery description message", ' ', "1.0 beta");

    vector<string> allowed_formats;
    allowed_formats.push_back("dense");
    allowed_formats.push_back("sparse");
    ValuesConstraint<string> allowedVals( allowed_formats );
    ValueArg<string> formatArg("f","format","Input/Output format of quantum operators",false,"sparse",&allowedVals,cmd);

	SwitchArg weightsSwitch("w","weights","Compute the minimum weights (very expensive!)", false);
    cmd.add(weightsSwitch);

    UnlabeledValueArg<string> filenameArg("filename","Input file with quantum operators",false,"","filename",cmd);

	cmd.parse( argc, argv );

    if(formatArg.getValue().compare("sparse") == 0) {
        return main_sparse(filenameArg.getValue(),weightsSwitch.getValue());
    } else {
        return main_dense_1d(filenameArg.getValue(),weightsSwitch.getValue());
    }
} catch (ArgException &e)  // catch any exceptions
	{ cerr << "error: " << e.error() << " for arg " << e.argId() << endl; }
}
//@-node:gcross.20081119221421.5:@thin codequery.cc
//@-leo
