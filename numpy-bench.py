#!/bin/python3

import numpy as np
import random
import sys
import time

start = time.process_time()

A = []
B = []

mat_size = int(sys.argv[1])

for i in range(0, mat_size):
    tmpA = []
    tmpB = []
    for k in range(0, mat_size):
        tmpA.append(random.randint(0, 10000) / 100)
        tmpB.append(random.randint(0, 10000) / 100)
    A.append(tmpA)
    B.append(tmpB)

matA = np.array(A, dtype = np.dtype(np.float32))
matB = np.array(B, dtype = np.dtype(np.float32))

end = time.process_time()
print("Random generation time : " + str(end - start))

start_process = time.process_time()
C = matA.dot(matB)
end_process = time.process_time()

print("Compute time : ", end='')
print(end_process - start_process)
