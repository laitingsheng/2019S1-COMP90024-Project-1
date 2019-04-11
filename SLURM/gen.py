from multiprocessing import Pool

TEMPLATE = '''#!/bin/bash
#SBATCH -c {core}
#SBATCH -J COMP90024-n1c{core}
#SBATCH --mail-type=ALL
#SBATCH --mail-user=tingshengl@student.unimelb.edu.au
#SBATCH --ntasks-per-node=1
#SBATCH -N {node}
#SBATCH --open-mode=truncate
#SBATCH -o SLURM/n{node}c{core}.summary
#SBATCH -p physical
#SBATCH -t {time}

mkdir -p SLURM/n{node}c{core}

module {modules}

for i in {{1..{loop}}}
do
    {execute} /data/projects/COMP90024/bigTwitter.json /data/projects/COMP90024/melbGrid.json 1> SLURM/n{node}c{core}/$i.out 2> SLURM/n{node}c{core}/$i.err
done
'''

BOOST_MODULE = "Boost/1.69.0-spartan_gcc-8.1.0"
MPI_MODULE = "OpenMPI/3.1.0-GCC-8.2.0-cuda9-ucx"

CORES = (2, 4, 8, 16, 32)
NODES = (2, 4)

def create_file(node, core, execute, time="12:00:00", modules=BOOST_MODULE, loop=100):
    with open(f"n{node}c{core}.slurm", "w+t") as f:
        f.write(TEMPLATE.format(node=node, core=core, time=time, modules=modules, loop=loop, execute=execute))

create_file(1, 1, "bin-st/main")

with Pool(5) as p:
    p.starmap(create_file, ((1, core, "bin-mt/main") for core in CORES))
