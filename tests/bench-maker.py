#!/bin/python3

from subprocess import Popen, PIPE, TimeoutExpired
import sys
from math import inf

def exec_commande_line(cmd_line, cmd_timeout, cmd_input):
    prosses = Popen(cmd_line, stdin=PIPE, stdout=PIPE, stderr=PIPE)
    (out, err) = ("", "")
    timeout_flag = 0

    if cmd_timeout == 0:
        cmd_timeout = None

    try:
        (out, err) = prosses.communicate(input=cmd_input, timeout=cmd_timeout)
    except TimeoutExpired:
        prosses.kill()
        (out, err) = prosses.communicate()
        timeout_flag = 1

    exit_code = prosses.wait()
    out = out.decode()
    err = err.decode()
    exit_code = int(exit_code)
    return (cmd_input, out, err, exit_code, timeout_flag)

if len(sys.argv) != 3:
    print("Usage: [size_mat] [nb_iteration]")
    exit()

mat_size = int(sys.argv[1])
nb_iteration = int(sys.argv[2])

mult_naive_cmd = ["./dev", "mul", str(mat_size)]
mult_simd_cmd = ["./dev", "mul_simd", str(mat_size)]
mult_th_cmd = ["./dev", "mul_th", str(mat_size)]
mult_th_simd_cmd = ["./dev", "mul_th_simd", str(mat_size)]

prefix_cmd = ["/bin/time", "-f", "%e"]
sufix_cmd = [""]

mult_naive_cmd_out = []
mult_simd_cmd_out = []
mult_th_cmd_out = []
mult_th_simd_cmd_out = []

for i in range(0, nb_iteration):
    cmd = prefix_cmd + mult_naive_cmd
    (cmd_input, out, err, exit_code, _) = exec_commande_line(cmd, 0, "")
    mult_naive_cmd_out.append(float(err.split(" ")[0].strip("\n")))
    #print("[" + str(i) + "] " + err.split(" ")[0].strip("\n"))

for i in range(0, nb_iteration):
    cmd = prefix_cmd + mult_simd_cmd
    (cmd_input, out, err, exit_code, _) = exec_commande_line(cmd, 0, "")
    mult_simd_cmd_out.append(float(err.split(" ")[0].strip("\n")))
    #print("[" + str(i) + "] " + err.split(" ")[0].strip("\n"))

for i in range(0, nb_iteration):
    cmd = prefix_cmd + mult_th_cmd
    (cmd_input, out, err, exit_code, _) = exec_commande_line(cmd, 0, "")
    mult_th_cmd_out.append(float(err.split(" ")[0].strip("\n")))
    #print("[" + str(i) + "] " + err.split(" ")[0].strip("\n"))

for i in range(0, nb_iteration):
    cmd = prefix_cmd + mult_th_simd_cmd
    (cmd_input, out, err, exit_code, _) = exec_commande_line(cmd, 0, "")
    mult_th_simd_cmd_out.append(float(err.split(" ")[0].strip("\n")))
    #print("[" + str(i) + "] " + err.split(" ")[0].strip("\n"))

render = "Naive,SIMD,Threaded,ThreadedSIMD\n"
for i in range(0, nb_iteration):
    render += str(mult_naive_cmd_out[i]) + ","
    render += str(mult_simd_cmd_out[i]) + ","
    render += str(mult_th_cmd_out[i]) + ","
    render += str(mult_th_simd_cmd_out[i]) + "\n"

with open("bench.out", "w") as f:
    f.write(render)

naive_res = [0.0, inf, 0.0]
simd_res = [0.0, inf, 0.0]
th_res = [0.0, inf, 0.0]
th_simd_res = [0.0, inf, 0.0]

for i in range(0, nb_iteration):
    naive_res[0] += mult_naive_cmd_out[i]
    simd_res[0] += mult_simd_cmd_out[i]
    th_res[0] += mult_th_cmd_out[i]
    th_simd_res[0] += mult_th_simd_cmd_out[i]
    if mult_naive_cmd_out[i] <= naive_res[1]:
        naive_res[1] = mult_naive_cmd_out[i]
    else:
        naive_res[2] = mult_naive_cmd_out[i]
    if mult_simd_cmd_out[i] <= simd_res[1]:
        simd_res[1] = mult_simd_cmd_out[i]
    else:
        simd_res[2] = mult_simd_cmd_out[i]
    if mult_th_cmd_out[i] <= th_res[1]:
        th_res[1] = mult_th_cmd_out[i]
    else:
        th_res[2] = mult_th_cmd_out[i]
    if mult_th_simd_cmd_out[i] <= th_simd_res[1]:
        th_simd_res[1] = mult_th_simd_cmd_out[i]
    else:
        th_simd_res[2] = mult_th_simd_cmd_out[i]

naive_res[0] = naive_res[0] / nb_iteration
simd_res[0] = simd_res[0] / nb_iteration
th_res[0] = th_res[0] / nb_iteration
th_simd_res[0] = th_simd_res[0] / nb_iteration

simd_better = (naive_res[0] / simd_res[0]) * 100
th_simd_better = (th_res[0] / th_simd_res[0]) * 100
overall_better = (naive_res[0] / th_simd_res[0]) * 100

print("Summary :")
print("Test           |  Avg   |  Min   |  Max")
print("---------------+--------|--------|--------")
print("Naive          | %6.3f | %6.3f | %6.3f" % (naive_res[0], naive_res[1], naive_res[2]))
print("SIMD           | %6.3f | %6.3f | %6.3f  (%3.0f%% faster than naive)" % (simd_res[0], simd_res[1], simd_res[2], simd_better))
print("Threated Naive | %6.3f | %6.3f | %6.3f" % (th_res[0], th_res[1], th_res[2]))
print("Threaded SIMD  | %6.3f | %6.3f | %6.3f  (%3.0f%% faster than threaded, %3.0f%% faster than naive)" % (th_simd_res[0], th_simd_res[1], th_simd_res[2], th_simd_better, overall_better))





