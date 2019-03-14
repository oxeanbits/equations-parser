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
#include "mpFuncCommon.h"

#include <cassert>
#include <string>
#include <iostream>
#include <ctime>
#include <regex>

#include "mpValue.h"
#include "mpParserBase.h"


MUP_NAMESPACE_START

  //------------------------------------------------------------------------------
  //
  // FunParserID
  //
  //------------------------------------------------------------------------------

  FunParserID::FunParserID()
    :ICallback(cmFUNC, _T("parserid"), 0)
  {}

  //------------------------------------------------------------------------------
  void FunParserID::Eval(ptr_val_type &ret, const ptr_val_type * /*a_pArg*/, int /*a_iArgc*/)
  {
    string_type sVer = _T("muParserX V") + GetParent()->GetVersion();
    *ret = sVer;
  }

  //------------------------------------------------------------------------------
  const char_type* FunParserID::GetDesc() const
  {
    return _T("parserid() - muParserX version information");
  }

  //------------------------------------------------------------------------------
  IToken* FunParserID::Clone() const
  {
    return new FunParserID(*this);
  }

  //------------------------------------------------------------------------------
  //
  // Max Function
  //
  //------------------------------------------------------------------------------

  FunMax::FunMax() : ICallback(cmFUNC, _T("max"), -1)
  {}

  //------------------------------------------------------------------------------
  void FunMax::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc)
  {
    if (a_iArgc < 1)
        throw ParserError(ErrorContext(ecTOO_FEW_PARAMS, GetExprPos(), GetIdent()));

    float_type max(-1e30), val(0);
    for (int i=0; i<a_iArgc; ++i)
    {
      switch(a_pArg[i]->GetType())
      {
      case 'f':
      case 'i': val = a_pArg[i]->GetFloat(); break;
      case 'n': break; // ignore not in list entries (missing parameter)
      case 'c':
      default:
        {
          ErrorContext err;
          err.Errc = ecTYPE_CONFLICT_FUN;
          err.Arg = i+1;
          err.Type1 = a_pArg[i]->GetType();
          err.Type2 = 'f';
          throw ParserError(err);
        }
      }
      max = std::max(max, val);
    }

    *ret = max;
  }

  //------------------------------------------------------------------------------
  const char_type* FunMax::GetDesc() const
  {
    return _T("max(x,y,...,z) - Returns the maximum value from all of its function arguments.");
  }

  //------------------------------------------------------------------------------
  IToken* FunMax::Clone() const
  {
    return new FunMax(*this);
  }

  //------------------------------------------------------------------------------
  //
  // Min Function
  //
  //------------------------------------------------------------------------------

  FunMin::FunMin() : ICallback(cmFUNC, _T("min"), -1)
  {}

  //------------------------------------------------------------------------------
  /** \brief Returns the minimum value of all values.
      \param a_pArg Pointer to an array of Values
      \param a_iArgc Number of values stored in a_pArg
  */
  void FunMin::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc)
  {
    if (a_iArgc < 1)
        throw ParserError(ErrorContext(ecTOO_FEW_PARAMS, GetExprPos(), GetIdent()));

    float_type min(1e30), val(min);

    for (int i=0; i<a_iArgc; ++i)
    {
      switch(a_pArg[i]->GetType())
      {
      case 'f':
      case 'i': val = a_pArg[i]->GetFloat();   break;
      default:
        {
          ErrorContext err;
          err.Errc = ecTYPE_CONFLICT_FUN;
          err.Arg = i+1;
          err.Type1 = a_pArg[i]->GetType();
          err.Type2 = 'f';
          throw ParserError(err);
        }
      }
      min = std::min(min, val);
    }

    *ret = min;
  }

  //------------------------------------------------------------------------------
  const char_type* FunMin::GetDesc() const
  {
    return _T("min(x,y,...,z) - Returns the minimum value from all of its function arguments.");
  }

  //------------------------------------------------------------------------------
  IToken* FunMin::Clone() const
  {
    return new FunMin(*this);
  }

  //------------------------------------------------------------------------------
  //
  // class FunSum
  //
  //------------------------------------------------------------------------------

  FunSum::FunSum()
    :ICallback(cmFUNC, _T("sum"), -1)
  {}

  //------------------------------------------------------------------------------
  /** \brief Returns the minimum value of all values.
      \param a_pArg Pointer to an array of Values
      \param a_iArgc Number of values stored in a_pArg
  */
  void FunSum::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc)
  {
    if (a_iArgc < 1)
        throw ParserError(ErrorContext(ecTOO_FEW_PARAMS, GetExprPos(), GetIdent()));

    float_type sum(0);

    for (int i=0; i<a_iArgc; ++i)
    {
      switch(a_pArg[i]->GetType())
      {
      case 'f':
      case 'i': sum += a_pArg[i]->GetFloat();   break;
      default:
        {
          ErrorContext err;
          err.Errc = ecTYPE_CONFLICT_FUN;
          err.Arg = i+1;
          err.Type1 = a_pArg[i]->GetType();
          err.Type2 = 'f';
          throw ParserError(err);
        }
      }
    }

    *ret = sum;
  }

  //------------------------------------------------------------------------------
  const char_type* FunSum::GetDesc() const
  {
    return _T("sum(x,y,...,z) - Returns the sum of all arguments.");
  }

  //------------------------------------------------------------------------------
  IToken* FunSum::Clone() const
  {
    return new FunSum(*this);
  }

  //------------------------------------------------------------------------------
  //
  // class FunAvg
  //
  //------------------------------------------------------------------------------

  FunAvg::FunAvg()
    :ICallback(cmFUNC, _T("avg"), -1)
  {}

  //------------------------------------------------------------------------------
  /** \brief Returns the minimum value of all values.
      \param a_pArg Pointer to an array of Values
      \param a_iArgc Number of values stored in a_pArg
  */
  void FunAvg::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc)
  {
    if (a_iArgc < 1)
        throw ParserError(ErrorContext(ecTOO_FEW_PARAMS, GetExprPos(), GetIdent()));

    float_type avg(0);

    for (int i=0; i<a_iArgc; ++i)
    {
      switch(a_pArg[i]->GetType())
      {
      case 'f':
      case 'i': avg += a_pArg[i]->GetFloat();   break;
      default:
        {
          ErrorContext err;
          err.Errc = ecTYPE_CONFLICT_FUN;
          err.Arg = i+1;
          err.Type1 = a_pArg[i]->GetType();
          err.Type2 = 'f';
          throw ParserError(err);
        }
      }
    }

    avg = avg/a_iArgc;

    *ret = avg;
  }

  //------------------------------------------------------------------------------
  const char_type* FunAvg::GetDesc() const
  {
    return _T("avg(x,y,...,z) - Returns the average of all arguments.");
  }

  //------------------------------------------------------------------------------
  IToken* FunAvg::Clone() const
  {
    return new FunAvg(*this);
  }

  //------------------------------------------------------------------------------
  //
  // SizeOf
  //
  //------------------------------------------------------------------------------

  FunSizeOf::FunSizeOf()
    :ICallback(cmFUNC, _T("sizeof"), 1)
  {}

  //------------------------------------------------------------------------------
  FunSizeOf::~FunSizeOf()
  {}

  //------------------------------------------------------------------------------
  /** \brief Returns the number of elements stored in the first parameter. */
  void FunSizeOf::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc)
  {
    assert(a_iArgc==1);
    _unused(a_iArgc);

    *ret = (float_type)a_pArg[0]->GetArray().GetRows();
  }

  //------------------------------------------------------------------------------
  const char_type* FunSizeOf::GetDesc() const
  {
    return _T("sizeof(a) - Returns the number of elements in a.");
  }

  //------------------------------------------------------------------------------
  IToken* FunSizeOf::Clone() const
  {
    return new FunSizeOf(*this);
  }

  //------------------------------------------------------------------------------
  //
  // class FunDateDiff
  //
  //------------------------------------------------------------------------------

  //https://en.wikipedia.org/wiki/Rata_Die
  int rata_die(int y, int m, int d) { /* Rata Die day one is 0001-01-01 */
    if (m < 3)
      y--, m += 12;
    return 365*y + y/4 - y/100 + y/400 + (153*m - 457)/5 + d - 306;
  }

  FunDateDiff::FunDateDiff()
    :ICallback(cmFUNC, _T("daysdiff"), -1)
  {}

  //------------------------------------------------------------------------------
  /** \brief Returns the number of days between two dates.
      \param a_pArg Pointer to an array of Values
      \param a_iArgc Number of values stored in a_pArg
  */
  void FunDateDiff::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc)
  {
    if (a_iArgc != 2)
        throw ParserError(ErrorContext(ecTOO_FEW_PARAMS, GetExprPos(), GetIdent()));

    string_type date_a = a_pArg[0]->GetString();
    string_type date_b = a_pArg[1]->GetString();

    struct tm tm, tm2;
    //http://man7.org/linux/man-pages/man3/strptime.3.html
    if (!strptime(date_a.c_str(), "%Y-%m-%d", &tm)) {
      ErrorContext err;
      err.Errc = ecINVALID_DATE_FORMAT;
      err.Arg = 1;
      err.Type1 = a_pArg[0]->GetType();
      err.Type2 = 's';
      throw ParserError(err);
    }

    if (!strptime(date_b.c_str(), "%Y-%m-%d", &tm2)) {
      ErrorContext err;
      err.Errc = ecINVALID_DATE_FORMAT;
      err.Arg = 2;
      err.Type1 = a_pArg[1]->GetType();
      err.Type2 = 's';
      throw ParserError(err);
    }

    int total_days1 = rata_die(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    int total_days2 = rata_die(tm2.tm_year + 1900, tm2.tm_mon + 1, tm2.tm_mday);

    *ret = abs(total_days1 - total_days2);
  }

  const char_type* FunDateDiff::GetDesc() const
  {
    return _T("daysdiff(a,b) - Returns the difference in days between two dates.");
  }

  IToken* FunDateDiff::Clone() const
  {
    return new FunDateDiff(*this);
  }

  //------------------------------------------------------------------------------
  //
  // class FunHoursDiff
  //
  //------------------------------------------------------------------------------

  FunHoursDiff::FunHoursDiff()
    :ICallback(cmFUNC, _T("hoursdiff"), -1)
  {}

  //------------------------------------------------------------------------------
  /** \brief Returns the number of hours between two dates.
      \param a_pArg Pointer to an array of Values
      \param a_iArgc Number of values stored in a_pArg
  */
  void FunHoursDiff::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc)
  {
    if (a_iArgc != 2)
        throw ParserError(ErrorContext(ecTOO_FEW_PARAMS, GetExprPos(), GetIdent()));

    string_type date_a = a_pArg[0]->GetString();
    string_type date_b = a_pArg[1]->GetString();

    // Matches exactly: "yyyy-mm-dd"
    std::regex basic_date ("^\\d{4}-\\d{2}-\\d{2}$");
    if (std::regex_match (date_a, basic_date) && std::regex_match (date_b, basic_date)) {
      date_a += "T00:00";
      date_b += "T00:00";
    } else if (std::regex_match (date_a, basic_date) || std::regex_match (date_b, basic_date)) {
      ErrorContext err;
      err.Errc = ecDATE_AND_DATETIME;
      err.Arg = 1;
      err.Type1 = a_pArg[0]->GetType();
      err.Type2 = 's';
      throw ParserError(err);
    }

    // http://man7.org/linux/man-pages/man3/strptime.3.html
    struct tm tm, tm2;
    if (!strptime(date_a.c_str(), "%Y-%m-%dT%H:%M", &tm)) {
      ErrorContext err;
      err.Errc = ecINVALID_DATETIME_FORMAT;
      err.Arg = 1;
      err.Type1 = a_pArg[0]->GetType();
      err.Type2 = 's';
      throw ParserError(err);
    }

    if (!strptime(date_b.c_str(), "%Y-%m-%dT%H:%M", &tm2)) {
      ErrorContext err;
      err.Errc = ecINVALID_DATETIME_FORMAT;
      err.Arg = 2;
      err.Type1 = a_pArg[1]->GetType();
      err.Type2 = 's';
      throw ParserError(err);
    }

    int total_days1 = rata_die(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    int total_days2 = rata_die(tm2.tm_year + 1900, tm2.tm_mon + 1, tm2.tm_mday);
    int daysdiff = total_days2 - total_days1;

    float_type initial_hours = tm.tm_hour + tm.tm_min/60.0;
    float_type final_hours = tm2.tm_hour + tm2.tm_min/60.0;
    float_type hoursdiff = daysdiff * 24 + (final_hours - initial_hours);

    // Should be rounded with precision 2. (Ex: 3.67)
    *ret = (std::round(hoursdiff * 100)) / 100;
  }

  const char_type* FunHoursDiff::GetDesc() const
  {
    return _T("hoursdiff(a,b) - Returns the difference in hours between two dates.");
  }

  IToken* FunHoursDiff::Clone() const
  {
    return new FunHoursDiff(*this);
  }

  //------------------------------------------------------------------------------
  //
  // class FunCurrentDate
  //
  //------------------------------------------------------------------------------

  FunCurrentDate::FunCurrentDate()
    :ICallback(cmFUNC, _T("current_date"), -1)
  {}
  //------------------------------------------------------------------------------
  /** \brief Returns the current date with format yyyy-mm-dd.
      \param a_pArg Pointer to an array of Values
      \param a_iArgc Number of values stored in a_pArg
  */
  void FunCurrentDate::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc)
  {
    if (a_iArgc != 0)
        throw ParserError(ErrorContext(ecTOO_MANY_PARAMS, GetExprPos(), GetIdent()));

    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);

    *ret = std::to_string(now->tm_year + 1900) + "-" + std::to_string(now->tm_mon + 1) + "-" + std::to_string(now->tm_mday);
  }

  //------------------------------------------------------------------------------
  const char_type* FunCurrentDate::GetDesc() const
  {
    return _T("current_date() - Returns the current date with format yyyy-mm-dd.");
  }

  //------------------------------------------------------------------------------
  IToken* FunCurrentDate::Clone() const
  {
    return new FunCurrentDate(*this);
  }

  //------------------------------------------------------------------------------
  //
  // class FunTimeDiff
  //
  //------------------------------------------------------------------------------

  //FunTimeDiff::FunTimeDiff()
  //  :ICallback(cmFUNC, _T("timediff"), -1)
  //{}

  ////------------------------------------------------------------------------------
  ///** \brief Returns the minimum value of all values.
  //    \param a_pArg Pointer to an array of Values
  //    \param a_iArgc Number of values stored in a_pArg
  //*/
  //void FunTimeDiff::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc)
  //{
  //  if (a_iArgc != 2)
  //      throw ParserError(ErrorContext(ecTOO_FEW_PARAMS, GetExprPos(), GetIdent()));

  //  string_type date_a = a_pArg[0]->GetString();
  //  string_type date_b = a_pArg[1]->GetString();

  //  struct tm tm, tm2;
  //  //std::string s("2011-10-05T22:26:12-04:00");
  //  //std::string s2("2011-10-10T21:26:12-04:00");
  //  if (strptime(date_a.c_str(), "%Y-%m-%dT%H:%M:%S%z", &tm)) {
  //    std::cout << "date is valid" << std::endl;
  //    std::cout << "day " << tm.tm_mday << std::endl;
  //    std::cout << "month " << (tm.tm_mon + 1) << std::endl;
  //    std::cout << "year " << (tm.tm_year + 1900) << std::endl;
  //    std::cout << "hour " << tm.tm_hour << std::endl;
  //    std::cout << "minutes " << tm.tm_min << std::endl;
  //  } else {
  //    std::cout << "date is invalid" << std::endl;
  //  }

  //  if (strptime(date_b.c_str(), "%Y-%m-%dT%H:%M:%S%z", &tm2)) {
  //    std::cout << "date is valid" << std::endl;
  //    std::cout << "day " << tm2.tm_mday << std::endl;
  //    std::cout << "month " << (tm2.tm_mon + 1) << std::endl;
  //    std::cout << "year " << (tm2.tm_year + 1900) << std::endl;
  //    std::cout << "hour " << tm2.tm_hour << std::endl;
  //    std::cout << "minutes " << tm2.tm_min << std::endl;
  //  } else {
  //    std::cout << "date is invalid" << std::endl;
  //  }

  //  std::time_t t = std::time(0);   // get time now
  //  std::tm* now = std::localtime(&t);
  //  std::cout << (now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-' <<  now->tm_mday << "\n";

  //  // Arithmetic time values.
  //  // On a posix system, these are seconds since 1970-01-01 00:00:00 UTC
  //  std::time_t time1 = std::mktime(&tm);
  //  std::time_t time2 = std::mktime(&tm2);

  //  // Divide by the number of seconds in a day
  //  const int seconds_per_day = 60*60*24;
  //  std::time_t difference = (time1 - time2) / seconds_per_day;

  //  // To be fully portable, we shouldn't assume that these are Unix time;
  //  // instead, we should use "difftime" to give the difference in seconds:
  //  double portable_difference = std::difftime(time1, time2) / seconds_per_day;

  //  //for (int i=0; i<a_iArgc; ++i)
  //  //{
  //  //  switch(a_pArg[i]->GetType())
  //  //  {
  //  //  case 'f':
  //  //  case 'i': avg += a_pArg[i]->GetFloat();   break;
  //  //  default:
  //  //    {
  //  //      ErrorContext err;
  //  //      err.Errc = ecTYPE_CONFLICT_FUN;
  //  //      err.Arg = i+1;
  //  //      err.Type1 = a_pArg[i]->GetType();
  //  //      err.Type2 = 'f';
  //  //      throw ParserError(err);
  //  //    }
  //  //  }
  //  //}
  //  //    avg = avg/a_iArgc;

  //  *ret = abs(floor(portable_difference));
  //}

  ////------------------------------------------------------------------------------
  //const char_type* FunTimeDiff::GetDesc() const
  //{
  //  return _T("timediff(a,b) - Returns the difference in days between two dates.");
  //}

  ////------------------------------------------------------------------------------
  //IToken* FunTimeDiff::Clone() const
  //{
  //  return new FunTimeDiff(*this);
  //}

MUP_NAMESPACE_END
