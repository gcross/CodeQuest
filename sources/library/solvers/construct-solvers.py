template = """\
#include <boost/function.hpp>

#include "codequest.hpp"

namespace CodeQuest {

  using boost::function;

  typedef function<code (const dynamic_operator_vector&,bool)> solver;

  solver solver_%03i = solveForFixedSize<dynamic_operator_vector,%i>;
}
"""

for i in range(1,256+1):
    with open("%03i.cpp" % i,"w") as f:
        print >> f, template % (i,i)
