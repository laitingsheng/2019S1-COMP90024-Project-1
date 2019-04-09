#!/bin/bash
#SBATCH -p physical
#SBATCH -N 2
#SBATCH -c 4
#SBATCH -o /dev/null

module load Boost/1.69.0-spartan_gcc-8.1.0 OpenMPI/3.1.0-GCC-8.2.0-cuda9-ucx

mpiexec -np 2 --cpus-per-proc 4 bin-mn/main /data/projects/COMP90024/bigTwitter.json /data/projects/COMP90024/melbGrid.json 1> n2c8.out 2> n2c8.err
