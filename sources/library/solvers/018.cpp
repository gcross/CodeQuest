#include <boost/function.hpp>

#include "codequest.hpp"

namespace CodeQuest {

  using boost::function;

  typedef function<code (const dynamic_operator_vector&,bool)> solver;

  solver solver_018 = solveForFixedSize<dynamic_operator_vector,18>;
}
