#!/bin/python3

from subprocess import Popen, PIPE, TimeoutExpired
from json_parser import *

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

def print_stdin(string):
  i = 0;
  l = 1;
  print('        ' + str(l) + ' ', end='')
  while i < len(string):
    if string[i] == '\\':
      i += 1
      if string[i] == 'n':
        l += 1
        print('\n       ', end='')
        if l < 10:
          print(' ', end='')
        print(str(l) + ' ', end='')
      else:
        print(string[i], end='')
    else:
      print(string[i], end='')
    i += 1
  print()

def show_status(stat):
  if stat:
    print("\x1b[32m[OK]\x1b[0m", end='')
  else:
    print("\x1b[31m[KO]\x1b[0m", end='')

def pretty_print_test_res(stat_stdout, stat_stderr, stat_exit, test_name):
  print(" stdout ", end='')
  show_status(stat_stdout)
  print(" stderr ", end='')
  show_status(stat_stderr)
  print(" exit ", end='')
  show_status(stat_exit)
  print(" - " + test_name)

def show_diff(cmd_input, out, err, exit_code, cmd_line, test_obj):
  max_output_size = 2000
  cmd_input = str(cmd_input)
  cmd_input = cmd_input[2:len(cmd_input) - 1]
  print("   Error in test : " + test_obj['test_name'])
  print("     Description     : " + test_obj['test_description'])
  print("     Commande line   : " + str(cmd_line))
  if cmd_input != None:
    if len(cmd_input) > max_output_size:
      print("     stdin           : \"" + cmd_input[0:100] + " ... " + cmd_input[-100:] + "\"")
    else:
      print("     stdin           : \"" + cmd_input + "\"")
  if len(out) > max_output_size:
    print("     stdout result   : \"" + out[0:100] + " ... " + out[-100:] + "\"")
  else:
    print("     stdout result   : \"" + out + "\"")
  if len(test_obj['expected_stdout']) > max_output_size:
    print("     stdout expected : \"" + test_obj['expected_stdout'][0:100] + " ... " + test_obj['expected_stdout'][-100:], end='')
  else:
    print("     stdout expected : \"" + test_obj['expected_stdout'], end='')
  if test_obj['exact_stdout'] == "true":
    print("\" (exact output)")
  else:
    print("\" (searched in output)")
  print("     stderr result   : \"" + str(err) + "\"")
  print("     stderr expected : \"" + test_obj['expected_stderr'], end='')
  if test_obj['exact_stderr'] == "true":
    print("\" (exact output)")
  else:
    print("\" (searched in output)")
  print("     exit result     : " + str(exit_code))
  print("     exit expected   : " + test_obj['expected_exitstatus'])

def check_stream(result, expected, exact):
  if expected != "skip":
    if exact == "true":
      return result == expected
    else:
      return expected in result
  else:
    return 1

def check_exit(result, expected):
  return result == expected

def check_outputs(out, err, exit_code, test_obj):
  stat_stdout = check_stream(out, test_obj['expected_stdout'], test_obj['exact_stdout'])
  stat_stderr = check_stream(err, test_obj['expected_stderr'], test_obj['exact_stderr'])
  stat_exit = check_exit(exit_code, int(test_obj['expected_exitstatus']))
  return (stat_stdout, stat_stderr, stat_exit)

def make_test(cmd, test_obj, cmd_input):
  (cmd_input, out, err, exit_code, timeout_flag) = \
    exec_commande_line(cmd, int(test_obj['test_timeout']), cmd_input)
  if timeout_flag:
    return 1

  (stat_stdout, stat_stderr, stat_exit) = check_outputs(out, err, exit_code, test_obj)
  pretty_print_test_res(stat_stdout, stat_stderr, stat_exit, test_obj['test_name'])
  if (not stat_stdout or not stat_stderr or not stat_exit):
    show_diff(cmd_input, out, err, exit_code, cmd, test_obj)
    return (0, 1)
  return (1, 1)

def exec_simple_tests_type(bin_path, test_obj):
  cmd = [bin_path, test_obj['command']]
  return make_test(cmd, test_obj, bytes(test_obj['stdin'], 'utf-8'))

def exec_test_file(bin_path, test_file_path):
  test_obj = load_test_file(test_file_path)
  (nb_success, nb_tests) = (0, 0)

  if test_obj['test_type'] == "simple":
    (nb_success, nb_tests) = exec_simple_tests_type(bin_path, test_obj)
  else:
    print("[TEST-SUITE] ERROR: Not a valid test_type.")

  return (nb_success, nb_tests)
