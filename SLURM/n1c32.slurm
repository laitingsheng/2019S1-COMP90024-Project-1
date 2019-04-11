#!/bin/bash
#SBATCH -c 32
#SBATCH -J COMP90024-n1c32
#SBATCH --mail-type=ALL
#SBATCH --mail-user=tingshengl@student.unimelb.edu.au
#SBATCH --ntasks-per-node=1
#SBATCH -N 1
#SBATCH --open-mode=truncate
#SBATCH -o SLURM/n1c32.summary
#SBATCH -p physical
#SBATCH -t 12:00:00

mkdir -p SLURM/n1c32

module Boost/1.69.0-spartan_gcc-8.1.0

for i in {1..100}
do
    bin-mt/main /data/projects/COMP90024/bigTwitter.json /data/projects/COMP90024/melbGrid.json 1> SLURM/n1c32/$i.out 2> SLURM/n1c32/$i.err
done
