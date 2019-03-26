CMAKE_MINIMUM_REQUIRED(VERSION 3.12)
PROJECT(Project_1)
FIND_PACKAGE(Boost 1.66.0 REQUIRED COMPONENTS container mpi unit_test_framework)

SET(CMAKE_CXX_STANDARD 17)

IF(APPLE)
    SET(CMAKE_CXX_FLAGS "-O3 -Xpreprocessor -fopenmp -Wall -Wextra -Wpedantic")
    INCLUDE_DIRECTORIES("/usr/local/opt/libomp/include")
    LINK_DIRECTORIES("/usr/local/opt/libomp/lib")
ELSE()
    SET(CMAKE_CXX_FLAGS "-O3 -fopenmp -Wall -Wextra -Wpedantic")
ENDIF()

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

ADD_EXECUTABLE(Project_1_Test src/project_test.cpp)

ADD_EXECUTABLE(Project_1 src/main.cpp)
TARGET_LINK_LIBRARIES(Project_1 boost_mpi)

IF(APPLE)
    TARGET_LINK_LIBRARIES(Project_1 omp)
ENDIF()
