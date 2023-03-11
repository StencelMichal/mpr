#!/usr/bin/env python
import sys
from mpi4py import MPI
import random

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
size = comm.Get_size()

pointsAmount = int(sys.argv[1])
node_points_amount = pointsAmount // size


def random_point():
    x = random.random()
    y = random.random()
    return x, y


comm.Barrier()
start = MPI.Wtime()
inside = 0
for _ in range(node_points_amount):
    point = random_point()
    x, y = point
    if x ** 2 + y ** 2 < 1:
        inside += 1

total_inside = comm.reduce(inside, op=MPI.SUM, root=0)
if rank == 0:
    pi = 4 * (total_inside / float(pointsAmount))
    stop = MPI.Wtime()
    time = stop - start
    print  "{time};{points_total};{pi}".format(time=time, points_total=pointsAmount, pi=pi)
