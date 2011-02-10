#@+leo-ver=5-thin
#@+node:gcross.20101123222425.2061: * @thin CMakeLists.txt
cmake_minimum_required (VERSION 2.6)
project (CodeQuest CXX)

find_package (Boost 1.43 REQUIRED)
find_path (TCLAP_INCLUDE_DIR "tclap/CmdLine.h" ${CMAKE_INCLUDE_PATH})

if(TCLAP_INCLUDE_DIR)
    include_directories(${TCLAP_INCLUDE_DIR})
else()
    message(SEND_ERROR "You need to download and install the TCLAP library to use this code.")
endif()

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(SEND_ERROR "You need to download and install the Boost C++ library to use this code.")
endif()

if( NOT CMAKE_BUILD_TYPE )
    set( CMAKE_BUILD_TYPE "RelWithDebInfo" )
endif ()

message (STATUS "Build type is " ${CMAKE_BUILD_TYPE})

if(UNIX)
    if(CMAKE_COMPILER_IS_GNUCXX)
         set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -fomit-frame-pointer -fstrict-aliasing -momit-leaf-frame-pointer -fno-tree-pre -falign-loops -mtune=native -funroll-loops -funsafe-loop-optimizations -fipa-pta -ftree-loop-linear -ftree-loop-im -fivopts -fvariable-expansion-in-unroller -ftracer -ftree-vectorize ${CMAKE_CXX_FLAGS}")
         #add_definitions(-Wall -Wno-strict-aliasing)
    endif(CMAKE_COMPILER_IS_GNUCXX)
endif(UNIX)

message(STATUS "Flags:" ${CMAKE_CXX_FLAGS_RELEASE})

include_directories("includes")

add_subdirectory ("sources")
add_subdirectory ("documentation")
add_subdirectory ("tests")
#@-leo
