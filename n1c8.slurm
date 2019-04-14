#!/bin/bash
#SBATCH -c 8
#SBATCH -J COMP90024-n1c8
#SBATCH --ntasks-per-node=1
#SBATCH -N 1
#SBATCH --open-mode=truncate
#SBATCH -o n1c8.summary
#SBATCH -p physical
#SBATCH -t 24:00:00

module load Boost/1.69.0-spartan_gcc-8.1.0

bin/main_sn_mt /data/projects/COMP90024/bigTwitter.json /data/projects/COMP90024/melbGrid.json 1> n1c8.out 2> n1c8.err
