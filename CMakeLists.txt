CMAKE_MINIMUM_REQUIRED(VERSION 3.12)
PROJECT(Project_1)
FIND_PACKAGE(Boost 1.66.0 REQUIRED COMPONENTS container iostreams mpi unit_test_framework)

SET(CMAKE_CXX_STANDARD 17)

IF(APPLE)
    SET(CMAKE_CXX_FLAGS "-O3 -Xpreprocessor -fopenmp -Wall -Wextra -Wpedantic")
    INCLUDE_DIRECTORIES(/usr/local/opt/llvm/include /usr/local/opt/openmpi/include)
    LINK_DIRECTORIES(/usr/local/opt/llvm/lib /usr/local/opt/libevent/lib /usr/local/opt/openmpi/lib)
ELSE()
    SET(CMAKE_CXX_FLAGS "-O3 -fopenmp -Wall -Wextra -Wpedantic")
ENDIF()

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
LINK_DIRECTORIES(${Boost_LIBRARY_DIRS})

ADD_EXECUTABLE(grid_test src/grid_test.cpp)
ADD_EXECUTABLE(processor_test src/processors_test.cpp)
ADD_EXECUTABLE(Project_1 src/main.cpp)

IF(APPLE)
    TARGET_LINK_LIBRARIES(Project_1 omp mpi boost_iostreams-mt boost_mpi-mt)
    TARGET_LINK_LIBRARIES(processor_test omp mpi boost_iostreams-mt boost_mpi-mt)
ELSE()
    TARGET_LINK_LIBRARIES(Project_1 boost_iostreams boost_mpi)
    TARGET_LINK_LIBRARIES(processor_test boost_iostreams boost_mpi)
ENDIF()
