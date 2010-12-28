//@+leo-ver=5-thin
//@+node:gcross.20101228150742.1584: * @thin testlib.cpp
//@@language cplusplus

#include "randomized-tests.hpp"

mt19937 rng;
uniform_smallint<> one_through_three(1,3);
variate_generator<mt19937&, uniform_smallint<> > random_pauli(rng, one_through_three);
uniform_01<> u01;
variate_generator<mt19937&, uniform_01<> > random_real(rng, u01);
//@-leo
