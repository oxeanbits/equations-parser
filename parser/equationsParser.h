#ifndef EQUATIONS_PARSER_H
#define EQUATIONS_PARSER_H

#include <string>
//--- Parser framework -----------------------------------------------------
#include "mpParser.h"
#include "mpDefines.h"

#define EQUATIONS_PARSER_START namespace EquationsParser {
#define EQUATIONS_PARSER_END }

EQUATIONS_PARSER_START

void ReplaceAll(std::string& source, const std::string& from, const std::string& to);
std::string Calc(std::string input);
std::string CalcJson(std::string input);
void CalcArray(std::vector<std::string> in, std::vector<std::string> &out);
mup::Value PureCalc(std::string input);

EQUATIONS_PARSER_END

#endif
