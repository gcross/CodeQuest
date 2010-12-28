//@+leo-ver=5-thin
//@+node:gcross.20101228150742.1436: * @thin testlib-minimizer-static-4.cpp
//@@language cplusplus

#include "randomized-tests.hpp"

void testMinimizerStatic4() {
    cout << "TESTING WEIGHT MINIMIZER USING STATIC VECTORS FOR 4 QUBITS:" << endl << endl;
    typedef boost::tuple<unsigned int,pair<unsigned int,unsigned int> > test_parameters;
    const vector<test_parameters> tests = tuple_list_of
        (10000, make_pair(1,3))
        (10000, make_pair(1,8))
        ;
    for(unsigned int i = 0; i < tests.size(); ++i) {
        run_weight_minimization_test_batch_with_fixed_number_of_qubits<static_qec<4>::type>
            (i+1,tests[i].get<0>(),4,tests[i].get<1>());
    }
}
//@-leo
