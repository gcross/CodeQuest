#include "randomized-tests.hpp"

void testMinimizerDynamic() {
    cout << "TESTING WEIGHT MINIMIZER USING DYNAMIC VECTORS:" << endl << endl;
    typedef boost::tuple<unsigned int,pair<unsigned int,unsigned int>,pair<unsigned int,unsigned int> > test_parameters;
    const vector<test_parameters> tests = tuple_list_of
        (10000, make_pair(1,5), make_pair(1,3))
        (10000, make_pair(1,8), make_pair(1,8))
        ;
    for(unsigned int i = 0; i < tests.size(); ++i) {
        run_weight_minimization_test_batch<dynamic_qec>
            (i+1,tests[i].get<0>(),tests[i].get<1>(),tests[i].get<2>());
    }
}
