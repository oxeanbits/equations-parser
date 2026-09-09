#ifndef MUP_FUNC_ROUND_H
#define MUP_FUNC_ROUND_H

#include "mpICallback.h"

MUP_NAMESPACE_START

class FunRound : public ICallback
{
public:
  FunRound();
  virtual void Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc) override;
  virtual const char_type* GetDesc() const override;
  virtual IToken* Clone() const override;
};

class FunRoundDecimal : public ICallback
{
public:
  FunRoundDecimal();
  virtual void Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc) override;
  virtual const char_type* GetDesc() const override;
  virtual IToken* Clone() const override;
};

MUP_NAMESPACE_END

#endif
