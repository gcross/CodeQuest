#include "randomized-tests.hpp"

void testSolverAdaptive() {
    cout << "TESTING ALGORITHM USING ADAPTIVE STRATEGY:" << endl << endl;
    typedef boost::tuple<unsigned int,pair<unsigned int,unsigned int>,pair<unsigned int,unsigned int> > test_parameters;
    const vector<test_parameters> tests = tuple_list_of
        (10000, make_pair(2,5), make_pair(1,3))
        (10000, make_pair(2,8), make_pair(1,8))
        (10000, make_pair(10,20), make_pair(1,40))
        //(200, make_pair(50,100), make_pair(1,200))
        //(10, make_pair(100,200), make_pair(1,400))
        ;
    for(unsigned int i = 0; i < tests.size(); ++i) {
        run_test_batch<dynamic_qec>
            (i+1,tests[i].get<0>(),tests[i].get<1>(),tests[i].get<2>());
    }
}
