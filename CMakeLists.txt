CMAKE_MINIMUM_REQUIRED(VERSION 3.12)
PROJECT(Project_1)
FIND_PACKAGE(Boost 1.66.0 REQUIRED COMPONENTS container iostreams mpi unit_test_framework)

SET(CMAKE_CXX_STANDARD 17)

IF (APPLE)
    SET(CMAKE_CXX_FLAGS "-O3 -Xpreprocessor -fopenmp -Wall -Wextra -Wpedantic")
    INCLUDE_DIRECTORIES(/usr/local/opt/llvm/include /usr/local/opt/openmpi/include)
    LINK_DIRECTORIES(/usr/local/opt/llvm/lib /usr/local/opt/libevent/lib /usr/local/opt/openmpi/lib)
ELSE ()
    SET(CMAKE_CXX_FLAGS "-O3 -fopenmp -Wall -Wextra -Wpedantic")
ENDIF ()

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

ADD_LIBRARY(grid OBJECT main/src/grid.cpp)
ADD_EXECUTABLE(grid_test main/test/grid_test.cpp $<TARGET_OBJECTS:grid>)

# @formatter:off
ADD_LIBRARY(processors OBJECT main/src/processors/processor.cpp
        main/src/processors/single_thread_processor.cpp
        main/src/processors/multi_thread_processor.cpp
        main/src/processors/multi_node_processor.cpp
)
# @formatter:on
ADD_EXECUTABLE(processors_test main/test/processors_test.cpp $<TARGET_OBJECTS:grid> $<TARGET_OBJECTS:processors>)

IF(APPLE)
#    TARGET_LINK_LIBRARIES(Project_1 omp mpi boost_iostreams-mt boost_mpi-mt)
    TARGET_LINK_LIBRARIES(processors_test omp mpi boost_iostreams-mt boost_mpi-mt)
ELSE()
#    TARGET_LINK_LIBRARIES(Project_1 boost_iostreams boost_mpi)
    TARGET_LINK_LIBRARIES(processors_test boost_iostreams boost_mpi)
ENDIF()
