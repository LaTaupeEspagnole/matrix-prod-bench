#!/bin/python3

import json

class TestObj(object):
  def __init__(self,
               test_type,
               stdin,
               command,
               expected_stdout,
               expected_stderr,
               expected_exitstatus,
               exact_stdout,
               exact_stderr,
               test_description,
               test_name,
               test_timeout):
    self.test_type =            test_type
    self.stdin =                stdin
    self.command =              command
    self.expected_stdout =      expected_stdout
    self.expected_stderr =      expected_stderr
    self.expected_exitstatus =  expected_exitstatus
    self.exact_stdout =         exact_stdout
    self.exact_stderr =         exact_stderr
    self.test_description =     test_description
    self.test_name =            test_name
    self.test_timeout =         test_timeout

def load_test_file(file_name):
  test_file = open(file_name, 'r')
  return json.load(test_file)
