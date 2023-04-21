#!/bin/bash


echo "Running tests..."

function test_eval() {
  input=$1
  expected_output="ans = "$2
  actual_output=$(echo "$input
exit" | ./example | grep "ans =")

  if [[ "$actual_output" == "$expected_output" ]]; then
    echo -e "\e[32mTest passed for input $input\e[0m"
  else
    echo -e "\e[31mTest failed for input $input: Expected $expected_output but got $actual_output\e[0m"
    exit 1
  fi
}

# Test cases
test_eval "2 + 2" "4"
test_eval "55 * 33" "1815"
test_eval "37 / 25.3" "1.46245059288538"
test_eval "sin(0.67)" "0.62098598703656"
test_eval "cos(0.67)" "0.783821665880849"

echo "All tests passed!"

