#include "randomized-tests.hpp"

void testSolverStatic64() {
    cout << "TESTING ALGORITHM USING STATIC VECTORS FOR 64 QUBITS:" << endl << endl;
    typedef boost::tuple<unsigned int,pair<unsigned int,unsigned int> > test_parameters;
    const vector<test_parameters> tests = tuple_list_of
        (10000, make_pair(1,256))
        ;
    for(unsigned int i = 0; i < tests.size(); ++i) {
        run_test_batch_with_fixed_number_of_qubits<static_qec<64,256> >
            (i+1,tests[i].get<0>(),64,tests[i].get<1>());
    }
}
