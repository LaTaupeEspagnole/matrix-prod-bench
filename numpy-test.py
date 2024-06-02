#!/bin/python3

import numpy as np
import sys

command = sys.argv[1]

def getNext(control = ":"):
    s = ""
    c = sys.stdin.read(1)
    while c != control:
        s += c
        c = sys.stdin.read(1)
    return s

def getNextInt():
    return int(getNext())

def getNextFloat():
    return float(getNext())

def getNextMatrix(size1, size2, check_end=True):
    res = []
    for i in range(size1):
        line = []
        for j in range(size2):
            line.append(getNextFloat())
        res.append(line)

    if check_end and sys.stdin.read(1) != ";":
        print("Not a valid matrix (control)")
        exit(1)

    return res

def read_matrix(check_end=True):
    control = sys.stdin.read(1)
    if control != "M":
        print("Not a valid matrix (control)")
        exit(1)

    size1 = getNextInt()
    size2 = getNextInt()
    return (size1, size2, getNextMatrix(size1, size2, check_end))

def print_matrix(size1, size2, matrix):
    print("M" + str(size1) + ":" + str(size2), end=':')
    for l in matrix:
        for e in l:
            print("{:.6f}".format(e), end=':')

if command in ["mus", "mut", "mul", "mts", "mua"]:
    size1A, size2A, matA = read_matrix()
    size1B, size2B, matB = read_matrix()
    if size1A == 0 or size2A == 0 or size1B == 0 or size2B == 0:
        sys.stderr.write("Function error")
        exit(5)
    npMatA = np.array(matA)
    npMatB = np.array(matB)
    npRes = np.matmul(npMatA, npMatB)
    res = npRes.tolist()
    print_matrix(size1A, size2B, res)
elif command in ["sub", "sui"]:
    size1A, size2A, matA = read_matrix()
    size1B, size2B, matB = read_matrix()
    if size1A == 0 or size2A == 0 or size1B == 0 or size2B == 0:
        sys.stderr.write("Function error")
        exit(5)
    npMatA = np.array(matA)
    npMatB = np.array(matB)
    npRes = np.subtract(npMatA, npMatB)
    res = npRes.tolist()
    print_matrix(size1A, size2B, res)
elif command == "ieq":
    size1A, size2A, matA = read_matrix()
    size1B, size2B, matB = read_matrix()
    if size1A == 0 or size2A == 0 or size1B == 0 or size2B == 0:
        print("False")
        exit(0)
    npMatA = np.array(matA)
    npMatB = np.array(matB)
    if np.array_equal(npMatA, npMatB):
        print("True")
    else:
        print("False")
elif command == "ini":
    size1 = int(getNext(";"))
    size2 = int(getNext(";"))
    if size1 == 0 or size2 == 0:
        sys.stderr.write("Function error.")
        exit(5)

    res = []
    for i in range(size1):
        line = []
        for j in range(size2):
            line.append(0.0)
        res.append(line)
    print_matrix(size1, size2, res)
elif command == "dec":
    size1 = int(getNext(";"))
    size2 = int(getNext(";"))
    if size1 == 0 or size2 == 0:
        sys.stderr.write("Function error.")
        exit(5)
    print("M" + str(size1) + ":" + str(size2), end=':')
elif command == "cpy":
    size1A, size2A, matA = read_matrix(check_end=False)
    if size1A == 0 or size2A == 0:
        sys.stderr.write("Invalid input format")
        exit(3)
    npMatA = np.array(matA)
    res = npMatA.tolist()
    print_matrix(size1A, size2A, res)
    sys.stderr.write("skip")
elif command in ["add", "adi"]:
    size1A, size2A, matA = read_matrix()
    size1B, size2B, matB = read_matrix()
    if size1A == 0 or size2A == 0 or size1B == 0 or size2B == 0:
        sys.stderr.write("Function error")
        exit(5)
    npMatA = np.array(matA)
    npMatB = np.array(matB)
    npRes = np.add(npMatA, npMatB)
    res = npRes.tolist()
    print_matrix(size1A, size2A, res)

exit(0)
