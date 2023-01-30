#ifndef EQUATIONS_PARSER_H
#define EQUATIONS_PARSER_H

#include <string>
//--- Parser framework -----------------------------------------------------
#include "mpParser.h"
#include "mpDefines.h"

MUP_NAMESPACE_START

void EquationsParserReplaceAll(std::string& source, const std::string& from, const std::string& to);
std::string EquationsParserCalc(std::string input);
std::string EquationsParserCalcJson(std::string input);
void EquationsParserCalcArray(std::vector<std::string> in, std::vector<std::string> &out);

MUP_NAMESPACE_END

#endif
