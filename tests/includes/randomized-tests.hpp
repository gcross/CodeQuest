//@+leo-ver=5-thin
//@+node:gcross.20090521215822.19: * @thin randomized-tests.hpp
//@@language cplusplus

#ifndef RANDOMIZED_TESTS_HPP
#define RANDOMIZED_TESTS_HPP

//@+<< Headers >>
//@+node:gcross.20090521215822.20: ** << Headers >>
#include <vector>
#include <list>
#include <utility>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>

#include <boost/foreach.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_smallint.hpp>
#include <boost/random/uniform_01.hpp>
#include <boost/progress.hpp>
#include <boost/timer.hpp>
#include <boost/assign/list_of.hpp>

#include "codequest.hpp"
//@-<< Headers >>

//@+<< Namespace Imports >>
//@+node:gcross.20090522205550.2: ** << Namespace Imports >>
using namespace CodeQuest;
using namespace boost;
using namespace boost::assign;
using namespace std;
//@-<< Namespace Imports >>

//@+<< Random number generators >>
//@+node:gcross.20090522205550.6: ** << Random number generators >>
extern mt19937 rng;
extern uniform_smallint<> one_through_three;
extern variate_generator<mt19937&, uniform_smallint<> > random_pauli;
extern uniform_01<> u01;
extern variate_generator<mt19937&, uniform_01<> > random_real;
//@-<< Random number generators >>

//@+others
//@+node:gcross.20090521215822.21: ** append_error
#define append_error(message)   {   \
    ostringstream error;            \
    error << message;               \
    errors.push_back(error.str());  \
}
//@+node:gcross.20090521215822.22: ** compute_basis
typedef vector<pair<size_t,unsigned int> > basis_information;

template<class operator_vector> basis_information compute_basis(operator_vector& operators, const char name[], vector<string>& errors, bool reduce_first=true) {
    basis_information operator_basis;

    if(operators.size()==0) operator_basis;

    size_t number_of_physical_qubits = operators[0].length();

    if(reduce_first) reduce_row_echelon_split_representation(operators,true);

    for(unsigned int i = 0; i < operators.size(); ++i) {
        for(unsigned int mask = 1; mask <= 2; ++mask) {
            for(unsigned int column = 0; column < number_of_physical_qubits; ++column) {
                if(not (operators[i][column] & mask)) goto next_column;
                for(unsigned int j = 0; j < operators.size(); ++j)
                    if(i != j and (operators[j][column] & mask)) goto next_column;
                operator_basis.push_back(make_pair<size_t,int>(column,mask));
                goto next_operator;
                next_column: ;
            }
        }
        append_error(name << " operator #" << i << " does not have a single-qubit X or Z in a position that is shared by no other operator!")
        next_operator: ;
    }

    return operator_basis;
}
//@+node:gcross.20090521215822.23: ** contained_in
template<class quantum_operator,class operator_vector> bool contained_in(const quantum_operator& operator_,const operator_vector& basis_operators, const basis_information& basis_info) {
    quantum_operator residual = operator_;
    for(unsigned int i = 0; i < basis_operators.size() and not residual.is_identity(); ++i)
        if(residual[basis_info[i].first] & basis_info[i].second)
            residual *= basis_operators[i];
    return residual.is_identity();
}
//@+node:gcross.20090521215822.24: ** check_for_problems_in_code
template<class qec_type> vector<string> check_for_problems_in_code(const typename qec_type::operator_vector& operators, qec_type& code) {
    vector<string> errors;

    const size_t number_of_physical_qubits = code.number_of_physical_qubits,
                 number_of_stabilizers = code.stabilizers.size(),
                 number_of_gauge_qubits = code.gauge_qubits.size(),
                 number_of_logical_qubits = code.logical_qubits.size();

    typedef typename qec_type::quantum_operator quantum_operator;
    typedef typename qec_type::qubit_type qubit_type;
    typedef typename qec_type::qubit_vector qubit_vector;
    typedef typename qec_type::operator_vector operator_vector;
    typedef typename qec_type::index_vector index_vector;

    //@+<< Check completeness >>
    //@+node:gcross.20090521215822.25: *3* << Check completeness >>
    if(number_of_physical_qubits!=code.stabilizers.size()+code.gauge_qubits.size()+code.logical_qubits.size())
        append_error("There are " << number_of_physical_qubits << " physical qubits, but the computed code found " << code.stabilizers.size() << " stabilizers, " << code.gauge_qubits.size() << " gauge qubits and " << code.logical_qubits.size() << " logical qubits, which adds up to " << (code.stabilizers.size()+code.gauge_qubits.size()+code.logical_qubits.size()) << "!")

    size_t code_stabilizers_original_size = code.stabilizers.size();
    basis_information stabilizers_basis = compute_basis(code.stabilizers,"Stabilizer",errors);
    if(code.stabilizers.size() != code_stabilizers_original_size)
        append_error("Stabilizers are not independent!  Before reduced row escehlon there were " << code_stabilizers_original_size << " operators, and after there were " << code.stabilizers.size() << "operators!");

    operator_vector code_operators = code.stabilizers;
    BOOST_FOREACH(qubit_type& q, code.gauge_qubits) {
        code_operators.push_back(q.X);
        code_operators.push_back(q.Z);
    }
    operator_vector sg_operators = code_operators;
    BOOST_FOREACH(qubit_type& q, code.logical_qubits) {
        code_operators.push_back(q.X);
        code_operators.push_back(q.Z);
    }

    size_t code_operators_original_size = code_operators.size();
    compute_basis(code_operators,"Code",errors);
    if(code_operators.size() != code_operators_original_size)
        append_error("Code operators are not independent!  Before reduced row escehlon there were " << code_operators_original_size << " operators, and after there were " << code_operators.size() << "operators!");

    {

        basis_information sg_operators_basis = compute_basis(sg_operators,"Stabilizer or gauge qubit",errors);
        if(sg_operators.size() != sg_operators_basis.size()) {
            append_error("Error computing basis for stabilizer/gauge qubit operators!  Skipping remainder of completeness test.");
            goto skip_completeness_test;
        }

        operator_vector input_operators = operators;
        basis_information input_operators_basis = compute_basis(input_operators, "Input",errors);
        if(input_operators.size() != input_operators_basis.size()) {
            append_error("Error computing basis for input operators!  Skipping remainder of completeness test.");
            goto skip_completeness_test;
        }

        for(unsigned int i = 0; i < sg_operators.size(); ++i) {
            if(not contained_in(sg_operators[i],input_operators,input_operators_basis)) {
                append_error("There exists a stabilizer or gauge qubit operator is not producable from the input operators!");
                break;
            }
        }

        for(unsigned int i = 0; i < operators.size(); ++i) {
            if(not contained_in(operators[i],sg_operators,sg_operators_basis)) {
                append_error("Input operator " << (i+1) << " is not producable from the stabilizer and gauge qubit operators!")
            }
        }
    }
    skip_completeness_test: ;
    //@-<< Check completeness >>

    //@+<< Check commutators >>
    //@+node:gcross.20090521215822.26: *3* << Check commutators >>
    #define check_op_comm(index1,op1,prename1,postname1,index2,op2,prename2,postname2)              \
            if(not (op1 || op2)) {                                                                  \
                ostringstream error;                                                                \
                error << prename1 << " #" << index1 << " (" << op1 << ")" << postname1 << " and "   \
                      << prename2 << " #" << index2 << " (" << op2 << ")" << postname2 << " and "   \
                      << "do not commute!";                                                         \
                errors.push_back(error.str());                                                      \
            }

    #define check_op_anticomm(index1,op1,prename1,postname1,index2,op2,prename2,postname2)          \
            if(op1 || op2) {                                                                        \
                ostringstream error;                                                                \
                error << prename1 << " #" << index1 << " (" << op1 << ")" << postname1 << " and "   \
                      << prename2 << " #" << index2 << " (" << op2 << ")" << postname2 << " and "   \
                      << "do not anti-commute!";                                                    \
                errors.push_back(error.str());                                                      \
            }


    for(unsigned int i = 0; i < code.stabilizers.size(); ++i) {
        for(unsigned int j = i+1; j < code.stabilizers.size(); ++j)
            check_op_comm(i+1,code.stabilizers[i],"Stabilizer","",j+1,code.stabilizers[j],"Stabilizer","")
        for(unsigned int j = 0; j < code.gauge_qubits.size(); ++j) {
            check_op_comm(i+1,code.stabilizers[i],"Stabilizer","",j+1,code.gauge_qubits[j].X,"Gauge Qubit","'s X operator")
            check_op_comm(i+1,code.stabilizers[i],"Stabilizer","",j+1,code.gauge_qubits[j].Z,"Gauge Qubit","'s Z operator")
        }
        for(unsigned int j = 0; j < code.logical_qubits.size(); ++j) {
            check_op_comm(i+1,code.stabilizers[i],"Stabilizer","",j+1,code.logical_qubits[j].X,"Logical Qubit","'s X operator")
            check_op_comm(i+1,code.stabilizers[i],"Stabilizer","",j+1,code.logical_qubits[j].Z,"Logical Qubit","'s Z operator")
        }
    }

    for(unsigned int i = 0; i < code.gauge_qubits.size(); ++i) {
        check_op_anticomm(i+1,code.gauge_qubits[i].X,"Gauge Qubit","'s X operator",i+1,code.gauge_qubits[i].Z,"Gauge Qubit","'s Z operator")
        for(unsigned int j = i+1; j < code.gauge_qubits.size(); ++j) {
            check_op_comm(i+1,code.gauge_qubits[i].X,"Gauge Qubit","'s X operator",j+1,code.gauge_qubits[j].X,"Gauge Qubit","'s X operator")
            check_op_comm(i+1,code.gauge_qubits[i].X,"Gauge Qubit","'s X operator",j+1,code.gauge_qubits[j].Z,"Gauge Qubit","'s Z operator")
            check_op_comm(i+1,code.gauge_qubits[i].Z,"Gauge Qubit","'s Z operator",j+1,code.gauge_qubits[j].X,"Gauge Qubit","'s X operator")
            check_op_comm(i+1,code.gauge_qubits[i].Z,"Gauge Qubit","'s Z operator",j+1,code.gauge_qubits[j].Z,"Gauge Qubit","'s Z operator")
        }
        for(unsigned int j = 0; j < code.logical_qubits.size(); ++j) {
            check_op_comm(i+1,code.gauge_qubits[i].X,"Gauge Qubit","'s X operator",j+1,code.logical_qubits[j].X,"Logical Qubit","'s X operator")
            check_op_comm(i+1,code.gauge_qubits[i].X,"Gauge Qubit","'s X operator",j+1,code.logical_qubits[j].Z,"Logical Qubit","'s Z operator")
            check_op_comm(i+1,code.gauge_qubits[i].Z,"Gauge Qubit","'s Z operator",j+1,code.logical_qubits[j].X,"Logical Qubit","'s X operator")
            check_op_comm(i+1,code.gauge_qubits[i].Z,"Gauge Qubit","'s Z operator",j+1,code.logical_qubits[j].Z,"Logical Qubit","'s Z operator")
        }
    }


    for(unsigned int i = 0; i < code.logical_qubits.size(); ++i) {
        check_op_anticomm(i+1,code.logical_qubits[i].X,"Logical Qubit","'s X operator",i+1,code.logical_qubits[i].Z,"Logical Qubit","'s Z operator")
        for(unsigned int j = i+1; j < code.logical_qubits.size(); ++j) {
            check_op_comm(i+1,code.logical_qubits[i].X,"Logical Qubit","'s X operator",j+1,code.logical_qubits[j].X,"Logical Qubit","'s X operator")
            check_op_comm(i+1,code.logical_qubits[i].X,"Logical Qubit","'s X operator",j+1,code.logical_qubits[j].Z,"Logical Qubit","'s Z operator")
            check_op_comm(i+1,code.logical_qubits[i].Z,"Logical Qubit","'s Z operator",j+1,code.logical_qubits[j].X,"Logical Qubit","'s X operator")
            check_op_comm(i+1,code.logical_qubits[i].Z,"Logical Qubit","'s Z operator",j+1,code.logical_qubits[j].Z,"Logical Qubit","'s Z operator")
        }
    }
    //@-<< Check commutators >>

    //@+<< Check distances >>
    //@+node:gcross.20100318131715.1383: *3* << Check distances >>
    if (number_of_logical_qubits > 1) {
        for(unsigned int i = 0; i < number_of_logical_qubits-1; ++i) {
            if(code.logical_qubit_error_distances[i] > code.logical_qubit_error_distances[i+1]) {
                append_error("The computed distances for the logical qubits are not in ascending order!");
                break;
            }
        }
    }

    if ((number_of_logical_qubits > 0) &&
        (number_of_stabilizers+2*number_of_gauge_qubits+2*number_of_logical_qubits <= 20)
    ) {
        vector<unsigned int> observed_distances(number_of_logical_qubits,number_of_physical_qubits);

        for(unsigned int chosen_stabilizers = 0;
            chosen_stabilizers < (1 << number_of_stabilizers);
            ++chosen_stabilizers
        ) {
            unsigned int mask = 1;
            quantum_operator product_of_stabilizers(number_of_physical_qubits);
            BOOST_FOREACH(const quantum_operator& op, code.stabilizers) {
                if(mask & chosen_stabilizers)
                    product_of_stabilizers *= op;
                mask <<= 1;
            }
            for(unsigned int chosen_gauge_qubit_X_operators = 0;
                             chosen_gauge_qubit_X_operators < (1 << number_of_gauge_qubits);
                           ++chosen_gauge_qubit_X_operators
            ) {
                unsigned int mask = 1;
                quantum_operator product_of_gauge_X_operators(number_of_physical_qubits);
                BOOST_FOREACH(const qubit_type& qubit, code.gauge_qubits) {
                    if(mask & chosen_gauge_qubit_X_operators)
                        product_of_gauge_X_operators *= qubit.X;
                    mask <<= 1;
                }
                for(unsigned int chosen_gauge_qubit_Z_operators = 0;
                                 chosen_gauge_qubit_Z_operators < (1 << number_of_gauge_qubits);
                               ++chosen_gauge_qubit_Z_operators
                ) {
                    unsigned int mask = 1;
                    quantum_operator product_of_gauge_Z_operators(number_of_physical_qubits);
                    BOOST_FOREACH(const qubit_type& qubit, code.gauge_qubits) {
                        if(mask & chosen_gauge_qubit_Z_operators)
                            product_of_gauge_Z_operators *= qubit.Z;
                        mask <<= 1;
                    }
                    for(unsigned int chosen_logical_qubit_X_operators = 0;
                                     chosen_logical_qubit_X_operators < (1 << number_of_logical_qubits);
                                   ++chosen_logical_qubit_X_operators
                    ) {
                        unsigned int mask = 1;
                        quantum_operator product_of_logical_X_operators(number_of_physical_qubits);
                        BOOST_FOREACH(const qubit_type& qubit, code.logical_qubits) {
                            if(mask & chosen_logical_qubit_X_operators)
                                product_of_logical_X_operators *= qubit.X;
                            mask <<= 1;
                        }
                        for(unsigned int chosen_logical_qubit_Z_operators = (chosen_logical_qubit_X_operators == 0 ? 1 : 0);
                                chosen_logical_qubit_Z_operators < (1 << number_of_logical_qubits);
                                ++chosen_logical_qubit_Z_operators
                        ) {
                            unsigned int mask = 1;
                            quantum_operator product_of_logical_Z_operators(number_of_physical_qubits);
                            BOOST_FOREACH(const qubit_type& qubit, code.logical_qubits) {
                                if(mask & chosen_logical_qubit_Z_operators)
                                    product_of_logical_Z_operators *= qubit.Z;
                                mask <<= 1;
                            }
                            const unsigned int distance = (
                                product_of_stabilizers *
                                product_of_gauge_X_operators *
                                product_of_gauge_Z_operators *
                                product_of_logical_X_operators *
                                product_of_logical_Z_operators
                            ).weight();
                            mask = 1;
                            BOOST_FOREACH(unsigned int& observed_distance, observed_distances) {
                                if(((mask & chosen_logical_qubit_X_operators) || (mask & chosen_logical_qubit_Z_operators))
                                   && distance < observed_distance
                                  ) observed_distance = distance;
                                mask <<= 1;
                            }
                        }
                    }
                }
            }
        }

        for(unsigned int i = 0; i < number_of_logical_qubits; ++i) {
            if(observed_distances[i] != code.logical_qubit_error_distances[i]) {
                ostringstream error;
                error << "The computed distances disagree with the observed distances, which are [";
                for(unsigned int j = 0; j < number_of_logical_qubits-1; ++j)
                    error << observed_distances[j] << ",";
                error << observed_distances[number_of_logical_qubits-1] << "].";
                errors.push_back(error.str());
                break;
            }
        }
    }
    //@-<< Check distances >>

    return errors;
}
//@+node:gcross.20100318202249.1389: ** generate_random_operators
template<class qec_type> typename qec_type::operator_vector generate_random_operators(
        unsigned int number_of_physical_qubits,
        unsigned int number_of_operators,
        float bernoulli_trial_probability
) {
    typename qec_type::operator_vector operators;
    operators.reserve(number_of_operators);
    for(unsigned int i = 0; i < number_of_operators; ++i) {
        typename qec_type::quantum_operator op(number_of_physical_qubits);
        while(op.weight() == 0) {
            for(unsigned int j = 0; j < number_of_physical_qubits; ++j) {
                if(random_real() > bernoulli_trial_probability)
                    op.set(j,random_pauli());
                else
                    op.set(j,0);
            }
        }
        operators.push_back(op);
    }
    return operators;
}
//@+node:gcross.20090522205550.4: ** generate_and_test_code
template<class qec_type> double generate_and_test_code(
        unsigned int number_of_physical_qubits,
        unsigned int number_of_operators,
        float bernoulli_trial_probability
) {
    typename qec_type::operator_vector operators =
        generate_random_operators<qec_type>(
            number_of_physical_qubits,
            number_of_operators,
            bernoulli_trial_probability
        );

    timer T;
    qec_type code(operators);
    code.optimize_logical_qubits(false);
    double elapsed_time = T.elapsed();

    vector<string> errors = check_for_problems_in_code(operators,code);

    if(errors.size() > 0) {

        cout << "Operators are:" << endl;

        BOOST_FOREACH(typename qec_type::quantum_operator& op, operators) {
            cout << "\t" << op << endl;
        }

        cout << endl;


        cout << "Code is" << endl << code;


        cout << "Errors are:" << endl;

        BOOST_FOREACH(string& error, errors) { cout << error << endl; }

        throw exception();
    } else return elapsed_time;
}
//@+node:gcross.20100318202249.1391: ** generate_and_test_weight_minimization_problem
template<class quantum_operator> struct always_true {
    inline pair<bool,unsigned int> operator() (const quantum_operator & restrict op) const {
        return make_pair(true,42);
    }
};

template<class qec_type> void generate_and_test_weight_minimization_problem(
        unsigned int number_of_physical_qubits,
        unsigned int number_of_operators,
        float bernoulli_trial_probability
) {
    typedef typename qec_type::quantum_operator quantum_operator;
    typedef typename qec_type::operator_vector operator_vector;
    typedef typename qec_type::pseudo_generator_vector pseudo_generator_vector;
    typedef typename qec_type::index_vector index_vector;

    operator_vector operators =
        generate_random_operators<qec_type>(
            number_of_physical_qubits,
            number_of_operators,
            bernoulli_trial_probability
        );
    reduce_row_echelon_split_representation<operator_vector>(operators);
    number_of_operators = operators.size();
    operator_vector copy_of_operators_that_can_be_destroyed = operators;

    pseudo_generator_vector pseudo_generators =
        compute_pseudo_generators
            < operator_vector
            , pseudo_generator_vector
            > (copy_of_operators_that_can_be_destroyed);

    pair<quantum_operator,unsigned int> error_information = 
        compute_minimum_weight_operator<
            pseudo_generator_vector,
            index_vector,
            always_true<quantum_operator>,
            unsigned int
        > (
            pseudo_generators,
            always_true<quantum_operator>(),
            false
        );

    const quantum_operator & restrict minimum_weight_operator = error_information.first;
    bool minimum_weight_operator_has_been_observed = false;
    unsigned int minimum_weight_observed = number_of_physical_qubits+1;

    for(unsigned int chosen_operators = 1;
        chosen_operators < (1 << number_of_operators);
        ++chosen_operators
    ) {
        unsigned int mask = 1;
        quantum_operator product_of_operators(number_of_physical_qubits);
        BOOST_FOREACH(const quantum_operator& op, operators) {
            if(mask & chosen_operators)
                product_of_operators *= op;
            mask <<= 1;
        }
        const unsigned int weight = product_of_operators.weight();
        if(weight < minimum_weight_observed) minimum_weight_observed = weight;
        if(product_of_operators == minimum_weight_operator) minimum_weight_operator_has_been_observed = true;
    }

    vector<string> errors;

    if(!minimum_weight_operator_has_been_observed)
        append_error("The claimed minimal weight operator could not be obtained from a product of the operators!")
    if(minimum_weight_observed != minimum_weight_operator.weight())
        append_error("The minimal weight found by the algorithm was " << minimum_weight_operator.weight() << ", but the observed minimum weight was " << minimum_weight_observed << "!");

    if(errors.size() > 0) {

        cout << "Operators are:" << endl;

        BOOST_FOREACH(typename qec_type::quantum_operator& op, operators) {
            cout << "\t" << op << endl;
        }

        cout << endl;


        cout << "Minimum weight operator (with weight " << minimum_weight_operator.weight() << ") is:" << endl
             << "\t" << minimum_weight_operator << endl;


        cout << "Errors are:" << endl;

        BOOST_FOREACH(string& error, errors) { cout << error << endl; }

        throw exception();
    }
}
//@+node:gcross.20100318202249.1393: ** run_test_batch
template<class qec_type> bool run_test_batch(
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
        total_time += generate_and_test_code<qec_type>(
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
//@+node:gcross.20090522205550.5: ** run_weight_minimization_test_batch
template<class qec_type> bool run_weight_minimization_test_batch(
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
    for(unsigned int i = 0; i < number_of_cases; ++i) {
        generate_and_test_weight_minimization_problem<qec_type>(
            random_number_of_physical_qubits(),
            random_number_of_operators(),
            random_real()
        );
        ++show_progress;
    }
    cout << endl;
    cout << "All test cases completed successfully.  Took " << T.elapsed() << " seconds." << endl;
    cout << endl;
}
//@+node:gcross.20090522205550.8: ** run_test_batch_with_fixed_number_of_qubits
template<class qec_type> bool run_test_batch_with_fixed_number_of_qubits(
    unsigned int batch_number,
    unsigned int number_of_cases,
    unsigned int number_of_qubits,
    pair<unsigned int,unsigned int> operator_range
) {
    cout << "Test batch " << batch_number << ":" << endl;
    cout << "\tNumber of qubits: " << number_of_qubits << endl;
    cout << "\tNumber of operators: " << operator_range.first << "-" << operator_range.second << endl;
    cout << "Generating " << number_of_cases << " random test cases:";
    uniform_smallint<> small_operator_range(operator_range.first,operator_range.second);
    variate_generator<mt19937&, uniform_smallint<> > random_number_of_operators(rng, small_operator_range);

    timer T;
    progress_display show_progress( number_of_cases );
    double total_time = 0;
    for(unsigned int i = 0; i < number_of_cases; ++i) {
        total_time += generate_and_test_code<qec_type>(
            number_of_qubits,
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
//@+node:gcross.20100318202249.1395: ** run_weight_minimization_test_batch_with_fixed_number_of_qubits
template<class qec_type> bool run_weight_minimization_test_batch_with_fixed_number_of_qubits(
    unsigned int batch_number,
    unsigned int number_of_cases,
    unsigned int number_of_qubits,
    pair<unsigned int,unsigned int> operator_range
) {
    cout << "Test batch " << batch_number << ":" << endl;
    cout << "\tNumber of qubits: " << number_of_qubits << endl;
    cout << "\tNumber of operators: " << operator_range.first << "-" << operator_range.second << endl;
    cout << "Generating " << number_of_cases << " random test cases:";
    uniform_smallint<> small_operator_range(operator_range.first,operator_range.second);
    variate_generator<mt19937&, uniform_smallint<> > random_number_of_operators(rng, small_operator_range);

    timer T;
    progress_display show_progress( number_of_cases );
    for(unsigned int i = 0; i < number_of_cases; ++i) {
        generate_and_test_weight_minimization_problem<qec_type>(
            number_of_qubits,
            random_number_of_operators(),
            random_real()
        );
        ++show_progress;
    }
    cout << endl;
    cout << "All test cases completed successfully.  Took " << T.elapsed() << " seconds." << endl;
    cout << endl;
}
//@-others

#endif
//@-leo
