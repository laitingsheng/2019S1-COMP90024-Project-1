import re
import subprocess
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
{dependency}

mkdir -p SLURM/n{node}c{core}

module load {modules}

for i in {{1..{loop}}}
do
    {execute} /data/projects/COMP90024/bigTwitter.json /data/projects/COMP90024/melbGrid.json 1> SLURM/n{node}c{core}/$i.out 2> SLURM/n{node}c{core}/$i.err
done
'''

BOOST_MODULE = "Boost/1.69.0-spartan_gcc-8.1.0"
MPI_MODULE = "OpenMPI/3.1.0-GCC-8.2.0-cuda9-ucx"

CONFIGURES = (
    (1, 1), (1, 2), (1, 4), (1, 8), (1, 16), (1, 32), (1, 64),
    (2, 1), (2, 2), (2, 4), (2, 8), (2, 16), (2, 32),
    (4, 1), (4, 2), (4, 4), (4, 8), (4, 16),
    (8, 1), (8, 2), (8, 4), (8, 8),
)

def create_file(node, core, execute, modules, time="24:00:00", loop=20, dependency=""):
    with open(f"SLURM/n{node}c{core}.slurm", "w+t") as f:
        f.write(TEMPLATE.format(node=node, core=core, time=time, modules=modules, loop=loop, execute=execute, dependency=dependency))

JOB_ID_RGX = re.compile("Submitted batch job (\\d+)\n")

prev = ""
for node, core in CONFIGURES:
    create_file(
        node,
        core,
        f"{'' if node == 1 else f'mpiexec -np {node} --map-by node:pe={core} '}bin/main_{'sn' if node == 1 else 'mn'}_{'st' if core == 1 else 'mt'}",
        BOOST_MODULE if node == 1 else f"{BOOST_MODULE} {MPI_MODULE}",
        dependency=f"#SBATCH --dependency=afterany:{prev}" if prev else ""
    )
    r = subprocess.run(["sbatch", f"SLURM/n{node}c{core}.slurm"], capture_output=True, encoding="utf-8")
    prev = JOB_ID_RGX.match(r.stdout).group(1)
