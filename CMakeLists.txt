#@+leo-ver=5-thin
#@+node:gcross.20101123222425.2061: * @thin CMakeLists.txt
cmake_minimum_required (VERSION 2.6)
project (CodeQuest)

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

add_subdirectory ("sources")
add_subdirectory ("documentation")
add_subdirectory ("tests")
#@-leo
