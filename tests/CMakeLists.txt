#@+leo-ver=5-thin
#@+node:gcross.20101123222425.2063: * @thin tests/CMakeLists.txt
add_executable(test EXCLUDE_FROM_ALL
    randomized-tests.cc
)
set_property(TARGET test PROPERTY RUNTIME_OUTPUT_DIRECTORY ..)
#@-leo
