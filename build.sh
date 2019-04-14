#!/bin/bash

module load Python/3.7.1-GCC-6.2.0 Boost/1.69.0-spartan_gcc-8.1.0 OpenMPI/3.1.0-GCC-8.2.0-cuda9-ucx

python build.py
