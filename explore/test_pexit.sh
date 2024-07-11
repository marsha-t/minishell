#!/bin/bash

echo "Starting main shell"

# Launching subshell
(
  echo "Inside subshell"
  
  # Running command in subshell
  # ls some_nonexistent_directory
  
  # Storing exit status of the subshell
  exit_status=$?
  
  # Printing exit status
  echo "Exit status in subshell: $exit_status"
  
  # Exiting subshell
  exit 42
  # exit -2
  # exit "" "2" ""
  # exit 10 -
  # exit - 10
  # exit 0000000
  # exit 9223372036854775807 
  # exit 9223372036854775808
  # exit -9223372036854775808
  # exit -9223372036854775809
  # exit 1 2 3

  # $exit_status
)
# Storing exit status of the subshell
subshell_exit_status=$?

# Printing exit status of subshell
echo "Exit status of subshell: $subshell_exit_status"

echo "Back in main shell"
