#!/bin/bash

# Load Boost 1.66 and OpenMPI 3 with GCC 8.2.0 for full support of C++17
module load Boost/1.66.0-GCC-8.2.0 OpenMPI/3.1.0-GCC-8.2.0-cuda9-ucx

# create directories for the compilation output
mkdir -p bin-common bin-st bin-mt bin-mn

# Common Object Files for All
g++ -std=c++17 -I./main/include -O3 -c -o bin-common/grid.o main/lib/grid.cpp
g++ -std=c++17 -I./main/include -O3 -c -o bin-common/processor.o main/lib/processors/processor.cpp

# Single Thread
g++ -std=c++17 -I./main/include -O3 -c -o bin-st/processor_st.o main/lib/processors/processor_st.cpp
g++ -std=c++17 -I./main/include -O3 -lboost_iostreams -lboost_timer -DSINGLE_THREAD -o bin-st/main main/main.cpp bin-common/grid.o bin-common/processor.o bin-st/processor_st.o

# Common Object File for Multi-Threading Relatives
g++ -std=c++17 -I./main/include -O3 -fopenmp -c -o bin-common/processor_m.o main/lib/processors/processor_m.cpp

# Multi Thread Single Node
g++ -std=c++17 -I./main/include -O3 -fopenmp -c -o bin-mt/processor_mt.o main/lib/processors/processor_mt.cpp
g++ -std=c++17 -I./main/include -O3 -fopenmp -lboost_iostreams -lboost_timer -o bin-mt/main main/main.cpp bin-common/grid.o bin-common/processor.o bin-common/processor_m.o bin-mt/processor_mt.o

# Multi Thread Multi Node
g++ -std=c++17 -I./main/include -O3 -fopenmp -c -o bin-mn/processor_mn.o main/lib/processors/processor_mn.cpp
mpic++ -std=c++17 -I./main/include -O3 -fopenmp -lboost_iostreams -lboost_mpi -lboost_serialization -lboost_timer -DMULTI_NODE -o bin-mn/main main/main.cpp bin-common/grid.o bin-common/processor.o bin-common/processor_m.o bin-mn/processor_mn.o
