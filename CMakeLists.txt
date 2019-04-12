CMAKE_MINIMUM_REQUIRED(VERSION 3.12)
PROJECT(Project_1)
FIND_PACKAGE(Boost 1.67 REQUIRED COMPONENTS iostreams mpi serialization timer unit_test_framework)

SET(CMAKE_CXX_STANDARD 17)

IF (APPLE)
    SET(CMAKE_CXX_FLAGS "-O3 -Xpreprocessor -fopenmp -Wall -Wextra -Wpedantic")
    INCLUDE_DIRECTORIES(/usr/local/opt/llvm/include /usr/local/opt/openmpi/include)
    LINK_DIRECTORIES(/usr/local/opt/llvm/lib /usr/local/opt/libevent/lib /usr/local/opt/openmpi/lib)
ELSE ()
    SET(CMAKE_CXX_FLAGS "-O3 -fopenmp -Wall -Wextra -Wpedantic")
    INCLUDE_DIRECTORIES(/usr/lib/x86_64-linux-gnu/openmpi/include/openmpi /usr/lib/x86_64-linux-gnu/openmpi/include)
    LINK_DIRECTORIES(/usr/lib/x86_64-linux-gnu/openmpi/lib)
ENDIF ()

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS} main/include)
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

ADD_LIBRARY(grid OBJECT main/lib/grid.cpp)
ADD_EXECUTABLE(grid_test main/test/grid_test.cpp $<TARGET_OBJECTS:grid>)

ADD_LIBRARY(processor OBJECT main/lib/processors/processor.cpp)
ADD_LIBRARY(processor-sn-st OBJECT main/lib/processors/processor_sn_st.cpp)

# @formatter:off
ADD_EXECUTABLE(processors_test
    main/test/processors_test.cpp
    $<TARGET_OBJECTS:grid>
    $<TARGET_OBJECTS:processor>
    $<TARGET_OBJECTS:processor-sn-st>
)
# @formatter:on

IF (APPLE)
    TARGET_LINK_LIBRARIES(processors_test omp mpi boost_iostreams-mt boost_mpi-mt)
ELSE ()
    TARGET_LINK_LIBRARIES(processors_test pthread mpi_cxx mpi boost_iostreams boost_mpi)
ENDIF ()

