#!/bin/python3

import os
from test_execution import *

def print_dir_test_result(space_before, name_rate, nb_success, nb_tests):
  if nb_tests == 0:
    return

  print("")
  print(space_before + "----- " + name_rate + " : " + str(nb_success) + "/" + str(nb_tests), end='')
  print(" (%.0f%%) -----" % ((nb_success / nb_tests) * 100))
  print("")

def exec_test_dir(test_dir_path, bin_path):
  tot_nb_success = 0
  tot_nb_tests = 0
  for dirname, _, file_name in os.walk(test_dir_path):
    file_name.sort()
    if len(file_name) == 0:
      return (0, 0)
    for test_file in file_name:
      test_file_path = os.path.join(dirname, test_file)
      (nb_success, nb_tests) = exec_test_file(bin_path, test_file_path)
      tot_nb_success += nb_success
      tot_nb_tests += nb_tests
  return (tot_nb_success, tot_nb_tests)

def exec_tests(test_dir_path, bin_path):
  tot_nb_success = 0
  tot_nb_tests = 0
  for test_dir_path, directorys, filenames in os.walk(test_dir_path):
    directorys.sort()
    for test_dir in directorys:
      print(" *** " + test_dir.upper() + " tests ***")
      (nb_success, nb_tests) = exec_test_dir(test_dir_path + test_dir, bin_path)
      tot_nb_success += nb_success
      tot_nb_tests += nb_tests
      print_dir_test_result(" ", "Success rate", nb_success, nb_tests)
    break
  return (tot_nb_success, tot_nb_tests)
