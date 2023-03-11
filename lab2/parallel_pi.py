import sys
from mpi4py import MPI

comm = MPI.COMM_WORLD
rank = comm.Get_rank()

processorsAmount = int(sys.argv[1])

for _ in range(processorsAmount):
    comm.Barrier()
    start = MPI.Wtime()

total_inside = comm.reduce(points_inside, op=MPI.SUM, root=0)
if rank == 0:


