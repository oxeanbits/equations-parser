#include "mpFuncRound.h"

#include <cmath>

#include "mpError.h"
#include "mpValue.h"

MUP_NAMESPACE_START

FunRound::FunRound()
  :ICallback(cmFUNC, _T("round"), -1)
{}

void FunRound::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc)
{
  if (a_iArgc < 1) {
    throw ParserError(ErrorContext(ecTOO_FEW_PARAMS, GetExprPos(), GetIdent()));
  } else if (a_iArgc > 2) {
    throw ParserError(ErrorContext(ecTOO_MANY_PARAMS, GetExprPos(), GetIdent()));
  }

  float_type value = a_pArg[0]->GetFloat();
  if (a_iArgc == 1) {
    *ret = std::round(value);
    return;
  }

  string_type direction = a_pArg[1]->GetString();
  if (direction == _T("up")) {
    *ret = std::ceil(value);
  } else if (direction == _T("down")) {
    *ret = std::floor(value);
  } else {
    ErrorContext err(ecINVALID_PARAMETER, GetExprPos(), GetIdent());
    err.Arg = 2;
    throw ParserError(err);
  }
}

const char_type* FunRound::GetDesc() const
{
  return _T("round(x[, direction]) - round x normally, up or down");
}

IToken* FunRound::Clone() const
{
  return new FunRound(*this);
}

MUP_NAMESPACE_END
