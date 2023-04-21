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

# Arithmetic tests
test_eval "2 + 2" "4"
test_eval "55 * 33" "1815"
test_eval "37 / 25.3" "1.46245059288538"
test_eval "sin(0.67)" "0.62098598703656"
test_eval "cos(0.67)" "0.783821665880849"
test_eval "tan(0.67)" "0.792254174728257"
test_eval "(5 + 1) + (6 - 2)" "10"
test_eval "4 + 4 * 3" "16"
test_eval "35.52 * 17.01" "604.1952"
test_eval "35.52 * 11.39" "404.5728"
test_eval "11.84 * 53.57" "634.2688"
test_eval "11.84 * 35.76" "423.3984"
test_eval "(604.1952 + 404.5728 + 634.2688 + 423.3984) * 0.15" "309.96528"
test_eval "10.5 / 5.25" "2"
test_eval "abs(-5)" "5"
test_eval "log10(10)" "1"
test_eval "round(4.4)" "4"
test_eval "(3^3)^2" "729"
test_eval "3^(3^(2))" "19683"
test_eval "10!" "3628800"
test_eval "sqrt(16) + cbrt(8)" "6"
test_eval "log10(10) + ln(e) + log(10)" "4.30258509299405"
test_eval "sin(1) + cos(0) + tan(0.15722)" "1.99999931685569"
test_eval "max(1, 2) + min(3, 4) + sum(5, 6)" "16"
test_eval "avg(9, 9.8, 10)" "9.6"
test_eval "pow(2, 3)" "8"
test_eval "round_decimal(4.559, 2)" "4.56"

# Conditional tests
test_eval "4 > 2 ? \"bigger\" : \"smaller\"" "\"bigger\""
test_eval "2 == 2 ? true : false" "true"
test_eval "2 != 2 ? true : false" "false"
test_eval "\"this\" == \"this\" ? \"yes\" : \"no\"" '"yes"'
test_eval "\"this\" != \"that\" ? \"yes\" : \"no\"" '"yes"'
test_eval "true and false" "false"
test_eval "true or false" "true"
test_eval "(3==3) and (3!=3)" "false"
test_eval "exp(1) == e" "true"

# String tests
test_eval "length(\"test string\")" "11"
test_eval "toupper(\"test string\")" '"TEST STRING"'
test_eval "tolower(\"TEST STRING\")" '"test string"'
test_eval "concat(\"Hello \", \"World\")" '"Hello World"'
test_eval "concat(\"\", \"Hello World\")" '"Hello World"'
test_eval "concat(\"Hello World\", \"\")" '"Hello World"'
test_eval "concat(concat(\"tes t\", \" \\\"str \\\\\\\" \\\" ing \"),string(\"equa  \\\"   tion\"))" '"tes t "str \" " ing equa  "   tion"'
test_eval "str2number(\"5\")" "5"
test_eval "number(\"5\")" "5"
test_eval "left(\"Hello World\", 5)" '"Hello"'
test_eval "right(\"Hello World\", 5)" '"World"'

# Array tests
test_eval "link(\"Title\", \"http://foo.bar\")" '"<a href="http://foo.bar">Title</a>"'
test_eval 'link("Title", "#")' '"<a href="#">Title</a>"'


echo "All tests passed!"

