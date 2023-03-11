#!/usr/bin/env python
from mpi4py import MPI
import sys

comm = MPI.COMM_WORLD
rank = comm.Get_rank()

MESSAGES_AMOUNT = 100
bytes_amount = int(sys.argv[1])
message = "X" * bytes_amount


def ping_pong(_):
    comm.Barrier()
    start = MPI.Wtime()

    if rank == 0:
        comm.send(message, dest=1)
        comm.recv(source=1)
    elif rank == 1:
        data = comm.recv(source=0)
        comm.send(data, dest=0)

    stop = MPI.Wtime()

    return (stop - start) / 2


# total_time = sum(map(ping_pong, range(MESSAGES_AMOUNT)))
values = list(sorted(map(ping_pong, range(MESSAGES_AMOUNT))))
total_time = (values[49] + values[50]) / 2

if rank == 0:
    result = total_time / MESSAGES_AMOUNT
    speed_in_mega_bits = (bytes_amount * 8 / result) / 1000000
    print "{bytes};{speed}".format(bytes=bytes_amount, speed=speed_in_mega_bits)
