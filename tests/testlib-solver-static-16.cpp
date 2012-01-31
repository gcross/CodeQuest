#include "randomized-tests.hpp"

void testSolverStatic16() {
    cout << "TESTING ALGORITHM USING STATIC VECTORS FOR 16 QUBITS:" << endl << endl;
    typedef boost::tuple<unsigned int,pair<unsigned int,unsigned int> > test_parameters;
    const vector<test_parameters> tests = tuple_list_of
        (10000, make_pair(1,16))
        (10000, make_pair(16,64))
        ;
    for(unsigned int i = 0; i < tests.size(); ++i) {
        run_test_batch_with_fixed_number_of_qubits<static_qec<16,64> >
            (i+1,tests[i].get<0>(),16,tests[i].get<1>());
    }
}
