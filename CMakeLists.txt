CMAKE_MINIMUM_REQUIRED(VERSION 3.12)
PROJECT(Project_1)
FIND_PACKAGE(Boost 1.65.0 REQUIRED COMPONENTS container iostreams mpi serialization timer unit_test_framework)

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
ADD_LIBRARY(processor-st OBJECT main/lib/processors/processor_st.cpp)
ADD_LIBRARY(processor-m OBJECT main/lib/processors/processor_m.cpp)
ADD_LIBRARY(processor-mt OBJECT main/lib/processors/processor_mt.cpp)
ADD_LIBRARY(processor-mn OBJECT main/lib/processors/processor_mn.cpp)

# @formatter:off
ADD_EXECUTABLE(processors_test
    main/test/processors_test.cpp
    $<TARGET_OBJECTS:grid>
    $<TARGET_OBJECTS:processor>
    $<TARGET_OBJECTS:processor-st>
    $<TARGET_OBJECTS:processor-m>
    $<TARGET_OBJECTS:processor-mt>
    $<TARGET_OBJECTS:processor-mn>
)
# @formatter:on

IF (single_thread)
    ADD_COMPILE_DEFINITIONS(SINGLE_THREAD)
ELSEIF (multi_node)
    ADD_COMPILE_DEFINITIONS(MULTI_NODE)
ENDIF ()

IF (APPLE)
    TARGET_LINK_LIBRARIES(processors_test omp mpi boost_iostreams-mt boost_mpi-mt)
ELSE ()
    TARGET_LINK_LIBRARIES(processors_test pthread mpi_cxx mpi boost_iostreams boost_mpi)
ENDIF ()

IF (single_thread)
    # @formatter:off
    ADD_EXECUTABLE(main main/main.cpp
        $<TARGET_OBJECTS:grid>
        $<TARGET_OBJECTS:processor>
        $<TARGET_OBJECTS:processor-st>
    )
    # @formatter:on
ELSEIF (multi_node)
    # @formatter:off
    ADD_EXECUTABLE(main main/main.cpp
        $<TARGET_OBJECTS:grid>
        $<TARGET_OBJECTS:processor>
        $<TARGET_OBJECTS:processor-m>
        $<TARGET_OBJECTS:processor-mn>
    )
    # @formatter:on
    TARGET_LINK_LIBRARIES(main mpi)
    if (APPLE)
        TARGET_LINK_LIBRARIES(main boost_mpi-mt)
    ELSE()
        TARGET_LINK_LIBRARIES(main mpi_cxx boost_mpi boost_serialization)
    ENDIF()
ELSE()
    # @formatter:off
    ADD_EXECUTABLE(main main/main.cpp
        $<TARGET_OBJECTS:grid>
        $<TARGET_OBJECTS:processor>
        $<TARGET_OBJECTS:processor-m>
        $<TARGET_OBJECTS:processor-mt>
    )
    # @formatter:on
    if (APPLE)
        TARGET_LINK_LIBRARIES(main omp)
    ENDIF()
ENDIF ()

IF (APPLE)
    TARGET_LINK_LIBRARIES(main boost_iostreams-mt)
ELSE ()
    TARGET_LINK_LIBRARIES(main boost_iostreams boost_timer)
ENDIF ()
