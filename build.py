from os import system
from sys import platform, stderr

if platform.startswith("linux"):
    CXX = "g++"
    MPICXX = "mpic++"
else:
    print("platform not supported", file=stderr)
    exit(-1)

SRC = "main"
CXXFLAG = f"-std=c++17 -I./{SRC}/include -O3"

COMMAND = "mkdir -p bin"
print(COMMAND)
system(COMMAND)

# Common Object Files
COMMAND = f"{CXX} {CXXFLAG} -c -o bin/grid.o {SRC}/lib/grid.cpp"
print(COMMAND)
system(COMMAND)
COMMAND = f"{CXX} {CXXFLAG} -c -o bin/processor.o {SRC}/lib/processors/processor.cpp"
print(COMMAND)
system(COMMAND)

COMMON_OBJ = "bin/grid.o bin/processor.o"

BOOST = "-lboost_iostreams -lboost_timer"
BOOST_MPI = "-lboost_mpi -lboost_serialization"

# Single Node Single Thread
MACRO = "-DSNST"
SUFFIX = "sn_st"
COMMAND = f"{CXX} {CXXFLAG} -c -o bin/processor_{SUFFIX}.o {SRC}/lib/processors/processor_{SUFFIX}.cpp"
print(COMMAND)
system(COMMAND)
COMMAND = f"{CXX} {CXXFLAG} {BOOST} {MACRO} -o bin/main_{SUFFIX}.o {SRC}/main.cpp {COMMON_OBJ} bin/processor_{SUFFIX}.o"
print(COMMAND)
system(COMMAND)

# Single Node Multi Thread
MACRO = "-DSNMT"
SUFFIX = "sn_mt"
COMMAND = f"{CXX} {CXXFLAG} -c -o bin/processor_{SUFFIX}.o {SRC}/lib/processors/processor_{SUFFIX}.cpp"
print(COMMAND)
system(COMMAND)
COMMAND = f"{CXX} {CXXFLAG} {BOOST} {MACRO} -o bin/main_{SUFFIX}.o {SRC}/main.cpp {COMMON_OBJ} bin/processor_{SUFFIX}.o"
print(COMMAND)
system(COMMAND)

# Multi Node Single Thread
MACRO = "-DMNST"
SUFFIX = "mn_st"
COMMAND = f"{MPICXX} {CXXFLAG} -c -o bin/processor_{SUFFIX}.o {SRC}/lib/processors/processor_{SUFFIX}.cpp"
print(COMMAND)
system(COMMAND)
COMMAND = f"{MPICXX} {CXXFLAG} {BOOST} {BOOST_MPI} {MACRO} -o bin/main_{SUFFIX}.o {SRC}/main.cpp {COMMON_OBJ} bin/processor_{SUFFIX}.o"
print(COMMAND)
system(COMMAND)

# Multi Node Multi Thread
MACRO = "-DMNMT"
SUFFIX = "mn_mt"
COMMAND = f"{MPICXX} {CXXFLAG} -c -o bin/processor_{SUFFIX}.o {SRC}/lib/processors/processor_{SUFFIX}.cpp"
print(COMMAND)
system(COMMAND)
COMMAND = f"{MPICXX} {CXXFLAG} {BOOST} {BOOST_MPI} {MACRO} -o bin/main_{SUFFIX}.o {SRC}/main.cpp {COMMON_OBJ} bin/processor_{SUFFIX}.o"
print(COMMAND)
system(COMMAND)
