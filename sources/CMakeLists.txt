#@+leo-ver=5-thin
#@+node:gcross.20101123222425.2062: * @thin sources/CMakeLists.txt
install (FILES codequest.hpp DESTINATION include)

include_directories(.)

add_executable(codequest
    codequest.cc
    codequest-sparse.cc
    codequest-dense-1d.cc
)
set_property(TARGET codequest PROPERTY RUNTIME_OUTPUT_DIRECTORY ../programs)
install (FILES ../programs/codequest DESTINATION bin)
#@-leo
