#!/bin/bash
#SBATCH -p physical
#SBATCH -N 1
#SBATCH -c 8
#SBATCH -o /dev/null

module load Boost/1.69.0-spartan_gcc-8.1.0

bin-mt/main /data/projects/COMP90024/bigTwitter.json /data/projects/COMP90024/melbGrid.json 1> n1c8.out 2> n1c8.err
