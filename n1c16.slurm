#!/bin/bash
#SBATCH -c 16
#SBATCH -J COMP90024-n1c16
#SBATCH --mail-type=ALL
#SBATCH --mail-user=tingshengl@student.unimelb.edu.au
#SBATCH --ntasks-per-node=1
#SBATCH -N 1
#SBATCH --open-mode=truncate
#SBATCH -o SLURM/n1c16.summary
#SBATCH -p physical
#SBATCH -t 120

mkdir SLURM/n1c16

module load Boost/1.69.0-spartan_gcc-8.1.0

for i in {1..10}
do
    bin-mt/main /data/projects/COMP90024/bigTwitter.json /data/projects/COMP90024/melbGrid.json 1> SLURM/n1c16/$i.out 2> SLURM/n1c16/$i.err
done
