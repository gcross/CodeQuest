//@+leo-ver=4-thin
//@+node:gcross.20090521215822.19:@thin randomized-tests.cc
//@@language cplusplus

//@<< Headers >>
//@+node:gcross.20090521215822.20:<< Headers >>
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

#include "codelib.hpp"
//@-node:gcross.20090521215822.20:<< Headers >>
//@nl

//@<< Namespace Imports >>
//@+node:gcross.20090522205550.2:<< Namespace Imports >>
using namespace boost;
using namespace boost::assign;
using namespace std;
//@-node:gcross.20090522205550.2:<< Namespace Imports >>
//@nl

//@<< Random number generators >>
//@+node:gcross.20090522205550.6:<< Random number generators >>
mt19937 rng;
uniform_smallint<> one_through_three(1,3);
variate_generator<mt19937&, uniform_smallint<> > random_pauli(rng, one_through_three);
uniform_01<> u01;
variate_generator<mt19937&, uniform_01<> > random_real(rng, u01);
//@-node:gcross.20090522205550.6:<< Random number generators >>
//@nl

//@<< Typedefs >>
//@+node:gcross.20090522205550.7:<< Typedefs >>
typedef qec<dynamic_quantum_operator> dynamic_qec_type;

template<int nbits> struct static_qec {
    typedef qec<
        static_quantum_operator<nbits>,
        static_vector<qubit<static_quantum_operator<nbits> >,nbits>,
        static_vector<static_quantum_operator<nbits>,nbits*(nbits-1)>,
        static_vector<size_t,nbits>
      > type;
};
//@-node:gcross.20090522205550.7:<< Typedefs >>
//@nl

//@+others
//@+node:gcross.20090521215822.21:append_error
#define append_error(message)   {   \
    ostringstream error;            \
    error << message;               \
    errors.push_back(error.str());  \
}
//@-node:gcross.20090521215822.21:append_error
//@+node:gcross.20090521215822.22:compute_basis
typedef vector<pair<size_t,int> > basis_information;

template<class operator_vector> basis_information compute_basis(operator_vector& operators, const char name[], vector<string>& errors, bool reduce_first=true) {
    basis_information operator_basis;

    if(operators.size()==0) operator_basis;

    size_t number_of_physical_qubits = operators[0].length();

    if(reduce_first) reduce_row_echelon_split_representation(operators,true);

    for(int i = 0; i < operators.size(); ++i) {
        for(int mask = 1; mask <= 2; ++mask) {
            for(int column = 0; column < number_of_physical_qubits; ++column) {
                if(not (operators[i][column] & mask)) goto next_column;
                for(int j = 0; j < operators.size(); ++j)
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
//@-node:gcross.20090521215822.22:compute_basis
//@+node:gcross.20090521215822.23:contained_in
template<class quantum_operator,class operator_vector> bool contained_in(const quantum_operator& operator_,const operator_vector& basis_operators, const basis_information& basis_info) {
    quantum_operator residual = operator_;
    for(int i = 0; i < basis_operators.size() and not residual.is_identity(); ++i)
        if(residual[basis_info[i].first] & basis_info[i].second)
            residual *= basis_operators[i];
    return residual.is_identity();
}
//@-node:gcross.20090521215822.23:contained_in
//@+node:gcross.20090521215822.24:check_for_problems_in_code
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

    //@    << Check completeness >>
    //@+node:gcross.20090521215822.25:<< Check completeness >>
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

        for(int i = 0; i < sg_operators.size(); ++i) {
            if(not contained_in(sg_operators[i],input_operators,input_operators_basis)) {
                append_error("There exists a stabilizer or gauge qubit operator is not producable from the input operators!");
                break;
            }
        }

        for(int i = 0; i < operators.size(); ++i) {
            if(not contained_in(operators[i],sg_operators,sg_operators_basis)) {
                append_error("Input operator " << (i+1) << " is not producable from the stabilizer and gauge qubit operators!")
            }
        }
    }
    skip_completeness_test: ;
    //@-node:gcross.20090521215822.25:<< Check completeness >>
    //@nl

    //@    << Check commutators >>
    //@+node:gcross.20090521215822.26:<< Check commutators >>
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


    for(int i = 0; i < code.stabilizers.size(); ++i) {
        for(int j = i+1; j < code.stabilizers.size(); ++j)
            check_op_comm(i+1,code.stabilizers[i],"Stabilizer","",j+1,code.stabilizers[j],"Stabilizer","")
        for(int j = 0; j < code.gauge_qubits.size(); ++j) {
            check_op_comm(i+1,code.stabilizers[i],"Stabilizer","",j+1,code.gauge_qubits[j].X,"Gauge Qubit","'s X operator")
            check_op_comm(i+1,code.stabilizers[i],"Stabilizer","",j+1,code.gauge_qubits[j].Z,"Gauge Qubit","'s Z operator")
        }
        for(int j = 0; j < code.logical_qubits.size(); ++j) {
            check_op_comm(i+1,code.stabilizers[i],"Stabilizer","",j+1,code.logical_qubits[j].X,"Logical Qubit","'s X operator")
            check_op_comm(i+1,code.stabilizers[i],"Stabilizer","",j+1,code.logical_qubits[j].Z,"Logical Qubit","'s Z operator")
        }
    }

    for(int i = 0; i < code.gauge_qubits.size(); ++i) {
        check_op_anticomm(i+1,code.gauge_qubits[i].X,"Gauge Qubit","'s X operator",i+1,code.gauge_qubits[i].Z,"Gauge Qubit","'s Z operator")
        for(int j = i+1; j < code.gauge_qubits.size(); ++j) {
            check_op_comm(i+1,code.gauge_qubits[i].X,"Gauge Qubit","'s X operator",j+1,code.gauge_qubits[j].X,"Gauge Qubit","'s X operator")
            check_op_comm(i+1,code.gauge_qubits[i].X,"Gauge Qubit","'s X operator",j+1,code.gauge_qubits[j].Z,"Gauge Qubit","'s Z operator")
            check_op_comm(i+1,code.gauge_qubits[i].Z,"Gauge Qubit","'s Z operator",j+1,code.gauge_qubits[j].X,"Gauge Qubit","'s X operator")
            check_op_comm(i+1,code.gauge_qubits[i].Z,"Gauge Qubit","'s Z operator",j+1,code.gauge_qubits[j].Z,"Gauge Qubit","'s Z operator")
        }
        for(int j = 0; j < code.logical_qubits.size(); ++j) {
            check_op_comm(i+1,code.gauge_qubits[i].X,"Gauge Qubit","'s X operator",j+1,code.logical_qubits[j].X,"Logical Qubit","'s X operator")
            check_op_comm(i+1,code.gauge_qubits[i].X,"Gauge Qubit","'s X operator",j+1,code.logical_qubits[j].Z,"Logical Qubit","'s Z operator")
            check_op_comm(i+1,code.gauge_qubits[i].Z,"Gauge Qubit","'s Z operator",j+1,code.logical_qubits[j].X,"Logical Qubit","'s X operator")
            check_op_comm(i+1,code.gauge_qubits[i].Z,"Gauge Qubit","'s Z operator",j+1,code.logical_qubits[j].Z,"Logical Qubit","'s Z operator")
        }
    }


    for(int i = 0; i < code.logical_qubits.size(); ++i) {
        check_op_anticomm(i+1,code.logical_qubits[i].X,"Logical Qubit","'s X operator",i+1,code.logical_qubits[i].Z,"Logical Qubit","'s Z operator")
        for(int j = i+1; j < code.logical_qubits.size(); ++j) {
            check_op_comm(i+1,code.logical_qubits[i].X,"Logical Qubit","'s X operator",j+1,code.logical_qubits[j].X,"Logical Qubit","'s X operator")
            check_op_comm(i+1,code.logical_qubits[i].X,"Logical Qubit","'s X operator",j+1,code.logical_qubits[j].Z,"Logical Qubit","'s Z operator")
            check_op_comm(i+1,code.logical_qubits[i].Z,"Logical Qubit","'s Z operator",j+1,code.logical_qubits[j].X,"Logical Qubit","'s X operator")
            check_op_comm(i+1,code.logical_qubits[i].Z,"Logical Qubit","'s Z operator",j+1,code.logical_qubits[j].Z,"Logical Qubit","'s Z operator")
        }
    }
    //@-node:gcross.20090521215822.26:<< Check commutators >>
    //@nl

    if ((number_of_logical_qubits > 0) &&
        (number_of_stabilizers+2*number_of_gauge_qubits+2*number_of_logical_qubits <= 20)
    ) {
        //@        << Check distances >>
        //@+node:gcross.20100318131715.1383:<< Check distances >>
        vector<unsigned int> observed_distances(number_of_logical_qubits,number_of_physical_qubits);

        for(int chosen_stabilizers = 0;
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
            for(int chosen_gauge_qubit_X_operators = 0;
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
                for(int chosen_gauge_qubit_Z_operators = 0;
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
                    for(int chosen_logical_qubit_X_operators = 0;
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
                        for(int chosen_logical_qubit_Z_operators = (chosen_logical_qubit_X_operators == 0 ? 1 : 0);
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
                            const int distance = (
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

        for(int i = 0; i < number_of_logical_qubits; ++i) {
            if(observed_distances[i] != code.logical_qubit_error_distances[i]) {
                ostringstream error;
                error << "The computed distances for the logical qubits were [";
                for(int j = 0; j < number_of_logical_qubits-1; ++j)
                    error << code.logical_qubit_error_distances[j] << ",";
                error << code.logical_qubit_error_distances[number_of_logical_qubits-1] << "]";
                error << ", but the observed distances were [";
                for(int j = 0; j < number_of_logical_qubits-1; ++j)
                    error << observed_distances[j] << ",";
                error << observed_distances[number_of_logical_qubits-1] << "].";
                errors.push_back(error.str());
            }
        }
        //@-node:gcross.20100318131715.1383:<< Check distances >>
        //@nl
    }

    return errors;
}
//@-node:gcross.20090521215822.24:check_for_problems_in_code
//@+node:gcross.20100318202249.1389:generate_random_operators
template<class qec_type> typename qec_type::operator_vector generate_random_operators(
        int number_of_physical_qubits,
        int number_of_operators,
        float bernoulli_trial_probability
) {
    typename qec_type::operator_vector operators;
    operators.reserve(number_of_operators);
    for(int i = 0; i < number_of_operators; ++i) {
        typename qec_type::quantum_operator op(number_of_physical_qubits);
        while(op.weight() == 0) {
            for(int j = 0; j < number_of_physical_qubits; ++j) {
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
//@-node:gcross.20100318202249.1389:generate_random_operators
//@+node:gcross.20090522205550.4:generate_and_test_code
template<class qec_type> double generate_and_test_code(
        int number_of_physical_qubits,
        int number_of_operators,
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
//@-node:gcross.20090522205550.4:generate_and_test_code
//@+node:gcross.20090522205550.5:run_test_batch
template<class qec_type> bool run_test_batch(int batch_number, int number_of_cases, pair<int,int> qubit_range, pair<int,int> operator_range) {
    cout << "Test batch " << batch_number << ":" << endl;
    cout << "\tNumber of qubits: " << qubit_range.first << "-" << qubit_range.second << endl;
    cout << "\tNumber of operators: " << operator_range.first << "-" << operator_range.second << endl;
    cout << "Generating " << number_of_cases << " random test cases:";
    uniform_smallint<> small_qubit_range(qubit_range.first,qubit_range.second);
    variate_generator<mt19937&, uniform_smallint<> > random_number_of_physical_qubits(rng, small_qubit_range);
    uniform_smallint<> small_operator_range(operator_range.first,operator_range.second);
    variate_generator<mt19937&, uniform_smallint<> > random_number_of_operators(rng, small_operator_range);

    timer T;
    progress_display show_progress( 10000 );
    double total_time = 0;
    for(int i = 0; i < 10000; ++i) {
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
//@-node:gcross.20090522205550.5:run_test_batch
//@+node:gcross.20090522205550.8:run_test_batch_with_fixed_number_of_qubits
template<class qec_type> bool run_test_batch_with_fixed_number_of_qubits(int batch_number, int number_of_cases, int number_of_qubits, pair<int,int> operator_range) {
    cout << "Test batch " << batch_number << ":" << endl;
    cout << "\tNumber of qubits: " << number_of_qubits << endl;
    cout << "\tNumber of operators: " << operator_range.first << "-" << operator_range.second << endl;
    cout << "Generating " << number_of_cases << " random test cases:";
    uniform_smallint<> small_operator_range(operator_range.first,operator_range.second);
    variate_generator<mt19937&, uniform_smallint<> > random_number_of_operators(rng, small_operator_range);

    timer T;
    progress_display show_progress( 10000 );
    double total_time = 0;
    for(int i = 0; i < 10000; ++i) {
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
//@-node:gcross.20090522205550.8:run_test_batch_with_fixed_number_of_qubits
//@+node:gcross.20090521215822.27:main
int main(int argc, char** argv) {

    {
        cout << "TESTING ALGORITHM USING DYNAMIC VECTORS:" << endl << endl;
        typedef boost::tuple<int,pair<int,int>,pair<int,int> > test_parameters;
        const vector<test_parameters> tests = tuple_list_of
            (10000, make_pair(2,5), make_pair(1,3))
            (10000, make_pair(2,8), make_pair(1,8))
            (10000, make_pair(10,20), make_pair(1,40))
            //(200, make_pair(50,100), make_pair(1,200))
            //(10, make_pair(100,200), make_pair(1,400))
            ;
        for(int i = 0; i < tests.size(); ++i) {
            run_test_batch<dynamic_qec_type>
                (i+1,tests[i].get<0>(),tests[i].get<1>(),tests[i].get<2>());
        }
    }

    {
        cout << "TESTING ALGORITHM USING STATIC VECTORS FOR 4 QUBITS:" << endl << endl;
        typedef boost::tuple<int,pair<int,int> > test_parameters;
        const vector<test_parameters> tests = tuple_list_of
            (10000, make_pair(1,3))
            (10000, make_pair(1,8))
            ;
        for(int i = 0; i < tests.size(); ++i) {
            run_test_batch_with_fixed_number_of_qubits<static_qec<4>::type>
                (i+1,tests[i].get<0>(),4,tests[i].get<1>());
        }
    }

    {
        cout << "TESTING ALGORITHM USING STATIC VECTORS FOR 8 QUBITS:" << endl << endl;
        typedef boost::tuple<int,pair<int,int> > test_parameters;
        const vector<test_parameters> tests = tuple_list_of
            (10000, make_pair(1,8))
            (10000, make_pair(8,16))
            (10000, make_pair(1,50))
            ;
        for(int i = 0; i < tests.size(); ++i) {
            run_test_batch_with_fixed_number_of_qubits<static_qec<8>::type>
                (i+1,tests[i].get<0>(),8,tests[i].get<1>());
        }
    }

    {
        cout << "TESTING ALGORITHM USING STATIC VECTORS FOR 16 QUBITS:" << endl << endl;
        typedef boost::tuple<int,pair<int,int> > test_parameters;
        const vector<test_parameters> tests = tuple_list_of
            (10000, make_pair(1,16))
            (10000, make_pair(16,64))
            ;
        for(int i = 0; i < tests.size(); ++i) {
            run_test_batch_with_fixed_number_of_qubits<static_qec<16>::type>
                (i+1,tests[i].get<0>(),16,tests[i].get<1>());
        }
    }

    {
        cout << "TESTING ALGORITHM USING STATIC VECTORS FOR 32 QUBITS:" << endl << endl;
        typedef boost::tuple<int,pair<int,int> > test_parameters;
        const vector<test_parameters> tests = tuple_list_of
            (10000, make_pair(1,64))
            (10000, make_pair(64,256))
            ;
        for(int i = 0; i < tests.size(); ++i) {
            run_test_batch_with_fixed_number_of_qubits<static_qec<32>::type>
                (i+1,tests[i].get<0>(),32,tests[i].get<1>());
        }
    }

    {
        cout << "TESTING ALGORITHM USING STATIC VECTORS FOR 64 QUBITS:" << endl << endl;
        typedef boost::tuple<int,pair<int,int> > test_parameters;
        const vector<test_parameters> tests = tuple_list_of
            (10000, make_pair(1,256))
            ;
        for(int i = 0; i < tests.size(); ++i) {
            run_test_batch_with_fixed_number_of_qubits<static_qec<64>::type>
                (i+1,tests[i].get<0>(),64,tests[i].get<1>());
        }
    }

    return 0;
}
//@-node:gcross.20090521215822.27:main
//@-others
//@-node:gcross.20090521215822.19:@thin randomized-tests.cc
//@-leo
