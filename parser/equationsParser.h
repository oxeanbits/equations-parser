#ifndef EQUATIONS_PARSER_H
#define EQUATIONS_PARSER_H

#include <string>
//--- Parser framework -----------------------------------------------------
#include "mpParser.h"
#include "mpDefines.h"

#define EQUATIONS_PARSER_START namespace EquationsParser {
#define EQUATIONS_PARSER_END }

EQUATIONS_PARSER_START

void ReplaceAll(mup::string_type& source, const mup::string_type& from, const mup::string_type& to);
mup::string_type Calc(mup::string_type input);
mup::string_type CalcJson(mup::string_type input);
void CalcArray(std::vector<mup::string_type> in, std::vector<mup::string_type> &out);

EQUATIONS_PARSER_END

#endif
