#!/usr/bin/env python
from mpi4py import MPI
import sys

comm = MPI.COMM_WORLD
rank = comm.Get_rank()

MESSAGES_AMOUNT = 10000


def ping_pong(ping_pong_message):
    comm.Barrier()
    start = MPI.Wtime()

    if rank == 0:
        comm.send(ping_pong_message, dest=1)
        comm.recv(source=1)
    elif rank == 1:
        data = comm.recv(source=0)
        comm.send(data, dest=0)

    stop = MPI.Wtime()

    return stop - start


bytes_amount = int(sys.argv[1])
message = "X" * bytes_amount
total_time = reduce(lambda t1, t2: t1 + t2, map(ping_pong(message), range(MESSAGES_AMOUNT)))

if rank == 0:
    result = total_time / MESSAGES_AMOUNT
    speed_in_mega_bits = (bytes_amount * 8 / result) / 1000000
    print "{bytes};{speed}".format(bytes=bytes_amount, speed=speed_in_mega_bits)
