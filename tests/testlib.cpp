//@+leo-ver=5-thin
//@+node:gcross.20101228150742.1584: * @thin testlib.cpp
//@@language cplusplus

//@+<< Includes >>
//@+node:gcross.20110210011631.1497: ** << Includes >>
#include "randomized-tests.hpp"
//@-<< Includes >>

//@+others
//@+node:gcross.20110210011631.1495: ** Generators
mt19937 rng;
uniform_smallint<> one_through_three(1,3);
variate_generator<mt19937&, uniform_smallint<> > random_pauli(rng, one_through_three);
uniform_01<> u01;
variate_generator<mt19937&, uniform_01<> > random_real(rng, u01);
//@+node:gcross.20110210011631.1496: ** Functions
//@+node:gcross.20110210011631.1494: *3* generate_and_test_solver
double generate_and_test_solver(
        unsigned int number_of_physical_qubits,
        unsigned int number_of_operators,
        float bernoulli_trial_probability
) {
    dynamic_operator_vector operators =
        generate_random_operators<dynamic_qec>(
            number_of_physical_qubits,
            number_of_operators,
            bernoulli_trial_probability
        );

    timer T;
    code code = solve(operators,true);
    double elapsed_time = T.elapsed();

    vector<string> errors = check_for_problems_in_code(operators,code);

    if(errors.size() > 0) {

        cout << "Operators are:" << endl;

        BOOST_FOREACH(dynamic_quantum_operator& op, operators) {
            cout << "\t" << op << endl;
        }

        cout << endl;


        cout << "Code is" << endl << code;


        cout << "Errors are:" << endl;

        BOOST_FOREACH(string& error, errors) { cout << error << endl; }

        throw exception();
    } else return elapsed_time;
}
//@+node:gcross.20110210011631.1501: *3* run_test_solver_batch
bool run_test_solver_batch(
    unsigned int batch_number,
    unsigned int number_of_cases,
    pair<unsigned int,unsigned int> qubit_range,
    pair<unsigned int,unsigned int> operator_range
) {
    cout << "Test batch " << batch_number << ":" << endl;
    cout << "\tNumber of qubits: " << qubit_range.first << "-" << qubit_range.second << endl;
    cout << "\tNumber of operators: " << operator_range.first << "-" << operator_range.second << endl;
    cout << "Generating " << number_of_cases << " random test cases:";
    uniform_smallint<> small_qubit_range(qubit_range.first,qubit_range.second);
    variate_generator<mt19937&, uniform_smallint<> > random_number_of_physical_qubits(rng, small_qubit_range);
    uniform_smallint<> small_operator_range(operator_range.first,operator_range.second);
    variate_generator<mt19937&, uniform_smallint<> > random_number_of_operators(rng, small_operator_range);

    timer T;
    progress_display show_progress( number_of_cases );
    double total_time = 0;
    for(unsigned int i = 0; i < number_of_cases; ++i) {
        total_time += generate_and_test_solver(
            random_number_of_physical_qubits(),
            random_number_of_operators(),
            random_real()
        );
        ++show_progress;
    }
    cout << endl;
    cout << "All test cases completed successfully.  Took " << T.elapsed() << " seconds." << endl;
    cout << "Time spent computing codes was " << setprecision(2) << total_time << " second, or ~ ";
    if(total_time == 0)
        cout << "infinite";
    else
        cout << ((long)(number_of_cases/total_time));
    cout << " codes/second." << endl;
    cout << endl;
}
//@-others
//@-leo
