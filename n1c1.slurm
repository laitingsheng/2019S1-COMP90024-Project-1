#!/bin/bash
#SBATCH -c 1
#SBATCH -J COMP90024-st-n1c1
#SBATCH --mail-type=ALL
#SBATCH --mail-user=tingshengl@student.unimelb.edu.au
#SBATCH --ntasks-per-core=1
#SBATCH -N 1
#SBATCH --open-mode=truncate
#SBATCH -o /dev/null
#SBATCH -p physical
#SBATCH --profile=all
#SBATCH -t 120

module load Boost/1.69.0-spartan_gcc-8.1.0

bin-st/main /data/projects/COMP90024/bigTwitter.json /data/projects/COMP90024/melbGrid.json 1> SLURM/out/n1c1.out 2> SLURM/err/n1c1.err
