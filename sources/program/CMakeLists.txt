add_executable(codequest
    codequest
    codequest-sparse
    codequest-dense-1d
)
target_link_libraries(codequest CodeQuest)
set_property(TARGET codequest PROPERTY RUNTIME_OUTPUT_DIRECTORY ../../programs)
install (TARGETS codequest RUNTIME DESTINATION bin)
