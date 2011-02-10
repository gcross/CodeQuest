#@+leo-ver=5-thin
#@+node:gcross.20101123222425.2063: * @thin CMakeLists.txt
include_directories(includes)

add_library(testlib EXCLUDE_FROM_ALL
    testlib
    testlib-minimizer
    testlib-minimizer-dynamic
    testlib-minimizer-static
    testlib-minimizer-static-4
    testlib-minimizer-static-8
    testlib-minimizer-static-64
    testlib-solver
    testlib-solver-adaptive
    testlib-solver-dynamic
    testlib-solver-static
    testlib-solver-static-4
    testlib-solver-static-8
    testlib-solver-static-16
    testlib-solver-static-32
    testlib-solver-static-64
)

macro(makeTestProgram program-name)
    add_executable(${program-name} EXCLUDE_FROM_ALL ${program-name}.cpp)
    target_link_libraries(${program-name} testlib CodeQuest ${Boost_LIBRARIES})
    set_property(TARGET ${program-name} PROPERTY RUNTIME_OUTPUT_DIRECTORY .)
    add_dependencies(tests ${program-name})
endmacro(makeTestProgram)

add_custom_target(tests)
makeTestProgram(test)
makeTestProgram(test-minimizer)
makeTestProgram(test-minimizer-dynamic)
makeTestProgram(test-minimizer-static)
makeTestProgram(test-solver)
makeTestProgram(test-solver-dynamic)
makeTestProgram(test-solver-static)
#@-leo
