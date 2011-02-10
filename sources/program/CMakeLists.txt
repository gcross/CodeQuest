#@+leo-ver=5-thin
#@+node:gcross.20101123222425.2062: * @thin CMakeLists.txt
add_executable(codequest
    codequest
    codequest-sparse
    codequest-dense-1d
)
set_property(TARGET codequest PROPERTY RUNTIME_OUTPUT_DIRECTORY ../../programs)
install (TARGETS codequest RUNTIME DESTINATION bin)
#@-leo
