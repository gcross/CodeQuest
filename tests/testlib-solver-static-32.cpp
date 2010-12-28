//@+leo-ver=5-thin
//@+node:gcross.20101228150742.1451: * @thin testlib-solver-static-32.cpp
//@@language cplusplus

#include "randomized-tests.hpp"

void testSolverStatic32() {
    cout << "TESTING ALGORITHM USING STATIC VECTORS FOR 32 QUBITS:" << endl << endl;
    typedef boost::tuple<unsigned int,pair<unsigned int,unsigned int> > test_parameters;
    const vector<test_parameters> tests = tuple_list_of
        (10000, make_pair(1,64))
        (10000, make_pair(64,256))
        ;
    for(unsigned int i = 0; i < tests.size(); ++i) {
        run_test_batch_with_fixed_number_of_qubits<static_qec<32>::type>
            (i+1,tests[i].get<0>(),32,tests[i].get<1>());
    }
}
//@-leo
