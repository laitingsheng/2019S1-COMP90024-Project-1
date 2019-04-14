#!/bin/bash
#SBATCH -c 4
#SBATCH -J COMP90024-n2c4
#SBATCH --ntasks-per-node=1
#SBATCH -N 2
#SBATCH --open-mode=truncate
#SBATCH -o SLURM/n2c4.summary
#SBATCH -p physical
#SBATCH -t 24:00:00

mkdir -p SLURM/n2c4

module load Boost/1.69.0-spartan_gcc-8.1.0 OpenMPI/3.1.0-GCC-8.2.0-cuda9-ucx

for i in {1..20}
do
    mpiexec -np 2 --map-by node:pe=4 bin/main_mn_mt /data/projects/COMP90024/bigTwitter.json /data/projects/COMP90024/melbGrid.json 1> SLURM/n2c4/$i.out 2> SLURM/n2c4/$i.err
done
