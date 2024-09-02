#!/bin/bash


echo "Running tests..."

function test_eval() {
  input=$1
  expected_output1="ans = "$2
  expected_output2="ans = "$3
  actual_output=$(echo "$input
exit" | ./example | grep "ans =")

  if [[ "$actual_output" == "$expected_output1" ]]; then
    printf "\e[32mTest passed for input $input\e[0m\n"
  elif [[ "$actual_output" == "$expected_output2" ]]; then
    printf "\e[32mTest passed for input $input\e[0m\n"
  else
    if [ -n "$3" ]; then
      printf "\e[31mTest failed for input $input: Expected $expected_output1 or $expected_output2 but got $actual_output\e[0m\n"
    else
      printf "\e[31mTest failed for input $input: Expected $expected_output1 but got $actual_output\e[0m\n"
    fi
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
test_eval '((0.09/1.0)+2.58)-1.67' '1'
test_eval '10^log(3+2)' '40.6853365119738'
test_eval 'log(e)' '1'
test_eval '2^5^0' '2'

# String tests
test_eval "length(\"test string\")" "11"
test_eval "toupper(\"test string\")" '"TEST STRING"'
test_eval "tolower(\"TEST STRING\")" '"test string"'
test_eval "concat(\"Hello \", \"World\")" '"Hello World"'
test_eval "concat(\"\", \"Hello World\")" '"Hello World"'
test_eval "concat(\"Hello World\", \"\")" '"Hello World"'
test_eval "concat(concat(\"tes t\", \" \\\"str \\\\\\\" \\\" ing \"),string(\"equa  \\\"   tion\"))" '"tes t "str \" " ing equa  "   tion"'
test_eval "left(\"Hello World\", 5)" '"Hello"'
test_eval "right(\"Hello World\", 5)" '"World"'
test_eval 'left("Hello World", 5)' '"Hello"'
test_eval 'right("Hello World", 5)' '"World"'
test_eval 'right("Hello World", 20)' '"Hello World"'
test_eval 'contains("Hello World", "orld")' 'true'
test_eval 'contains("One Flew Over The Cuckoo'"'"'s", "koo")' 'true'
test_eval 'contains("Hello World", "Worlds")' 'false'
test_eval 'contains("1234567", "456")' 'true'
test_eval 'contains("1234567", "789")' 'false'
test_eval 'contains("2019-01-01T:08:30", "2019-01-01")' 'true'
test_eval 'contains("2019-01-01T:08:30", "2021-01-01")' 'false'
test_eval '"Maçã"' '"Maçã"'
test_eval '"string with quote\""' '"string with quote""'
test_eval 'toupper(concat("hello ", "world"))' '"HELLO WORLD"'
test_eval 'tolower("TEST LOWERCASE")' '"test lowercase"'

# Calculate tests
test_eval 'calculate("2+2+2*4")' '"12"'
test_eval 'calculate("(2+2)*4")' '"16"'
test_eval 'calculate("2^4")' '"16"'
test_eval 'calculate("sqrt(9)")' '"3"'
test_eval 'calculate("abs(-50)")' '"50"'
test_eval 'calculate("round(1.123)")' '"1"'
test_eval 'calculate("add_days(\"2019-01-01\", 3)")' '"2019-01-04"'
test_eval 'calculate("daysdiff(\"2019-01-01\", \"2019-01-02\")")' '"1"'
test_eval 'calculate("hoursdiff(\"2019-01-01\", \"2019-01-02\")")' '"24"'
test_eval 'calculate("3 > 2 ? \"higher\" : \"lower\"")' '"higher"'
test_eval 'calculate("3 < 2 ? \"higher\" : \"lower\"")' '"lower"'
test_eval 'calculate("concat(\"One \", concat(\"Two\", \" Three\"))")' '"One Two Three"'
test_eval 'calculate("\"One\" // \" \" // \"Two\" // \" \" // \"Three\"")' '"One Two Three"'
test_eval 'calculate("number(calculate(\"1 + 1\")) + 1")' '"3"'

# Array tests
test_eval "link(\"Title\", \"http://foo.bar\")" '"<a href="http://foo.bar">Title</a>"'
test_eval 'link("Title", "#")' '"<a href="#">Title</a>"'

test_eval 'link("Test title", "/test")' '"<a href="/test">Test title</a>"'
test_eval 'link("Test title", "/test", "testFileName")' '"<a href="/test" download="testFileName">Test title</a>"'

test_eval '5 > 3 ? "yes" : "no"' '"yes"'
test_eval '"this" == "this" ? 1 : 0' '1'
test_eval 'sqrt(9)+cbrt(8)+abs(-4.9812)' '9.9812'
test_eval '10! - 5! * -(-1)' '3628680'
test_eval 'sum(1,2,3,4,5) == max(14.99, 15)' 'true'
test_eval 'avg(1,2,3,4,5,6,7,8,9,10) / 10' '0.55'
test_eval 'round(4.62)' '5'
test_eval 'round_decimal(4.625, 2)' '4.63'

# Cast tests
test_eval "str2number(\"5\")" "5"
test_eval "number(\"5\")" "5"
test_eval 'string(5)' '"5"'
test_eval 'string(5.123)' '"5.123"'
test_eval 'string(4)' '"4"'
test_eval 'string(4.5)' '"4.5"'
test_eval 'string(true)' '"true"'
test_eval 'string(false)' '"false"'
test_eval 'string("4")' '"4"'
test_eval 'number(4)' '4'
test_eval 'number(4.5)' '4.5'
test_eval 'number(true)' '1'
test_eval 'number(false)' '0'
test_eval 'number("4")' '4'

# Date tests
test_eval 'current_date()' '"'$(date '+%Y-%m-%d')'"'
test_eval 'daysdiff("2018-01-01", "2018-12-31")' '364'
test_eval 'daysdiff("2016-01-01", "2016-12-31")' '365'
test_eval 'daysdiff("2000-01-01", "2000-12-31")' '365'
test_eval 'daysdiff("2100-01-01", "2100-12-31")' '364'
test_eval 'daysdiff("2018-01-01", "2017-12-31")' '1'
test_eval 'hoursdiff("2018-01-01", "2018-01-02")' '24'
test_eval 'hoursdiff("2018-01-01", "2018-1-02")' '24'
test_eval 'hoursdiff("2018-1-01", "2018-01-02")' '24'
test_eval 'hoursdiff("2018-1-01", "2018-1-02")' '24'
test_eval 'hoursdiff("2018-01-01", "2018-01-2")' '24'
test_eval 'hoursdiff("2018-01-1", "2018-01-02")' '24'
test_eval 'hoursdiff("2018-01-1", "2018-01-2")' '24'
test_eval 'hoursdiff("2018-1-1", "2018-1-2")' '24'
test_eval 'hoursdiff("2019-02-01", "2019-02-13")' '288'
test_eval 'hoursdiff("2019-02-01T08:00", "2019-02-01T12:00")' '4'
test_eval 'hoursdiff("2019-02-01T08:00", "2019-02-02T12:00")' '28'
test_eval 'hoursdiff("2019-02-01T08:20", "2019-02-01T12:00")' '3.67'
test_eval 'hoursdiff(current_date(), current_date())' '0'
test_eval 'hoursdiff("2018-01-01", "2018-01-01")' '0'
test_eval 'hoursdiff("2018-01-02T08:00", "2018-01-01T12:00")' '-20'
test_eval 'add_days("2019-01-01", 0)' '"2019-01-01"'
test_eval 'add_days("2019-01-01", 1)' '"2019-01-02"'
test_eval 'add_days("2019-01-01", -1)' '"2018-12-31"'
test_eval 'add_days("2019-01-01", 3)' '"2019-01-04"'
test_eval 'add_days("2019-01-01T08:30", 0)' '"2019-01-01T08:30"'
test_eval 'add_days("2019-01-01T12:30", 31)' '"2019-02-01T12:30"'
test_eval 'add_days("2019-01-01T15:30", 1)' '"2019-01-02T15:30"'
test_eval 'add_days("2019-01-01T08:30", 1.5)' '"2019-01-02T20:30"'
test_eval 'add_days("2019-01-01T08:30", -1)' '"2018-12-31T08:30"'
test_eval 'timediff("02:00:00", "03:30:00")' '1.5'
test_eval 'timediff("03:30:00", "02:00:00")' '22.5'
test_eval 'timediff("02:00:00", "02:00:30")' '0.01'

# Mask tests
test_eval 'mask("000-000", 123456)' '"123-456"'
test_eval 'mask("00000", 14)' '"00014"'
test_eval 'mask("000 00", 14)' '"000 14"'
test_eval 'concat("#", mask("000", 123))' '"#123"'
test_eval 'mask("0000", 12345)' '"12345"'
test_eval 'mask("00 00", 12345)' '"123 45"'
test_eval 'mask("0-0000-0000", 355911801)' '"3-5591-1801"'
test_eval 'mask("00-0000-0000", 3555911801)' '"35-5591-1801"'
test_eval 'mask("00-0000-0000-0000", 12123412341234)' '"12-1234-1234-1234"'
test_eval 'mask("0-0000-0000-0000-0000", 11234123412341234)' '"1-1234-1234-1234-1234"'
test_eval 'timediff("02:00:30", "02:00:00")' '23.99'
test_eval 'timediff("02:00:00", "02:00:00")' '0'

# Default value tests
test_eval 'default_value(10, 1)' '10'
test_eval 'default_value(NULL, 1)' '1'
test_eval 'default_value(10.4, 1.01)' '10.4'
test_eval 'default_value(NULL, 1.01)' '1.01'
test_eval 'default_value("filled", "default")' '"filled"'
test_eval 'default_value(NULL, "default")' '"default"'
test_eval 'default_value(false, true)' 'false'
test_eval 'default_value(NULL, true)' 'true'
test_eval 'default_value(1, 4.5)' '1'
test_eval 'default_value(1, 10)' '1'
test_eval 'default_value(1, 10.0)' '1'
test_eval 'default_value(1.0, 10)' '1'
test_eval 'default_value(1.0, 10.0)' '1'
test_eval 'default_value(1.5, 10)' '1.5'
test_eval 'default_value(1.5, 10.0)' '1.5'
test_eval 'default_value(1.5, 10.5)' '1.5'

# Exceptional cases
test_eval '4 / 0' 'inf'
test_eval '0 / 0' 'nan' '-nan'

# Regex tests
test_eval 'regex("Hello World", "Hello (.*)")' '"World"'
test_eval 'regex("71 123456789", "([0-9]{2}) [0-9]{9}")' '"71"'
test_eval 'regex("ISSUE-123 Fix bug", "(ISSUE-[0-9]+) (.*)")' '"ISSUE-123"'
test_eval 'regex("2019-01-01T:08:30", "([0-9]{4}-[0-9]{2}-[0-9]{2})T:[0-9]{2}:[0-9]{2}")' '"2019-01-01"'
test_eval 'regex("2019-01-01T:08:30", "([0-9]{4}-[0-9]{2})-[0-9]{2}T:[0-9]{2}:[0-9]{2}")' '"2019-01"'
test_eval 'regex("2019-01-01T:08:30", "([0-9]{4})-[0-9]{2}-[0-9]{2}T:[0-9]{2}:[0-9]{2}")' '"2019"'
# Regex tests with no capture group
test_eval 'regex("Hello World", "Hello .*")' '""'

# Regex tests with optional groups
test_eval 'regex("Product 1234 (color: red)", "Product ([0-9]+)( \\(color: (.*)\\))?")' '"1234"'
test_eval 'regex("Product 1234", "Product ([0-9]+)( \\(color: (.*)\\))?")' '"1234"'

# Regex tests with alternation
test_eval 'regex("Green Apple", "(Green|Red) Apple")' '"Green"'
test_eval 'regex("Red Apple", "(Green|Red) Apple")' '"Red"'

# Regex tests with character classes
test_eval 'regex("BoxA123", "Box([A-Za-z][0-9]+)")' '"A123"'
test_eval 'regex("BoxC456", "Box([A-Za-z][0-9]+)")' '"C456"'

# Regex tests with positive lookaheads
test_eval 'regex("apple123banana", "([a-z]+)(?=\\d+)")' '"apple"'
test_eval 'regex("123red456blue", "(\\d+)(?=blue)")' '"456"'

# Regex tests with negative lookaheads
# test_eval 'regex("apple123banana", "([a-z]+)(?!\\d+)")' '"apple"' not working
test_eval 'regex("123red456blue", "(\\d+)(?!blue)")' '"123"'

# Regex tests with nested lookaheads
test_eval 'regex("apple123redbanana", "(?=apple)([a-z]+)(?=\\d+red)")' '"apple"'
test_eval 'regex("apple123red456banana", "(?=apple)([a-z]+)(?=(\\d+red)\\d+banana)")' '"apple"'

# Regex tests with positive and negative lookaheads combined
test_eval 'regex("apple123redbanana", "([a-z]+)(?=\\d+)(?!red)")' '"apple"'
test_eval 'regex("123red456blue789green", "(\\d+)(?=blue)(?!red)")' '"456"'

# Regex tests with nested capture groups
test_eval 'regex("abc123def", "(abc(123)def)")' '"abc123def"'

# Regex tests with non-capture groups
test_eval 'regex("2019-01-01T08:30", "([0-9]{4}(?:-[0-9]{2}){2})T[0-9]{2}:[0-9]{2}")' '"2019-01-01"'

# Regex tests with quantifiers
test_eval 'regex("aabbcc", "a{2}(b{2}c{2})")' '"bbcc"'

# Regex tests with escaped characters
test_eval 'regex("a.b.c", "a\\.(b\\.c)")' '"b.c"'
test_eval 'regex("a[1]b[2]c", "(a\\[1\\]b)\\[2\\]c")' '"a[1]b"'

# Regex tests with no match
test_eval 'regex("Hello World", "Bye (.*)")' '""'

# Week number of the year when 1st of January is a Sunday
test_eval 'weekyear("2023-01-01")' '1'
test_eval 'weekyear("2023-01-07")' '1'
test_eval 'weekyear("2023-01-08")' '2'

test_eval 'weekyear("2023-04-25")' '17'
test_eval 'weekyear("2023-04-29")' '17'
test_eval 'weekyear("2023-04-30")' '18'
test_eval 'weekyear("2023-05-06")' '18'

test_eval 'weekyear("2023-12-24")' '52'
test_eval 'weekyear("2023-12-31")' '53'

# Week number of the year a leap year
test_eval 'weekyear("2024-01-01")' '1'
test_eval 'weekyear("2024-01-06")' '1'
test_eval 'weekyear("2024-01-07")' '2'

test_eval 'weekyear("2024-12-22")' '52'
test_eval 'weekyear("2024-12-29")' '53'

# Week number of the year when 1st of January is friday
test_eval 'weekyear("2027-01-01")' '53'
test_eval 'weekyear("2027-01-02")' '53'
test_eval 'weekyear("2027-01-03")' '1'

test_eval 'weekyear("2027-12-27")' '52'
test_eval 'weekyear("2027-12-31")' '52'
test_eval 'weekyear("2028-01-01")' '53'

# weekday with date
test_eval 'weekday("2021-03-21")' '0'
test_eval 'weekday("2016-03-21")' '1'
test_eval 'weekday("2017-03-21")' '2'
test_eval 'weekday("2018-03-21")' '3'
test_eval 'weekday("2019-03-21")' '4'

# weekday with date_time
test_eval 'weekday("2014-03-21T21:03")' '5'
test_eval 'weekday("2015-03-21T21:03")' '6'

echo "All tests passed!"

