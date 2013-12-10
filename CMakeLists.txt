cmake_minimum_required( VERSION 2.6 ) 

project ( StarcraftSimulation )


# Compiler Flags
set( CMAKE_CXX_FLAGS "-std=c++0x -Wall" )


if(CMAKE_COMPILER_IS_GNUCXX)
    message(STATUS "GCC detected, adding compile flags")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion -Wshadow")
endif(CMAKE_COMPILER_IS_GNUCXX)


# Default build type
if ( NOT CMAKE_BUILD_TYPE )
    set ( CMAKE_BUILD_TYPE Debug CACHE STRING "Build Types: Debug Release"  FORCE )
endif()
set_property( CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS Debug Release )


add_subdirectory( src   )
#add_subdirectory( tests )

 
