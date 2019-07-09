#!/bin/python3

import sys
import os
from json_parser import *
from test_execution import *
from test_dir_execution import *

if len(sys.argv) == 1:
  print("Usage: [tests_dir] [bin_path]")
  exit()

nb_success = 0
nb_tests = 0

print("")
print("  ******************************************************")
print("  ***** Martix operation processing lib test-suite *****")
print("  ******************************************************")
print("")

tests_dir = sys.argv[1]
bin_path = sys.argv[2]
if tests_dir[len(tests_dir) - 1] != "/":
  tests_dir += "/"
if bin_path[0] != "." and bin_path[1] != "/":
  bin_path = "./" + bin_path

(nb_success, nb_tests) = exec_tests(tests_dir, bin_path)
print_dir_test_result("", "Total success rate", nb_success, nb_tests)

