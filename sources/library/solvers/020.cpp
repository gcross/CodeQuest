#include <boost/function.hpp>

#include "codequest.hpp"

namespace CodeQuest {

  using boost::function;

  typedef function<code (const dynamic_operator_vector&,bool)> solver;

  solver solver_020 = solveForFixedSize<dynamic_operator_vector,20>;
}

