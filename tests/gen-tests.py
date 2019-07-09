#!/bin/python3

import random

def getMat(mat, mat_size_col, line, col):
    return mat[mat_size_col * line + col]

def setMat(mat, mat_size_col, line, col, val):
    mat[mat_size_col * line + col] = val

def printMat(mat, mat_size_line, mat_size_col):
    for l in range(0, mat_size_col):
        for c in range(0, mat_size_line):
            print(getMat(mat, mat_size_col, l, c), end=' ')
        print("\n", end='')

def serializeMat(mat, mat_size_line, mat_size_col):
    res = "M" + str(mat_size_line) + ":" + str(mat_size_col) + ":"
    for l in range(0, mat_size_col):
        for c in range(0, mat_size_line):
            res += str(getMat(mat, mat_size_col, l, c)) + ".000000" + ":"
    res += ";"
    return res

print(" Test generator ", end='\n\n')

test_type = input("Test type (simple) : ")
mat_size_line = input("Matrix size line : ")
mat_size_col = input("Matrix size column : ")
commande = input("Test-suite commande (see ./test help) : ")
stderr = input("Stderr expected : ")
exit_status = input("Exit status : ")
exact_stdout = input("Exact stdout ? (true/false) : ")
exact_stderr = input("Exact stderr ? (true/false) : ")
test_name = input("Test name : ")
test_description = input("Test description : ")
test_timeout = input("Test timeout (0 for none) : ")
test_filename = input("Test filename (*.json) : ")

matA = []
matB = []
matR = []
mat_size = int(mat_size_line) * int(mat_size_col)

for i in range(0, mat_size):
    matA.append(random.randint(0, 100))
    matB.append(random.randint(0, 100))
    matR.append(0)

for l in range(0, int(mat_size_col)):
    for c in range(0, int(mat_size_line)):
        res = 0
        for i in range(0, int(mat_size_col)):
            res += getMat(matA, int(mat_size_col), l, i) * getMat(matB, int(mat_size_col), i, c)
        setMat(matR, int(mat_size_col), l, c, res)

serializedMatA = serializeMat(matA, int(mat_size_line), int(mat_size_col))
serializedMatB = serializeMat(matB, int(mat_size_line), int(mat_size_col))
serializedMatR = serializeMat(matR, int(mat_size_line), int(mat_size_col))[:-1]

stdin = serializedMatA + serializedMatB
stdout = serializedMatR

test_json = "\
{\n\
  \"test_type\": \"" + test_type + "\",\n\
  \"stdin\": \"" + stdin + "\",\n\
  \"command\": \"" + commande + "\",\n\
  \"expected_stdout\": \"" + stdout + "\",\n\
  \"expected_stderr\": \"" + stderr + "\",\n\
  \"expected_exitstatus\": \"" + exit_status + "\",\n\
  \"exact_stdout\": \"" + exact_stdout + "\",\n\
  \"exact_stderr\": \"" + exact_stderr + "\",\n\
  \"test_name\": \"" + test_name + "\",\n\
  \"test_description\": \"" + test_description + "\",\n\
  \"test_timeout\": \"" + test_timeout + "\"\n\
}"

with open(test_filename, "w") as f:
    f.write(test_json)

print()
print(test_json)
