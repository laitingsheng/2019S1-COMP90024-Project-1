from os import system

CONFIGURES = (
    (1, 1), (1, 2), (1, 4), (1, 8), (1, 16), (1, 32), (1, 64),
    (2, 1), (2, 2), (2, 4), (2, 8), (2, 16), (2, 32),
    (4, 1), (4, 2), (4, 4), (4, 8), (4, 16),
    (8, 1), (8, 2), (8, 4), (8, 8),
)

for node, core in CONFIGURES:
    system(f"sbatch SLURM/n{node}c{core}.slurm")
