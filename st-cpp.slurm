#!/bin/bash
#SBATCH -p physical
#SBATCH -N 1
#SBATCH -c 1
#SBATCH -o st-cpp.slurm.out

mkdir -p bin-st &> /dev/null

module load Boost/1.66.0-GCC-8.2.0 OpenMPI/3.1.0-GCC-8.2.0-cuda9-ucx &> /dev/null

g++ -std=c++17 -I./main/include -O3 -c -o bin-st/grid.o main/lib/grid.cpp &> /dev/null
g++ -std=c++17 -I./main/include -O3 -c -o bin-st/processor.o main/lib/processors/processor.cpp &> /dev/null
g++ -std=c++17 -I./main/include -O3 -c -o bin-st/processor_st.o main/lib/processors/processor_st.cpp &> /dev/null
g++ -std=c++17 -I./main/include -lboost_timer -lboost_iostreams -DSINGLE_THREAD -O3 -o bin-st/main main/main.cpp bin-st/grid.o bin-st/processor.o bin-st/processor_st.o &> /dev/null

bin-st/main ../COMP90024/bigTwitter.json ../COMP90024/melbGrid.json
