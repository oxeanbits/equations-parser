/*
               __________                                 ____  ___
    _____  __ _\______   \_____ _______  ______ __________\   \/  /
   /     \|  |  \     ___/\__  \\_  __ \/  ___// __ \_  __ \     / 
  |  Y Y  \  |  /    |     / __ \|  | \/\___ \\  ___/|  | \/     \ 
  |__|_|  /____/|____|    (____  /__|  /____  >\___  >__| /___/\  \
        \/                     \/           \/     \/           \_/
                                       Copyright (C) 2016, Ingo Berg
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
#include "mpFuncStr.h"

#include <cmath>
#include <cassert>
#include <cstdio>
#include <cwchar>
#include <algorithm>

#include "mpValue.h"
#include "mpError.h"


MUP_NAMESPACE_START

  //------------------------------------------------------------------------------
  //
  // Concat function
  //
  //------------------------------------------------------------------------------

  FunStrConcat::FunStrConcat()
    :ICallback(cmFUNC, _T("concat"), 2)
  {}

  //------------------------------------------------------------------------------
  void FunStrConcat::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int)
  {
    string_type str1 = a_pArg[0]->GetString();
    string_type str2 = a_pArg[1]->GetString();
    *ret = (string_type) str1 + str2;
  }

  //------------------------------------------------------------------------------
  const char_type* FunStrConcat::GetDesc() const
  {
    return _T("concat(s1, s2) - Returns the concatenation of two strings s1 and s2.");
  }

  //------------------------------------------------------------------------------
  IToken* FunStrConcat::Clone() const
  {
    return new FunStrConcat(*this);
  }

  //------------------------------------------------------------------------------
  //
  // Left function
  //
  //------------------------------------------------------------------------------

  FunStrLeft::FunStrLeft()
    :ICallback(cmFUNC, _T("left"), 2)
  {}

  //------------------------------------------------------------------------------
  void FunStrLeft::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int)
  {
    string_type str = a_pArg[0]->GetString();
    int cut = a_pArg[1]->GetInteger();

    str = str.substr(0, cut);

    *ret = (string_type) str;
  }

  //------------------------------------------------------------------------------
  const char_type* FunStrLeft::GetDesc() const
  {
    return _T("left(s1, x) - Returns the left substring of a string, considering x characters.");
  }

  //------------------------------------------------------------------------------
  IToken* FunStrLeft::Clone() const
  {
    return new FunStrLeft(*this);
  }

  //------------------------------------------------------------------------------
  //
  // Right function
  //
  //------------------------------------------------------------------------------

  FunStrRight::FunStrRight()
    :ICallback(cmFUNC, _T("right"), 2)
  {}

  //------------------------------------------------------------------------------
  void FunStrRight::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int)
  {
    string_type str = a_pArg[0]->GetString();
    int cut = a_pArg[1]->GetInteger();

    str = str.substr(str.size() - cut, str.size());

    *ret = (string_type) str;
  }

  //------------------------------------------------------------------------------
  const char_type* FunStrRight::GetDesc() const
  {
    return _T("right(s1, x) - Returns the right substring of a string, considering x characters.");
  }

  //------------------------------------------------------------------------------
  IToken* FunStrRight::Clone() const
  {
    return new FunStrRight(*this);
  }

  //------------------------------------------------------------------------------
  //
  // default_value() auxiliary overloading functions
  //
  //------------------------------------------------------------------------------
  int_type default_value(int_type value, int_type standard) {
    return value == NULL ? standard : value;
  }

  float_type default_value(float_type value, float_type standard) {
    return value == NULL ? standard : value;
  }

  string_type default_value(string_type value, string_type standard) {
    return value.empty() ? standard : value;
  }

  string_type default_value(int_type value, string_type standard) {
    return value == NULL ? standard : "";
  }

  //------------------------------------------------------------------------------
  //
  // Default Value function (With two strings)
  //
  //------------------------------------------------------------------------------

  FunStrDefaultValue::FunStrDefaultValue()
    :ICallback(cmFUNC, _T("default_value"), 2)
  {}

  //------------------------------------------------------------------------------
  void FunStrDefaultValue::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int)
  {

    int_type integer_value;
    int_type integer_standard;
    float_type float_value;
    float_type float_standard;
    string_type string_value;
    string_type string_standard;

    if (a_pArg[1]->GetType() == 'i') {
      integer_standard = a_pArg[1]->GetInteger();
      integer_value = a_pArg[0]->GetInteger();
      *ret = (int_type) default_value(integer_value, integer_standard);

      return;
    }

    if (a_pArg[1]->GetType() == 'f') {
      float_standard = a_pArg[1]->GetFloat();
      float_value = a_pArg[0]->GetFloat();
      *ret = (float_type) default_value(float_value, float_standard);

      return;
    }

    if (a_pArg[1]->GetType() == 's') {
      string_standard = a_pArg[1]->GetString();

      if (a_pArg[0]->GetType() == 'i') {
        integer_value = a_pArg[0]->GetInteger();

        if (integer_value != NULL) throw ParserError(ErrorContext(ecINVALID_DEFAULT_VALUE_PARAM, GetExprPos(), GetIdent()));

        *ret = (string_type) default_value(integer_value, string_standard);
      } else if (a_pArg[0]->GetType() == 's') {
        string_value = a_pArg[0]->GetString();
        *ret = (string_type) default_value(string_value, string_standard);
      }

      return;
    }
  }

  //------------------------------------------------------------------------------
  const char_type* FunStrDefaultValue::GetDesc() const
  {
    return _T("default_value(value, standard) - If value is NULL return standard, otherwise return value");
  }

  //------------------------------------------------------------------------------
  IToken* FunStrDefaultValue::Clone() const
  {
    return new FunStrDefaultValue(*this);
  }

  //------------------------------------------------------------------------------
  //
  // Length function
  //
  //------------------------------------------------------------------------------

  FunStrLen::FunStrLen()
    :ICallback(cmFUNC, _T("length"), 1)
  {}

  //------------------------------------------------------------------------------
  void FunStrLen::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int)
  {
    string_type str = a_pArg[0]->GetString();
    *ret = (float_type)str.length();
  }

  //------------------------------------------------------------------------------
  const char_type* FunStrLen::GetDesc() const
  {
    return _T("length(s) - Returns the length of the string s.");
  }

  //------------------------------------------------------------------------------
  IToken* FunStrLen::Clone() const
  {
    return new FunStrLen(*this);
  }

  //------------------------------------------------------------------------------
  //
  // ToUpper function
  //
  //------------------------------------------------------------------------------

  FunStrToUpper::FunStrToUpper()
    :ICallback(cmFUNC, _T("toupper"), 1)
  {}

  //------------------------------------------------------------------------------
  void FunStrToUpper::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int)
  {
    using namespace std;

    string_type str = a_pArg[0]->GetString();
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);

    *ret = str;
  }

  //------------------------------------------------------------------------------
  const char_type* FunStrToUpper::GetDesc() const
  {
    return _T("toupper(s) - Converts the string s to uppercase characters.");
  }

  //------------------------------------------------------------------------------
  IToken* FunStrToUpper::Clone() const
  {
    return new FunStrToUpper(*this);
  }

  //------------------------------------------------------------------------------
  //
  // ToLower function
  //
  //------------------------------------------------------------------------------

  FunStrToLower::FunStrToLower()
    :ICallback(cmFUNC, _T("tolower"), 1)
  {}

  //------------------------------------------------------------------------------
  void FunStrToLower::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int)
  {
    using namespace std;

    string_type str = a_pArg[0]->GetString();
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);

    *ret = str;
  }

  //------------------------------------------------------------------------------
  const char_type* FunStrToLower::GetDesc() const
  {
    return _T("tolower(s) - Converts the string s to lowercase characters.");
  }

  //------------------------------------------------------------------------------
  IToken* FunStrToLower::Clone() const
  {
    return new FunStrToLower(*this);
  }

  //------------------------------------------------------------------------------
  //
  // String to double conversion => str2number()
  //
  //------------------------------------------------------------------------------

  FunStrToNumber::FunStrToNumber()
    :ICallback(cmFUNC, _T("str2number"), 1)
  {}

  //------------------------------------------------------------------------------
  void FunStrToNumber::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc)
  {
    assert(a_iArgc==1);
    _unused(a_iArgc);

    string_type in;
    double out;   // <- Ich will hier wirklich double, auch wenn der Type long double
                  // ist. sscanf und long double geht nicht mit GCC!

    in = a_pArg[0]->GetString();
    
#ifndef _UNICODE    
    sscanf(in.c_str(), "%lf", &out);
#else
    swscanf(in.c_str(), _T("%lf"), &out);
#endif

    *ret = (float_type)out;
  }

  //------------------------------------------------------------------------------
  const char_type* FunStrToNumber::GetDesc() const
  {
    return _T("str2number(s) - Converts the string stored in s into a floating foint value.");
  }

  //------------------------------------------------------------------------------
  IToken* FunStrToNumber::Clone() const
  {
    return new FunStrToNumber(*this);
  }

  //------------------------------------------------------------------------------
  //
  // String to double conversion => number()
  //
  //------------------------------------------------------------------------------

  FunStrNumber::FunStrNumber()
    :ICallback(cmFUNC, _T("number"), 1)
  {}

  //------------------------------------------------------------------------------
  void FunStrNumber::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc)
  {
    assert(a_iArgc==1);
    _unused(a_iArgc);

    string_type in;
    double out;

    in = a_pArg[0]->GetString();

#ifndef _UNICODE
    sscanf(in.c_str(), "%lf", &out);
#else
    swscanf(in.c_str(), _T("%lf"), &out);
#endif

    *ret = (float_type)out;
  }

  //------------------------------------------------------------------------------
  const char_type* FunStrNumber::GetDesc() const
  {
    return _T("number(s) - Converts the string stored in s into a floating foint value.");
  }

  //------------------------------------------------------------------------------
  IToken* FunStrNumber::Clone() const
  {
    return new FunStrNumber(*this);
  }
MUP_NAMESPACE_END
