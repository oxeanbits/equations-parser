/*
               __________                                 ____  ___
    _____  __ _\______   \_____ _______  ______ __________\   \/  /
   /     \|  |  \     ___/\__  \\_  __ \/  ___// __ \_  __ \     /
  |  Y Y  \  |  /    |     / __ \|  | \/\___ \\  ___/|  | \/     \
  |__|_|  /____/|____|    (____  /__|  /____  >\___  >__| /___/\  \
        \/                     \/           \/     \/           \_/
                                       Copyright (C) 2016 Ingo Berg
                                       All rights reserved.

  muParserX - A C++ math parser library with array and string support
  Copyright (c) 2016, Ingo Berg
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
  IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
  INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef MUP_FUNC_COMMON_H
#define MUP_FUNC_COMMON_H

#include "mpICallback.h"


MUP_NAMESPACE_START

  //------------------------------------------------------------------------------
  /** \brief Parser function callback for determining the size of an array.
      \ingroup functions
  */
  class FunParserID : public ICallback
  {
  public:
    FunParserID ();
    virtual void Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc) override;
    virtual const char_type* GetDesc() const override;
    virtual IToken* Clone() const override;
  }; // class FunParserID

  //------------------------------------------------------------------------------
  /** \brief Determine maximal value from the parameter list.
      \ingroup functions
  */
  class FunMax : public ICallback
  {
  public:
    FunMax();
    virtual void Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc) override;
    virtual const char_type* GetDesc() const override;
    virtual IToken* Clone() const override;
  }; // class FunMax

  //------------------------------------------------------------------------------
  /** \brief Determine minimal value from the parameter list.
      \ingroup functions
  */
  class FunMin : public ICallback
  {
  public:
    FunMin();
    virtual void Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc) override;
    virtual const char_type* GetDesc() const override;
    virtual IToken* Clone() const override;
  }; // class FunMin

  //------------------------------------------------------------------------------
  /** \brief Parser callback for summing up all function arguments.
      \ingroup functions
  */
  class FunSum : public ICallback
  {
  public:
    FunSum();
    virtual void Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc) override;
    virtual const char_type* GetDesc() const override;
    virtual IToken* Clone() const override;
  }; // class FunSum

  //------------------------------------------------------------------------------
  /** \brief Determine the average value from the parameter list.
      \ingroup functions
  */
  class FunAvg : public ICallback
  {
  public:
    FunAvg();
    virtual void Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc) override;
    virtual const char_type* GetDesc() const override;
    virtual IToken* Clone() const override;
  }; // class FunAvg

  //------------------------------------------------------------------------------
  /** \brief Parser function callback for determining the size of an array.
      \ingroup functions
  */
  class FunSizeOf : public ICallback
  {
  public:
    FunSizeOf();
    virtual ~FunSizeOf();
    virtual void Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc) override;
    virtual const char_type* GetDesc() const override;
    virtual IToken* Clone() const override;
  }; // class FunSizeOf

  //------------------------------------------------------------------------------
  /** \brief Returns the application of a mask into a integer.
      \ingroup functions
  */
  class FunMask : public ICallback
  {
  public:
    FunMask();
    virtual ~FunMask();
    virtual void Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc) override;
    virtual const char_type* GetDesc() const override;
    virtual IToken* Clone() const override;
  }; // class FunMask

    //------------------------------------------------------------------------------
  /** \brief Returns the result of a case operation.
      \ingroup functions
  */
  class FunCase: public ICallback
  {
  public:
    FunCase();
    virtual void Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc) override;
    virtual const char_type* GetDesc() const override;
    virtual IToken* Clone() const override;
  }; // class FunCase

  //------------------------------------------------------------------------------
  /** \brief Determine the difference in days between two dates.
      \ingroup functions
  */
  class FunDaysDiff : public ICallback
  {
  public:
    FunDaysDiff();
    virtual void Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc) override;
    virtual const char_type* GetDesc() const override;
    virtual IToken* Clone() const override;
  }; // class FunDaysDiff

  //------------------------------------------------------------------------------
  /** \brief Determine the difference in hours between two dates.
      \ingroup functions
  */
  class FunHoursDiff : public ICallback
  {
  public:
    FunHoursDiff();
    virtual void Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc) override;
    virtual const char_type* GetDesc() const override;
    virtual IToken* Clone() const override;
  }; // class FunHoursDiff

  //------------------------------------------------------------------------------
  /** \brief Return the current date in the yyyy-mm-dd format.
      \ingroup functions
  */
  class FunCurrentDate : public ICallback
  {
  public:
    FunCurrentDate();
    virtual void Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc) override;
    virtual const char_type* GetDesc() const override;
    virtual IToken* Clone() const override;
  }; // class FunCurrentDate

  //------------------------------------------------------------------------------
  /** \brief Return sum of a date/date_time with a days quantity
      \ingroup functions
  */
  class FunAddDays : public ICallback
  {
  public:
    FunAddDays();
    virtual void Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc) override;
    virtual const char_type* GetDesc() const override;
    virtual IToken* Clone() const override;
  }; // class FunAddDays

  //------------------------------------------------------------------------------
  /** \brief Determine the difference in hours between two times.
      \ingroup functions
  */
  class FunTimeDiff : public ICallback
  {
  public:
    FunTimeDiff();
    virtual void Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc) override;
    virtual const char_type* GetDesc() const override;
    virtual IToken* Clone() const override;
  }; // class FunTimeDiff

  //------------------------------------------------------------------------------
  /** \brief Returns the current time in the HH:MM:SS format.
      \ingroup functions
  */
  class FunCurrentTime : public ICallback
  {
  public:
    FunCurrentTime();
    virtual void Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc) override;
    virtual const char_type* GetDesc() const override;
    virtual IToken* Clone() const override;
  }; // class FunCurrentTime

  //------------------------------------------------------------------------------
  /** \brief Return the capture group of a regular expression.
      \ingroup functions
  */
  class FunRegex : public ICallback
  {
  public:
    FunRegex();
    virtual void Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc) override;
    virtual const char_type* GetDesc() const override;
    virtual IToken* Clone() const override;
  }; // class FunRegex

  //------------------------------------------------------------------------------
  /** \brief Return the week of year of a date.
      \ingroup functions
  */
  class FunWeekYear: public ICallback
  {
  public:
    FunWeekYear();
    virtual void Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc) override;
    virtual const char_type* GetDesc() const override;
    virtual IToken* Clone() const override;
  }; // class FunWeekYear

  //------------------------------------------------------------------------------
  /** \brief Return the weekday of a date.
      \ingroup functions
  */
  class FunWeekDay: public ICallback
  {
  public:
    FunWeekDay();
    virtual void Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc) override;
    virtual const char_type* GetDesc() const override;
    virtual IToken* Clone() const override;
  }; // class FunWeekDay

MUP_NAMESPACE_END

#endif
