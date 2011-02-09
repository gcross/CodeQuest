//@+leo-ver=5-thin
//@+node:gmc.20080824181205.14: * @thin codequest.hpp
//@@language cplusplus

#ifndef CODEQUEST_H
#define CODEQUEST_H

#define restrict __restrict__

//@+<< Includes >>
//@+node:gmc.20080824181205.15: ** << Includes >>
#include <iostream>
#include <algorithm>
#include <vector>
#include <exception>
#include <boost/dynamic_bitset.hpp>
#include <boost/foreach.hpp>
#include <boost/range/irange.hpp>
#include <boost/range/iterator_range.hpp>
#include <bitset>
#include <string>
#include <functional>
#include <utility>
//@-<< Includes >>

namespace CodeQuest {

//@+<< Usings >>
//@+node:gcross.20101217153202.1438: ** << Usings >>
using boost::dynamic_bitset;
using boost::irange;
using boost::make_iterator_range;

using std::bitset;
using std::cout;
using std::copy;
using std::endl;
using std::exception;
using std::fill;
using std::make_pair;
using std::min;
using std::ostream;
using std::pair;
using std::string;
using std::vector;
//@-<< Usings >>

//@+others
//@+node:gmc.20080907163416.85: ** Exceptions
//@+others
//@+node:gmc.20080826115908.2: *3* invalid_pauli
class invalid_pauli : public exception {

public:

    string operator_string;
    size_t column;

    invalid_pauli(const string &operator_string, const size_t column) throw()
        : operator_string(operator_string), column(column) { }

    ~invalid_pauli() throw() {}

    const char* what() throw() { return "invalid pauli operator specificed"; }

};
//@-others
//@+node:gmc.20080824181205.27: ** Functions
//@+others
//@+node:gmc.20080907163416.84: *3* reduce_row_echelon_block_representation
template<class operator_vector> void reduce_row_echelon_block_representation(operator_vector& rows, const bool zero_upper=true) {
    if(rows.size()==0) return;
    typename operator_vector::iterator rref, rowref = rows.begin();
    size_t system_size = rowref->length();
    size_t column = 0;
    unsigned char op_mask = 1;

    while(rowref != rows.end() && column < system_size) {
        for(rref = rowref; rref != rows.end(); rref++) {
            if((*rref)[column] & op_mask)
                goto nonzero_found_in_column;
        }

        goto next;

    nonzero_found_in_column:

        if(rref!=rowref)
            iter_swap(rref,rowref);

        if(zero_upper) {
            for(rref = rows.begin(); rref != rowref; rref++)
                if((*rref)[column] & op_mask)
                    (*rref) *= (*rowref);
        }

        for(rref = rowref+1; rref != rows.end(); rref++)
            if((*rref)[column] & op_mask)
                (*rref) *= (*rowref);

        rowref += 1;

    next:

        if(op_mask == 1) {
            op_mask = 2;
        } else {
            op_mask = 1;
            column++;
        }

    }
    rows.erase(rowref,rows.end());
}
//@+node:gcross.20090526153741.1: *3* reduce_row_echelon_split_representation
template<class operator_vector> void reduce_row_echelon_split_representation(operator_vector& rows, const bool zero_upper=false) {
    if(rows.size()==0) return;
    typename operator_vector::iterator rref, rowref = rows.begin();
    size_t system_size = rowref->length();
    size_t column = 0;
    unsigned char op_mask = 1;
    while(rowref != rows.end() && (op_mask == 1 || column < system_size)) {
        if(column >= system_size) {
            op_mask = 2;
            column -= system_size;
        }
        for(rref = rowref; rref != rows.end(); rref++)
            if((*rref)[column] & op_mask)
                goto nonzero_found_in_column;

        column += 1;
        continue;

    nonzero_found_in_column:

        if(rref!=rowref)
            iter_swap(rref,rowref);

        if(zero_upper) {
            for(rref = rows.begin(); rref != rowref; rref++)
                if((*rref)[column] & op_mask)
                    (*rref) *= (*rowref);
        }

        for(rref = rowref+1; rref != rows.end(); rref++)
            if((*rref)[column] & op_mask)
                (*rref) *= (*rowref);

        rowref += 1;
        column += 1;
    }
    rows.erase(rowref,rows.end());
}
//@+node:gcross.20100318202249.1411: *3* compute_minimum_weight_operator
template<
    class pseudo_generator_vector,
    class index_vector,
    class query_function_type,
    class query_result_type
    >
inline pair<typename pseudo_generator_vector::value_type::quantum_operator,query_result_type> compute_minimum_weight_operator(
        const pseudo_generator_vector & restrict pseudo_generators,
        const query_function_type & restrict query_function,
        const bool verbose = false
) {
    typedef typename pseudo_generator_vector::value_type::quantum_operator quantum_operator;
    typedef typename quantum_operator::bitset_type bitset;
    typedef typename index_vector::value_type uint;

    //@+<< Nested classes >>
    //@+node:gcross.20101217153202.1442: *4* << Nested classes >>
    //@+others
    //@+node:gcross.20101217153202.1443: *5* ChoiceIterator
    class ChoiceIterator : public index_vector {

    public:

        const uint n, k;
        bool valid;

        ChoiceIterator(const uint n,const uint k) : index_vector(k), n(n), k(k), valid(true) {
            assert(k <= n);
            BOOST_FOREACH(const uint i, irange((uint)0u,k)) { (*this)[i] = k-1-i; }
        }


        operator bool() { return valid; }

        int increment(const uint pivot_index) {
            uint& pivot = (*this)[pivot_index];
            pivot++;
            if(pivot == n-pivot_index) {
                if(pivot_index == k-1) {
                    valid = false;
                    return -2;
                } else {
                    pivot = increment(pivot_index+1)+1;
                    if(pivot<0) return -2;
                    if(pivot==n) {
                        valid = false;
                        return -2;
                    }
                }
            }

            return pivot;

        }

        void operator ++() { increment(0); }

    };
    //@+node:gcross.20101217153202.1444: *5* CoefficientIterator
    class CoefficientIterator : public index_vector {

    public:

        const index_vector maxes;
        bool valid;

        CoefficientIterator(const index_vector& maxes) : index_vector(maxes.size(),1), maxes(maxes), valid(true) { }

        operator bool() { return valid; }

        void increment(uint index) {
            if(index == -1) {
                valid = false;
            } else if (maxes[index] == 2) {
                increment(index-1);
            } else {
                uint& coefficient = (*this)[index];
                ++coefficient;
                if(coefficient == maxes[index]) {
                    coefficient = 1;
                    increment(index-1);
                }
            }
        }

        void operator ++() { increment(index_vector::size()-1); }

    };
    //@-others
    //@-<< Nested classes >>

    uint r = 0;

    const size_t number_of_physical_qubits = pseudo_generators[0].number_of_qubits(),
                 number_of_pseudo_generators = pseudo_generators.size();
    unsigned int minimum_weight_found = number_of_physical_qubits+1;
    quantum_operator minimum_weight_operator(number_of_physical_qubits);
    query_result_type minimum_weight_query_result;

    quantum_operator op(number_of_physical_qubits);

    bitset bits;
    quantum_operator::resize_bitset(bits,number_of_physical_qubits);
    bits.reset();

    while(minimum_weight_found > (r+1) && r < number_of_pseudo_generators) {
        r += 1;

        if(verbose)
            cout << "\tSearching for errors with weight <= " << (r+1) << " ..." << endl;

        ChoiceIterator choices(number_of_pseudo_generators,r);

        while(choices.valid) {

            index_vector field_sizes(r);
            BOOST_FOREACH(const uint i, irange<uint>(0u,r)) {
                field_sizes[i] = pseudo_generators[choices[i]].field_size;
            }

            CoefficientIterator coefficients(field_sizes);

            while(coefficients.valid) {
                pseudo_generators[choices[0]].set(op,coefficients[0]);
                BOOST_FOREACH(const uint i, irange<uint>(1u,r)) {
                    pseudo_generators[choices[i]].multiply(op,coefficients[i]);
                }

                bits = op.X;
                bits |= op.Z;
                size_t weight = bits.count();

                if (weight < minimum_weight_found) {

                    pair<bool,query_result_type> test_result = query_function(op);
                    if(test_result.first) {
                        minimum_weight_found = weight;
                        minimum_weight_operator = op;
                        minimum_weight_query_result = test_result.second;
                        if(weight == r) goto return_found_operator;
                    }
                }

                ++coefficients;
            }

            ++choices;

        }
    }

    return_found_operator:

    return make_pair(minimum_weight_operator,minimum_weight_query_result);

}
//@+node:gcross.20100910154654.1413: *3* compute_pseudo_generators
template<
    class operator_vector,
    class pseudo_generator_vector
    >
inline pseudo_generator_vector compute_pseudo_generators(operator_vector & restrict operators) {
    typedef typename pseudo_generator_vector::value_type pseudo_generator;

    reduce_row_echelon_block_representation<operator_vector>(operators);

    typename operator_vector::const_iterator rowref = operators.begin();
    unsigned int column = 0;

    pseudo_generator_vector pseudo_generators;
    while(rowref != operators.end()) {
        if(not (*rowref)[column]) {
            column++;
        } else if((rowref+1) != operators.end() and (*(rowref+1))[column]) {
            pseudo_generators.push_back(pseudo_generator(*rowref,*(rowref+1)));
            rowref += 2;
            column++;
        } else {
            pseudo_generators.push_back(pseudo_generator(*rowref));
            rowref += 1;
            column++;
        }
    }

    return pseudo_generators;
}
//@-others
//@+node:gmc.20080824181205.16: ** Classes
//@+others
//@+node:gmc.20080824181205.17: *3* quantum_operator
template<class bitset> struct quantum_operator {

    typedef bitset bitset_type;

    bitset X, Z;

    inline quantum_operator() { }
    inline quantum_operator(const bitset& X_, const bitset& Z_) : X(X_), Z(Z_) { }

    bool inline is_identity() const {
        return (X.none()) && (Z.none());
    }

    bool inline operator||(const quantum_operator& restrict op) const {
        return ((X&op.Z).count()+(Z&op.X).count()) % 2 == 0;
    }

    bool inline operator&&(const quantum_operator& restrict op) const {
        return ((X&op.Z).count()+(Z&op.X).count()) % 2 == 1;
    }

    void inline operator*=(const quantum_operator& op) { X ^= op.X; Z ^= op.Z; }

    bool inline operator==(const quantum_operator& op) const { return (X==op.X) && (Z==op.Z); }
    //bool inline operator!=(const quantum_operator& op) const { return not ((*this)==op); }

    unsigned char inline operator[](size_t index) const {
        return (X.test(index)? 1 : 0) | (Z.test(index)? 2 : 0);
    }

    void inline set(const size_t index, const unsigned char new_value) {
        X.set(index,new_value & 1);
        Z.set(index,new_value & 2);
    }

    size_t inline weight() const { return (X|Z).count(); }

    void inline reset() { X.reset(); Z.reset(); }

    inline char pauli_char_at(const size_t qubit_number) const {
        if(X.test(qubit_number)) {
            if(Z.test(qubit_number))
                return 'Y';
            else
                return 'X';
        } else if(Z.test(qubit_number))
            return 'Z';
        else
            return '.';
    }

    string to_string() const {
        string pauli_string;
        size_t length = X.size();
        pauli_string.reserve(length);
        BOOST_FOREACH(const size_t i, irange((size_t)0,length)) {
            pauli_string.push_back(pauli_char_at(i));
        }
        return pauli_string;
    }

};

struct dynamic_quantum_operator : public quantum_operator<dynamic_bitset<unsigned int> > {

    typedef dynamic_bitset<unsigned int> BitsetType;
    typedef quantum_operator<BitsetType> BaseType;
    using BaseType::X;
    using BaseType::Z;

    inline size_t length() const { return X.size(); }

    inline dynamic_quantum_operator(const size_t len) : BaseType(BitsetType(len),BitsetType(len)) { }

    inline dynamic_quantum_operator() : BaseType(BitsetType(0), BitsetType(0)) {}

    void inline resize(const size_t newlen, const bool value = false) {
        X.resize(newlen,value);
        Z.resize(newlen,value);
    }

    static void inline resize_bitset(BitsetType& bitset, const size_t newlen, const bool value = false) {
        bitset.resize(newlen,value);
    }

};

typedef vector<dynamic_quantum_operator> dynamic_operator_vector;

template<unsigned int number_of_bits> struct static_quantum_operator : public quantum_operator<bitset<number_of_bits> >{

    inline static_quantum_operator() { }
    inline static_quantum_operator(size_t len) { assert(len==number_of_bits); }

    inline size_t length() const { return number_of_bits; };

    void inline resize(const size_t newlen, const bool value = false) {
        assert(newlen<=number_of_bits);
    }

    static void inline resize_bitset(bitset<number_of_bits>& bitset, const size_t newlen, const bool value = false) {
        assert(newlen<=number_of_bits);
    }
};

template<class quantum_operator> quantum_operator inline operator*(const quantum_operator& restrict A, const quantum_operator& restrict B) {
    quantum_operator op(A);
    op *= B;
    return op;
}
//@+node:gmc.20080824181205.18: *3* qubit
template<class quantum_operator> struct qubit {
    quantum_operator X, Y, Z;

    inline qubit() {}

    inline qubit(const quantum_operator& restrict X,const quantum_operator& restrict Z) : X(X), Z(Z), Y(Z*X) {}
    inline qubit(const size_t number_of_physical_qubits) : X(number_of_physical_qubits), Z(number_of_physical_qubits), Y(number_of_physical_qubits) {}

    inline bool operator||(const quantum_operator& op) const { return (X||op) and (Z||op); }
};

template<class quantum_operator> inline bool operator||(const quantum_operator& restrict op, const qubit<quantum_operator>& restrict q) { return q||op; }

template<class bitset> ostream& operator<<(ostream& out, const quantum_operator<bitset>& op) {
    out << op.to_string();
    return out;
}

typedef qubit<dynamic_quantum_operator> dynamic_qubit;
typedef vector<dynamic_qubit> dynamic_qubit_vector;
//@+node:gmc.20080910123558.5: *3* pseudo_generator
template<class Quantum_operator> class pseudo_generator {

public:

    typedef Quantum_operator quantum_operator;

    unsigned int field_size;

    quantum_operator op_1, op_2;

    pseudo_generator() { }
    pseudo_generator(const quantum_operator& restrict op_1_) : op_1(op_1_), field_size(2) { }
    pseudo_generator(const quantum_operator& restrict op_1_, const quantum_operator& restrict op_2_) : op_1(op_1_), op_2(op_2_), field_size(4) { }

    inline void multiply(quantum_operator& restrict op, unsigned int coefficient) const {
        if(coefficient & 1) op *= op_1;
        if(coefficient & 2) op *= op_2;
    }

    inline void set(quantum_operator& restrict op, unsigned int coefficient) const {
        if(coefficient & 1) {
            op = op_1;
            if(coefficient & 2) op *= op_2;
        } else if (coefficient & 2) op = op_2;
    }

    inline size_t number_of_qubits() const { return op_1.length(); }

};
//@+node:gcross.20081201142225.2: *3* static_vector
template<typename T,unsigned int buffer_size> class static_vector {

public:

    typedef T* iterator;
    typedef const T* const_iterator;
    typedef T value_type;

    static_vector() : end_ptr(data), current_size(0) { }
    static_vector(size_t size) : end_ptr(data+size), current_size(size) { }
    static_vector(size_t size, T fill) : end_ptr(data+size), current_size(size) {
        BOOST_FOREACH(T& x, *this) {
            x = fill;
        }
    }
    static_vector(const static_vector& old) : end_ptr(data+old.size()), current_size(old.size()) {
        copy(old.begin(),old.end(),begin());
    }

    void operator=(const static_vector& old) {
        end_ptr = data+old.size();
        current_size = old.size();
        copy(old.begin(),old.end(),begin());
    }

    inline iterator begin() { return data; }
    inline iterator end() { return end_ptr; }
    inline const_iterator begin() const { return data; }
    inline const_iterator end() const { return end_ptr; }

    inline size_t size() const { return current_size; }

    inline void resize(const size_t new_size) {
        end_ptr = data+new_size;
        current_size = new_size;
    }

    inline void resize(const size_t new_size, const T& new_value) {
        fill(data+current_size,data+new_size,new_value);
        resize(new_size);
    }

    inline void push_back(const T& element) {
        (*end_ptr) = element;
        ++end_ptr;
        ++current_size;
        assert(current_size <= buffer_size);
    }

    inline void reserve(const size_t size) {
        assert(size <= buffer_size);
    }

    inline void erase(const iterator ptr) {
        erase(ptr,ptr+1);
    }

    inline void erase(const iterator start, const iterator finish) {
        end_ptr = copy(finish,end(),start);
        current_size = end_ptr-data;
    }

    inline void insert(const iterator position, const iterator start, const iterator finish) {
        size_t number_of_items = finish-start;
        end_ptr = copy_backward(position,end(),position+number_of_items);
        current_size = end_ptr-data;
        copy(start,finish,position);
    }

    inline void clear() {
        end_ptr = data;
        current_size = 0;
    }

    inline T& operator[](const unsigned int index) { return data[index]; }
    inline const T& operator[](const unsigned int index) const { return data[index]; }

protected:

    T data[buffer_size];

    T* end_ptr;
    size_t current_size;

};
//@+node:gcross.20090526161317.2: *3* gaussian_elimination_state
template<class quantum_operator, class operator_vector, class index_vector> struct gaussian_elimination_state {
    //@+<< Fields >>
    //@+node:gcross.20090526161317.3: *4* << Fields >>
    typename quantum_operator::bitset_type indices_taken, paulis_chosen;
    index_vector qubit_indices_chosen;
    size_t current_operator_index;
    const size_t number_of_physical_qubits;
    //@-<< Fields >>
    //@+others
    //@+node:gcross.20090526161317.4: *4* constructor
    gaussian_elimination_state(const unsigned int number_of_physical_qubits) :
        number_of_physical_qubits(number_of_physical_qubits),
        current_operator_index(0)
    {
        qubit_indices_chosen.reserve(number_of_physical_qubits);
        quantum_operator::resize_bitset(indices_taken,number_of_physical_qubits);
        quantum_operator::resize_bitset(paulis_chosen,number_of_physical_qubits);
    }
    //@+node:gcross.20090526161317.5: *4* run_elimination
    void run_elimination(operator_vector& operators) {
        //@+<< Perform the Gaussian elimination >>
        //@+node:gcross.20090526161317.6: *5* << Perform the Gaussian elimination >>
        //@+at
        // Initialize some iterators.
        //@@c
        typename operator_vector::iterator op_overwrite_iter = operators.begin()+current_operator_index;
        //@+at
        // Loop over the remaining operators.
        //@@c
        BOOST_FOREACH(quantum_operator op, operators) {
        //@+at
        // Loop over the previous operators and make sure that operator is independent of them.
        //@@c
            typename index_vector::iterator qubit_index_chosen_iter = qubit_indices_chosen.begin();
            size_t op2_index = 0;
            BOOST_FOREACH(const quantum_operator& op2, make_pair(operators.begin(),op_overwrite_iter)) {
                // Check whether op contains either X or Z, depending on pauli chosen,
                // and if so multiply it by op_factor to get rid of the pauli.
                if(paulis_chosen[op2_index] ? op.X.test(*qubit_index_chosen_iter) : op.Z.test(*qubit_index_chosen_iter)) op *= op2;
                ++qubit_index_chosen_iter;
                ++op2_index;
            }
        //@+at
        // If the operator is now the identity, then skip it and move to the next.
        //@@c
            if(op.is_identity()) continue;
        //@+at
        // Look for the first physical qubit at a non-taken index with a non-trivial Pauli.
        //@@c
            unsigned int qubit_index_chosen;
            bool pauli_chosen;
            for(qubit_index_chosen = 0; qubit_index_chosen < number_of_physical_qubits; ++qubit_index_chosen) {
                if(indices_taken[qubit_index_chosen]) continue;
                if(op.X.test(qubit_index_chosen)) {
                    pauli_chosen = true;
                    break;
                }
                if(op.Z.test(qubit_index_chosen)) {
                    pauli_chosen = false;
                    break;
                }
            }
            assert(qubit_index_chosen < number_of_physical_qubits);
        //@+at
        // Loop over the previous operators and make sure that operator is independent of them.
        //@@c
            if(pauli_chosen)
                BOOST_FOREACH(quantum_operator& op2, make_pair(operators.begin(),op_overwrite_iter)) {
                    if(op2.X.test(qubit_index_chosen)) op2 *= op;
                }
            else
                BOOST_FOREACH(quantum_operator& op2, make_pair(operators.begin(),op_overwrite_iter)) {
                    if(op2.Z.test(qubit_index_chosen)) op2 *= op;
                }
        //@+at
        // Store the index and pauli chosen, copy the operator to the overwrite index, and increment the current operator index.
        //@@c
            indices_taken.set(qubit_index_chosen);
            qubit_indices_chosen.push_back(qubit_index_chosen);
            paulis_chosen[current_operator_index++] = pauli_chosen;
            *(op_overwrite_iter++) = op;
        }
        //@+at
        // Erase everything after the last operator.
        //@@c
        operators.erase(op_overwrite_iter,operators.end());
        //@-<< Perform the Gaussian elimination >>
    }
    //@-others
};

//@+node:gmc.20080824181205.19: *3* qec
template<
    class Quantum_operator = dynamic_quantum_operator,
    class Qubit_vector = vector<qubit<Quantum_operator> >,
    class Operator_vector = vector<Quantum_operator>,
    class Index_vector = vector<size_t>,
    class Pseudo_generator_vector = vector<pseudo_generator<Quantum_operator> >
> struct qec {

    //@+<< Typedefs >>
    //@+node:gcross.20090521215822.9: *4* << Typedefs >>
    typedef Quantum_operator quantum_operator;
    typedef Qubit_vector qubit_vector;
    typedef Operator_vector operator_vector;
    typedef Index_vector index_vector;
    typedef Pseudo_generator_vector pseudo_generator_vector;

    typedef typename quantum_operator::bitset_type bitset;
    typedef qubit<quantum_operator> qubit_type;
    typedef typename qubit_vector::iterator qubit_iterator;
    typedef typename qubit_vector::const_iterator const_qubit_iterator;
    typedef typename operator_vector::iterator operator_iterator;
    typedef typename operator_vector::const_iterator const_operator_iterator;
    typedef typename index_vector::iterator index_iterator;
    typedef gaussian_elimination_state<quantum_operator,operator_vector,index_vector> gaussian_elimination_state_type;
    //@-<< Typedefs >>

    //@+<< Fields >>
    //@+node:gcross.20090521215822.10: *4* << Fields >>
    const size_t number_of_physical_qubits;
    bool optimized;

    operator_vector stabilizers;
    qubit_vector gauge_qubits, logical_qubits;

    gaussian_elimination_state_type post_stabilizer_elimination_state;

    index_vector logical_qubit_error_distances;
    operator_vector logical_qubit_errors;
    //@-<< Fields >>

    //@+others
    //@+node:gcross.20100318162833.1399: *4* Nested classes
    //@+node:gcross.20100318162833.1391: *5* anti_commutes_with_some_logical
    struct anti_commutes_with_some_logical {

        const bitset & restrict marked_as_eligible_to_fix_an_error;
        const const_qubit_iterator logical_qubits_begin,
                                   logical_qubits_end_of_optimized,
                                   logical_qubits_end;

        inline anti_commutes_with_some_logical(
                const qubit_vector & logical_qubits,
                const unsigned int number_of_optimized_logical_qubits,
                const bitset & restrict marked_as_eligible_to_fix_an_error_
        ) : logical_qubits_begin(logical_qubits.begin()),
            logical_qubits_end_of_optimized(logical_qubits.begin() + number_of_optimized_logical_qubits),
            logical_qubits_end(logical_qubits.end()),
            marked_as_eligible_to_fix_an_error(marked_as_eligible_to_fix_an_error_)
            { }

        inline pair<bool,pair<unsigned int,unsigned int> > operator() (const quantum_operator & restrict op) const {
            const_qubit_iterator qubitref = logical_qubits_begin;

            // Case 1
            unsigned int index = 0;
            for(; qubitref!=logical_qubits_end_of_optimized; ++qubitref) {
                if(marked_as_eligible_to_fix_an_error[index] && (op && qubitref->Z)) {
                    return make_pair(true,make_pair(1,index));
                }
                ++index;
            }

            // Case 2
            for(; qubitref!=logical_qubits_end; ++qubitref) {
                if((op && qubitref->X) || (op && qubitref->Z)) {
                    return make_pair(true,make_pair(2,qubitref - logical_qubits_begin));
                }
            }

            return make_pair(false,make_pair(0,-1));
        }

    };
    //@+node:gmc.20080824181205.26: *4* constructor
    qec(const operator_vector& operators, const bool compute_logicals=true) :
        number_of_physical_qubits(operators[0].length()),
        post_stabilizer_elimination_state(operators[0].length()),
        optimized(false)
    {

        if(operators.size()==0) return;

        stabilizers.reserve(operators.size());
        gauge_qubits.reserve(min(operators.size()/2,number_of_physical_qubits));

        //@+<< Build table of stabilizers and gauge qubits >>
        //@+node:gmc.20080824181205.34: *5* << Build table of stabilizers and gauge qubits >>
        BOOST_FOREACH(const quantum_operator& op_, operators) {
        quantum_operator op = op_;

        //@+at
        // Make sure that this operator commutes with all of the gauge qubit operators.
        //@@c
            BOOST_FOREACH(const qubit_type& gauge_qubit, gauge_qubits) {
                if(!(op||gauge_qubit.Z)) op *= gauge_qubit.X;
                if(!(op||gauge_qubit.X)) op *= gauge_qubit.Z;
            }
        //@+at
        // If the remaining operator is trivial, then we are done.
        //@@c
            if(op.is_identity()) continue;
        //@+at
        // Otherwise, see if it commutes with all of the stabilizers.
        //@@c
            operator_iterator first_found_conjugal_partner = stabilizers.begin();
            while(first_found_conjugal_partner != stabilizers.end() and ((*first_found_conjugal_partner)||op))
                ++first_found_conjugal_partner;
        //@+at
        // If all stabilizer elements commute with this operator, then add this operator to the list of stabilizers and continue to the next one.
        //@@c
            if(first_found_conjugal_partner==stabilizers.end()) {
                stabilizers.push_back(op);
                continue;
            }
        //@+at
        // Otherwise, make a gauge qubit out of these two anti-commuting operators.
        //@@c
            qubit_type gauge_qubit(*first_found_conjugal_partner,op);
            gauge_qubits.push_back(gauge_qubit);
        //@+at
        // We now need to make sure that all of the stabilizers commute with op by multiplying those which done by op's conjugal partner.  While we are doing this, we simultaneously filter out all of the duplicates of the stabilizer we chose to be op's conjugal partner.
        //@@c
            operator_iterator next_stabilizer_to_overwrite = stabilizers.begin();
            BOOST_FOREACH(const quantum_operator& stabilizer, stabilizers) {
                if(stabilizer==gauge_qubit.X) continue;
                *(next_stabilizer_to_overwrite++) = (stabilizer || op) ? stabilizer : stabilizer * gauge_qubit.X;
            }
            stabilizers.erase(next_stabilizer_to_overwrite,stabilizers.end());
        }

        //@+at
        // Now we have a set of stabilizers, but they might not be linearly independent.  We perform a gaussian elimination to fix this.
        //@@c
        post_stabilizer_elimination_state.run_elimination(stabilizers);
        //@-<< Build table of stabilizers and gauge qubits >>

        if(compute_logicals) {
            logical_qubits.reserve(number_of_logical_qubits());
            recompute_logical_qubits();
            assert(logical_qubits.size() == number_of_logical_qubits());
        }

    }

    //@+node:gcross.20081119221421.3: *4* optimize_logical_qubits
    void optimize_logical_qubits(const bool verbose=false) {

        const size_t number_of_logical_qubits = logical_qubits.size();

        if(number_of_logical_qubits == 0) return;
        if(optimized) return;

        logical_qubit_error_distances.reserve(number_of_logical_qubits);
        logical_qubit_errors.reserve(number_of_logical_qubits);

        operator_vector list_of_operators_that_commute_with_all_stabilizers = stabilizers;

        BOOST_FOREACH(const qubit_type& qubit, gauge_qubits) {
            list_of_operators_that_commute_with_all_stabilizers.push_back(qubit.X);
            list_of_operators_that_commute_with_all_stabilizers.push_back(qubit.Z);
        }

        BOOST_FOREACH(const qubit_type& qubit, logical_qubits) {
            list_of_operators_that_commute_with_all_stabilizers.push_back(qubit.X);
            list_of_operators_that_commute_with_all_stabilizers.push_back(qubit.Z);
        }

        pseudo_generator_vector pseudo_generators =
            compute_pseudo_generators
                <operator_vector
                ,pseudo_generator_vector
                >
                (list_of_operators_that_commute_with_all_stabilizers);

        unsigned int number_of_optimized_logical_qubits = 0;

        bitset marked_as_eligible_to_fix_an_error;
        quantum_operator::resize_bitset(marked_as_eligible_to_fix_an_error,logical_qubits.size());
        marked_as_eligible_to_fix_an_error.set();

        while (number_of_optimized_logical_qubits < number_of_logical_qubits) {

            anti_commutes_with_some_logical test_function(
                logical_qubits,
                number_of_optimized_logical_qubits,
                marked_as_eligible_to_fix_an_error
            );

            pair<quantum_operator,const pair<unsigned int,unsigned int> > error_information = 
                compute_minimum_weight_operator <
                    pseudo_generator_vector,
                    index_vector,
                    anti_commutes_with_some_logical,
                    pair<unsigned int,unsigned int>
                > (
                    pseudo_generators,
                    test_function,
                    verbose
                );

            const quantum_operator & restrict error = error_information.first;
            const unsigned int & restrict error_case = error_information.second.first,
                               & restrict error_index = error_information.second.second;

            const qubit_iterator logical_qubits_begin = logical_qubits.begin(),
                                 logical_qubits_end   = logical_qubits.end();
            qubit_iterator logical_qubits_end_of_optimized = logical_qubits_begin + number_of_optimized_logical_qubits,
                           qubit_fixing_the_error_ref = logical_qubits_begin + error_index;

            assert(1 == error_case || 2 == error_case);
            unsigned int index = 3453456;
            switch(error_case) {
                case 1:
                    //@+<< Case 1 >>
                    //@+node:gcross.20100208221030.1400: *5* << Case 1 >>
                    marked_as_eligible_to_fix_an_error[error_index] = false;
                    assert(error && qubit_fixing_the_error_ref->Z);
                    index = error_index+1;
                    BOOST_FOREACH(
                        qubit_type& qubit_to_fix,
                        make_iterator_range(qubit_fixing_the_error_ref+1,logical_qubits_end_of_optimized)
                    ) {
                        if(marked_as_eligible_to_fix_an_error[index] && (error && qubit_to_fix.Z)) {
                            qubit_fixing_the_error_ref->X *= qubit_to_fix.X;
                            qubit_to_fix.Z *= qubit_fixing_the_error_ref->Z;
                        }
                        assert((!marked_as_eligible_to_fix_an_error[index]) || (error || qubit_to_fix.Z));
                        ++index;
                    }
                    BOOST_FOREACH(
                        qubit_type& qubit_to_fix,
                        make_iterator_range(logical_qubits_end_of_optimized,logical_qubits_end)
                    ) {
                        if(error && qubit_to_fix.Z) {
                            if(error && qubit_to_fix.X) {
                                qubit_fixing_the_error_ref->X *= qubit_to_fix.X;
                                qubit_fixing_the_error_ref->X *= qubit_to_fix.Z;
                                qubit_to_fix.Z *= qubit_fixing_the_error_ref->Z;
                                qubit_to_fix.X *= qubit_fixing_the_error_ref->Z;
                            } else {
                                qubit_fixing_the_error_ref->X *= qubit_to_fix.X;
                                qubit_to_fix.Z *= qubit_fixing_the_error_ref->Z;
                            }
                        } else if(error && qubit_to_fix.X) {
                            qubit_fixing_the_error_ref->X *= qubit_to_fix.Z;
                            qubit_to_fix.X *= qubit_fixing_the_error_ref->Z;
                        }
                        assert((error || qubit_to_fix.X) && (error || qubit_to_fix.Z));
                    }
                    //@-<< Case 1 >>
                    break;
                case 2:
                    //@+<< Case 2 >>
                    //@+node:gcross.20100208221030.1401: *5* << Case 2 >>
                    assert((error && qubit_fixing_the_error_ref->X) || (error && qubit_fixing_the_error_ref->Z));
                    if(error || qubit_fixing_the_error_ref->X)
                        swap(qubit_fixing_the_error_ref->X,qubit_fixing_the_error_ref->Z);
                    assert(error && qubit_fixing_the_error_ref->X);

                    iter_swap(qubit_fixing_the_error_ref,logical_qubits_end_of_optimized);
                    qubit_fixing_the_error_ref = logical_qubits_end_of_optimized;

                    ++number_of_optimized_logical_qubits;
                    ++logical_qubits_end_of_optimized;

                    BOOST_FOREACH(
                        qubit_type& qubit_to_fix,
                        make_iterator_range(logical_qubits_end_of_optimized,logical_qubits_end)
                    ) {
                        if(error && qubit_to_fix.Z) {
                            if(error && qubit_to_fix.X) {
                                qubit_fixing_the_error_ref->Z *= qubit_to_fix.X;
                                qubit_fixing_the_error_ref->Z *= qubit_to_fix.Z;
                                qubit_to_fix.Z *= qubit_fixing_the_error_ref->X;
                                qubit_to_fix.X *= qubit_fixing_the_error_ref->X;
                            } else {
                                qubit_fixing_the_error_ref->Z *= qubit_to_fix.X;
                                qubit_to_fix.Z *= qubit_fixing_the_error_ref->X;
                            }
                        } else if(error && qubit_to_fix.X) {
                            qubit_fixing_the_error_ref->Z *= qubit_to_fix.Z;
                            qubit_to_fix.X *= qubit_fixing_the_error_ref->X;
                        }
                        assert((error || qubit_to_fix.X) && (error || qubit_to_fix.Z));
                    }
                    if(error && qubit_fixing_the_error_ref->Z)
                        qubit_fixing_the_error_ref->Z *= qubit_fixing_the_error_ref->X;
                    assert((error && qubit_fixing_the_error_ref->X) && (error || qubit_fixing_the_error_ref->Z));

                    logical_qubit_error_distances.push_back(error.weight());
                    logical_qubit_errors.push_back(error);
                    //@-<< Case 2 >>
            }

        }

        BOOST_FOREACH(qubit_type& q, logical_qubits) {
            q.Y = q.Z*q.X;
        }

        optimized = true;
    }
    //@+node:gcross.20081203190837.3: *4* number_of_logical_qubits
    size_t number_of_logical_qubits() const {
        return (stabilizers.size() == 0 && gauge_qubits.size() == 0)
             ? 0
             : number_of_physical_qubits - stabilizers.size() - gauge_qubits.size();
    }
    //@+node:gmc.20080916172520.9: *4* recompute_logical_qubits
    void recompute_logical_qubits() {
        if(number_of_logical_qubits()==0) return;
        //@+<< Compute logical qubits >>
        //@+node:gcross.20090522205550.9: *5* << Compute logical qubits >>
        //@+at
        // Initialize the set of generators with the list of stabilizers and the X logical operators of the gauge qubits.
        //@@c
        const unsigned int number_of_generators = stabilizers.size() + gauge_qubits.size();
        if(number_of_generators==0) return;

        operator_vector generators(stabilizers);
        generators.reserve(number_of_generators);
        BOOST_FOREACH(const qubit_type& qubit, gauge_qubits) { generators.push_back(qubit.X); }

        //@+at
        // Run the Gaussian elimination.
        //@@c
        gaussian_elimination_state_type elimination_state = post_stabilizer_elimination_state;
        elimination_state.run_elimination(generators);

        //@+at
        // Loop though all of the qubit indices, looking for ones that have not been taken by a generator
        //@@c
        BOOST_FOREACH(const size_t current_qubit_index, irange((size_t)0u,number_of_physical_qubits)) {
        //@+at
        // If this qubit index has been taken by a generator, then skip to the next.
        //@@c
            if(elimination_state.indices_taken[current_qubit_index]) continue;
        //@+at
        // Okidoke, since this index has not been taken use it for our logical qubit.
        //@@c
            qubit_type logical_qubit(number_of_physical_qubits);
            logical_qubit.X.X.set(current_qubit_index);
            logical_qubit.Z.Z.set(current_qubit_index);
        //@+at
        // Now we need to multiply this qubit by factors that ensure it commutes with all of the generators.  The idea here is that if the generator anti-commutes with the local Pauli operator X or Z at current_qubit_index, then we need to multiply our logical qubit by an operator which we know anti-commutes with this generator and only this generator, and fortunately through the process of Gaussian elimination we know exactly what this operator is.
        //@@c
            size_t op_index = 0;
            typename index_vector::iterator chosen_qubit_index_iter = elimination_state.qubit_indices_chosen.begin();
            BOOST_FOREACH(const quantum_operator& op, generators) {
                if(elimination_state.paulis_chosen[op_index]) {
                    if(op.Z.test(current_qubit_index)) logical_qubit.X.Z.set(*chosen_qubit_index_iter);
                    if(op.X.test(current_qubit_index)) logical_qubit.Z.Z.set(*chosen_qubit_index_iter);
                } else {
                    if(op.Z.test(current_qubit_index)) logical_qubit.X.X.set(*chosen_qubit_index_iter);
                    if(op.X.test(current_qubit_index)) logical_qubit.Z.X.set(*chosen_qubit_index_iter);
                }
                ++chosen_qubit_index_iter;
                ++op_index;
            }
        //@+at
        // We now need to make sure that the logical qubit also commutes with all of the gauge qubit Z operators, since these were not present in our list of generators.
        //@@c
            BOOST_FOREACH(const qubit_type& q, gauge_qubits) {
                if(!(logical_qubit.X||q.Z)) logical_qubit.X *= q.X;
                if(!(logical_qubit.Z||q.Z)) logical_qubit.Z *= q.X;
            }
        //@+at
        // Finally, we are done.  Comptue the logical qubit Y operator for convenience, and then add the logical qubit to the list.
        //@@c
            logical_qubit.Y = logical_qubit.X * logical_qubit.Z;
            logical_qubits.push_back(logical_qubit);
        }
        //@+at
        // Assert that we found the correct number of logical qubits.
        //@@c
        assert(logical_qubits.size() == number_of_logical_qubits());
        //@-<< Compute logical qubits >>
    }
    //@-others
};

//@+<< I/O >>
//@+node:gcross.20101217153202.1447: *4* << I/O >>
template
    < class quantum_operator
    , class qubit_vector
    , class operator_vector
    , class index_vector
    , class pseudo_generator_vector
    >
ostream& operator<<(ostream& out, const qec<quantum_operator,qubit_vector,operator_vector,index_vector,pseudo_generator_vector>& code) {
    typedef typename qubit_vector::value_type qubit;

    out << "Stabilizers:" << endl;
    BOOST_FOREACH(const quantum_operator& op, code.stabilizers) {
        out << "    - " << op.to_string() << endl;
    }
    out << endl;

    out << "Gauge Qubits:" << endl;

    { int i = 0; BOOST_FOREACH(const qubit& qubit, code.gauge_qubits) {
        out << "# " << (i+1) << endl;
        out << "    - X: " << qubit.X.to_string() << endl;
        out << "      Y: " << qubit.Y.to_string() << endl;
        out << "      Z: " << qubit.Z.to_string() << endl;
        ++i;
    }}
    out << endl;

    out << "Logical Qubits:" << endl;
    { int i = 0; BOOST_FOREACH(const qubit& qubit, code.logical_qubits) {
        out << "# " << (i+1) << endl;
        out << "    - X: " << qubit.X.to_string() << endl;
        out << "      Y: " << qubit.Y.to_string() << endl;
        out << "      Z: " << qubit.Z.to_string() << endl;
        if(code.optimized) {
            out << "      Distance: " << code.logical_qubit_error_distances[i] << endl;
            out << "      Minimum weight error: " << code.logical_qubit_errors[i].to_string() << endl;
        }
        ++i;
    }}
    out << endl;

    out << "Summary:" << endl;
    out << "    Number of physical qubits: " << code.number_of_physical_qubits << endl;
    out << "    Number of stabilizers:     " << code.stabilizers.size() << endl;
    out << "    Number of gauge qubits:    " << code.gauge_qubits.size() << endl;
    out << "    Number of logical qubits:  " << code.logical_qubits.size() << endl;

    return out;
}
//@-<< I/O >>

//@+<< Aliases >>
//@+node:gcross.20101228150742.1589: *4* << Aliases >>
typedef qec<> dynamic_qec;

template<unsigned int nbits, unsigned int nops> struct wrapped_static_qec {
    typedef static_quantum_operator<nbits> quantum_operator;
    typedef qubit<static_quantum_operator<nbits> > qubit_t;
    typedef pseudo_generator<quantum_operator> pseudo_generator_t;
    typedef qec
        < quantum_operator
        , static_vector<qubit_t,nbits>
        , static_vector<quantum_operator,(nops < 2*nbits ? 2*nbits : nops)>
        , static_vector<size_t,nbits>
        , static_vector<pseudo_generator_t,nbits>
        > type;
};

template<unsigned int nbits, unsigned int nops> struct static_qec : public wrapped_static_qec<nbits,nops>::type {

    typedef typename wrapped_static_qec<nbits,nops>::type qec_t;

    static_qec(const typename qec_t::operator_vector& operators, const bool compute_logicals=true)
        : qec_t(operators,compute_logicals)
    { }

};
//@-<< Aliases >>
//@-others

template<class A, class B> class anti_commute_test {

public:

    anti_commute_test(const A& a_) : a(a_) { }

    bool operator()(const B& b) const { return not (a||b); }

protected:

    const A& a;

};

//@-others

}


#endif
//@-leo
