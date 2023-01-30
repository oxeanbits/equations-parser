#include "equationsParser.h"

#include <string>
#include <iostream>
#include <cmath>
#include <vector>

using namespace std;
using namespace mup;

EQUATIONS_PARSER_START

/**
 * @brief Evaluates an input string as a mathematical expression and returns the result
 * @param input The string to be evaluated as a mathematical expression
 */
string Calc(string input) {
  ParserX parser(pckALL_NON_COMPLEX);

  Value ans;
  parser.DefineVar(_T("ans"), Variable(&ans));

  try
  {
    parser.SetExpr(input);
    ans = parser.Eval();

    return ans.AsString();
  }
  catch(ParserError &e)
  {
    if (e.GetPos() != -1) {
      string_type error = "Error: ";
      error.append(e.GetMsg());
      return error;
    }
  }
  catch(std::runtime_error & ex)
  {
    string_type error = "Error: Runtime error - ";
    error.append(ex.what());
    return error;
  }
  return ans.AsString();
}

/**
 * @brief Replaces all occurrences of the substring @from in the source string with another the
 * substring @to
 * @param source A reference to the string to perform the replacement on
 * @param from The substring to be replaced
 * @param to The substring that will replace 'from'
 */
void ReplaceAll(std::string& source, const std::string& from, const std::string& to) {
  std::string newString;
  newString.reserve(source.length());  // avoids a few memory allocations

  std::string::size_type lastPos = 0;
  std::string::size_type findPos;

  while(std::string::npos != (findPos = source.find(from, lastPos)))
  {
    newString.append(source, lastPos, findPos - lastPos);
    newString += to;
    lastPos = findPos + from.length();
  }

  // Care for the rest after last occurrence
  newString += source.substr(lastPos);

  source.swap(newString);
}

/**
 * @brief Evaluates an input string as a mathematical expression and returns the result as a JSON
 * @param input The string to be evaluated as a mathematical expression
 * @return The result of the evaluation as a JSON string in the following format:
 * {
 *    "val": "result_value",
 *    "type": "result_type"
 * }
 * or in case of error:
 * {
 *    "error": "error_message"
 * }
 */
string CalcJson(string input) {
  ParserX parser(pckALL_NON_COMPLEX);

  Value ans;
  parser.DefineVar(_T("ans"), Variable(&ans));

  stringstream_type ss;

  ss << _T("{");

  try
  {
    parser.SetExpr(input);
    ans = parser.Eval();

    std::string ansString = ans.AsString();

    ReplaceAll(ansString, "\"", "\\\"");

    ss << _T("\"val\": \"") << ansString << _T("\"");
    ss << _T(",\"type\": \"") << ans.GetType() << _T("\"");
  }
  catch(ParserError &e)
  {
    if (e.GetPos() != -1) {
      string_type error = e.GetMsg();
      ss << _T("\"error\": \"") << error << _T("\"");
    }
  }
  catch(std::runtime_error & ex)
  {
    string_type error = "Error: Runtime error - ";
    error.append(ex.what());
    ss << _T("\"error\": \"") << error << _T("\"");
  }

  ss << _T("}");

  return ss.str();
}

/**
 * Calculates the result of a list of equations and stores them in the 'out' vector.
 *
 * @param equations a vector of strings representing mathematical equations
 * @param out a vector of strings where the results of the calculations will be stored
 */
void CalcArray(vector<string> equations, vector<string> &out) {
  for(string equation : equations) {
    out.push_back(CalcJson(equation));
  }
}

EQUATIONS_PARSER_END
