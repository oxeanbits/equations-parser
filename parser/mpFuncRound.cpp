#include "mpFuncRound.h"

#include <cmath>

#include "mpError.h"
#include "mpValue.h"

MUP_NAMESPACE_START

namespace
{
  void eval_round(ICallback &callback,
                  ptr_val_type &ret,
                  const ptr_val_type *args,
                  int argc,
                  bool has_precision)
  {
    int required_argc = has_precision ? 2 : 1;
    if (argc < required_argc) {
      throw ParserError(ErrorContext(ecTOO_FEW_PARAMS,
                                     callback.GetExprPos(),
                                     callback.GetIdent()));
    } else if (argc > required_argc + 1) {
      throw ParserError(ErrorContext(ecTOO_MANY_PARAMS,
                                     callback.GetExprPos(),
                                     callback.GetIdent()));
    }

    int_type scale = 1;
    if (has_precision) {
      int_type precision = args[1]->GetFloat();
      scale = std::pow(10, precision);
    }

    float_type value = args[0]->GetFloat() * scale;
    if (argc == required_argc) {
      *ret = std::round(value) / scale;
      return;
    }

    string_type direction = args[required_argc]->GetString();
    if (direction == _T("up")) {
      *ret = std::ceil(value) / scale;
    } else if (direction == _T("down")) {
      *ret = std::floor(value) / scale;
    } else {
      ErrorContext err(ecINVALID_PARAMETER,
                       callback.GetExprPos(),
                       callback.GetIdent());
      err.Arg = required_argc + 1;
      throw ParserError(err);
    }
  }
}

FunRound::FunRound()
  :ICallback(cmFUNC, _T("round"), -1)
{}

void FunRound::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc)
{
  eval_round(*this, ret, a_pArg, a_iArgc, false);
}

const char_type* FunRound::GetDesc() const
{
  return _T("round(x[, direction]) - round x normally, up or down");
}

IToken* FunRound::Clone() const
{
  return new FunRound(*this);
}

FunRoundDecimal::FunRoundDecimal()
  :ICallback(cmFUNC, _T("round_decimal"), -1)
{}

void FunRoundDecimal::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc)
{
  eval_round(*this, ret, a_pArg, a_iArgc, true);
}

const char_type* FunRoundDecimal::GetDesc() const
{
  return _T("round_decimal(x, precision[, direction]) - round x at the given precision");
}

IToken* FunRoundDecimal::Clone() const
{
  return new FunRoundDecimal(*this);
}

MUP_NAMESPACE_END
