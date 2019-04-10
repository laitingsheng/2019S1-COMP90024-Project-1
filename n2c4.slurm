#!/bin/bash
#SBATCH -c 4
#SBATCH -J COMP90024-n2c4
#SBATCH --mail-type=ALL
#SBATCH --mail-user=tingshengl@student.unimelb.edu.au
#SBATCH --ntasks-per-node=1
#SBATCH -N 2
#SBATCH --open-mode=truncate
#SBATCH -o SLURM/n2c4.summary
#SBATCH -p physical
#SBATCH -t 120

mkdir SLURM/n2c4

module load Boost/1.69.0-spartan_gcc-8.1.0 OpenMPI/3.1.0-GCC-8.2.0-cuda9-ucx

for i in {1..10}
do
    mpiexec -np 2 --cpus-per-proc 4 bin-mn/main /data/projects/COMP90024/bigTwitter.json /data/projects/COMP90024/melbGrid.json 1> SLURM/n2c4/$i.out 2> SLURM/n2c4/$i.err
done
