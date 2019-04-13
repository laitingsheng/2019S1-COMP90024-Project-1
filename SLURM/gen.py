from multiprocessing import cpu_count, Pool

TEMPLATE = '''#!/bin/bash
#SBATCH -c {core}
#SBATCH -J COMP90024-n{node}c{core}
#SBATCH --mail-type=ALL
#SBATCH --mail-user=tingshengl@student.unimelb.edu.au
#SBATCH --ntasks-per-node=1
#SBATCH -N {node}
#SBATCH --open-mode=truncate
#SBATCH -o SLURM/n{node}c{core}.summary
#SBATCH -p physical
#SBATCH -t {time}
#SBATCH --array=1-{loop}

mkdir -p SLURM/n{node}c{core}

module load {modules}

{execute} /data/projects/COMP90024/bigTwitter.json /data/projects/COMP90024/melbGrid.json 1> SLURM/n{node}c{core}/${{SLURM_ARRAY_TASK_ID}}.out 2> SLURM/n{node}c{core}/${{SLURM_ARRAY_TASK_ID}}.err
'''

BOOST_MODULE = "Boost/1.69.0-spartan_gcc-8.1.0"
MPI_MODULE = "OpenMPI/3.1.0-GCC-8.2.0-cuda9-ucx"

CONFIGURES = (
    (1, 1), (1, 2), (1, 4), (1, 8), (1, 16), (1, 32), (1, 64),
    (2, 1), (2, 2), (2, 4), (2, 8), (2, 16), (2, 32),
    (4, 1), (4, 2), (4, 4), (4, 8), (4, 16),
    (8, 1), (8, 2), (8, 4), (8, 8),
)

def create_file(node, core, execute, modules, time="5", loop=50):
    with open(f"n{node}c{core}.slurm", "w+t") as f:
        f.write(TEMPLATE.format(node=node, core=core, time=time, modules=modules, loop=loop, execute=execute))

with Pool(cpu_count()) as p:
    p.starmap(
        create_file,
        (
            (
                node,
                core,
                f"{'' if node == 1 else f'mpiexec -np {node} --map-by node:pe={core} '}bin/main_{'sn' if node == 1 else 'mn'}_{'st' if core == 1 else 'mt'}",
                BOOST_MODULE if node == 1 else f"{BOOST_MODULE} {MPI_MODULE}"
            )
            for node, core in CONFIGURES
        )
    )