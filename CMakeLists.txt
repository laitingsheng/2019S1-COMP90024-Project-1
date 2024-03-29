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
ADD_LIBRARY(processor-sn-mt OBJECT main/lib/processors/processor_sn_mt.cpp)
ADD_LIBRARY(processor-mn-st OBJECT main/lib/processors/processor_mn_st.cpp)
ADD_LIBRARY(processor-mn-mt OBJECT main/lib/processors/processor_mn_mt.cpp)

# @formatter:off
ADD_EXECUTABLE(processors_test
    main/test/processors_test.cpp
    $<TARGET_OBJECTS:grid>
    $<TARGET_OBJECTS:processor>
    $<TARGET_OBJECTS:processor-sn-st>
    $<TARGET_OBJECTS:processor-sn-mt>
    $<TARGET_OBJECTS:processor-mn-st>
    $<TARGET_OBJECTS:processor-mn-mt>
)
# @formatter:on

IF (APPLE)
    TARGET_LINK_LIBRARIES(processors_test omp mpi boost_iostreams-mt boost_mpi-mt)
ELSE ()
    TARGET_LINK_LIBRARIES(processors_test pthread mpi_cxx mpi boost_iostreams boost_mpi boost_serialization)
ENDIF ()

OPTION(SNST "Single Node Single Thread" OFF)
OPTION(SNMT "Single Node Multi Thread" OFF)
OPTION(MNST "Multi Node Single Thread" OFF)
OPTION(MNMT "Multi Node Multi Thread" OFF)

IF (SNST)
    ADD_COMPILE_DEFINITIONS(SNST)
    # @formatter:off
    ADD_EXECUTABLE(main main/main.cpp
        $<TARGET_OBJECTS:grid>
        $<TARGET_OBJECTS:processor>
        $<TARGET_OBJECTS:processor-sn-st>
    )
    # @formatter:on
ELSEIF (SNMT)
    ADD_COMPILE_DEFINITIONS(SNMT)
    # @formatter:off
    ADD_EXECUTABLE(main main/main.cpp
        $<TARGET_OBJECTS:grid>
        $<TARGET_OBJECTS:processor>
        $<TARGET_OBJECTS:processor-sn-mt>
    )
    # @formatter:on
    if (APPLE)
        TARGET_LINK_LIBRARIES(main omp)
    ENDIF()
ELSEIF(MNST)
    ADD_COMPILE_DEFINITIONS(MNST)
    # @formatter:off
    ADD_EXECUTABLE(main main/main.cpp
        $<TARGET_OBJECTS:grid>
        $<TARGET_OBJECTS:processor>
        $<TARGET_OBJECTS:processor-mn-st>
    )
    # @formatter:on
    IF (APPLE)
        TARGET_LINK_LIBRARIES(main omp mpi boost_iostreams-mt boost_mpi-mt)
    ELSE ()
        TARGET_LINK_LIBRARIES(main pthread mpi_cxx mpi boost_iostreams boost_mpi boost_serialization)
    ENDIF ()
ELSEIF(MNMT)
    ADD_COMPILE_DEFINITIONS(MNMT)
    # @formatter:off
    ADD_EXECUTABLE(main main/main.cpp
        $<TARGET_OBJECTS:grid>
        $<TARGET_OBJECTS:processor>
        $<TARGET_OBJECTS:processor-mn-mt>
    )
    # @formatter:on
    IF (APPLE)
        TARGET_LINK_LIBRARIES(main omp mpi boost_iostreams-mt boost_mpi-mt)
    ELSE ()
        TARGET_LINK_LIBRARIES(main pthread mpi_cxx mpi boost_iostreams boost_mpi boost_serialization)
    ENDIF ()
ELSE()
    MESSAGE(FATAL_ERROR "Mode not supported")
ENDIF()

IF (APPLE)
    TARGET_LINK_LIBRARIES(main boost_iostreams-mt)
ELSE ()
    TARGET_LINK_LIBRARIES(main boost_iostreams boost_timer)
ENDIF ()
