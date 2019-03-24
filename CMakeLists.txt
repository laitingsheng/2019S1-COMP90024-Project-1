CMAKE_MINIMUM_REQUIRED(VERSION 3.12)
PROJECT(Project_1)
FIND_PACKAGE(Boost 1.66.0 REQUIRED COMPONENTS container mpi)

SET(CMAKE_CXX_STANDARD 17)

IF(APPLE)
    SET(CMAKE_CXX_FLAGS "-O3 -Xpreprocessor -fopenmp -Wall -Wextra -Wpedantic")
    INCLUDE_DIRECTORIES("/usr/local/opt/libomp/include")
    LINK_DIRECTORIES("/usr/local/opt/libomp/lib")
ELSEIF(MSVC)
    SET(CMAKE_CXX_FLAGS "-Ox -Wall -EHscr -favor:AMD64 -arch:AVX2 -fp:except -openmp -GL -guard:cf -Qpar -Qvec-report:2 -Qpar-report:2 -MP -sdl -Zc:wchar_t -Zc:auto -Zc:implicitNoexcept")
    INCLUDE_DIRECTORIES("C:\\Program Files\ (x86)\\Microsoft SDKs\\MPI\\Include")
    LINK_DIRECTORIES("C:\\Program Files\ (x86)\\Microsoft SDKs\\MPI\\Lib\\x64")
ELSE()
    SET(CMAKE_CXX_FLAGS "-O3 -fopenmp -Wall -Wextra -Wpedantic")
ENDIF()

INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})

ADD_EXECUTABLE(Project_1 main.cpp)
TARGET_LINK_LIBRARIES(Project_1 stdc++fs boost_mpi)

IF(APPLE)
    TARGET_LINK_LIBRARIES(Project_1 omp)
ENDIF()
