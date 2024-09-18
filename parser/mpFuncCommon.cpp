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
#include <time.h>

#include "mpValue.h"
#include "mpParserBase.h"

#define ONE_DAY (24 * 60 * 60)

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
  /** \brief Returns the average value of all values.
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
  // Mask
  //
  //------------------------------------------------------------------------------

  static string_type to_string(long_double_type number) {
    std::string string_number = std::to_string (number);
    int offset = 1;
    if (string_number.find_last_not_of('0') == string_number.find('.')) {
      offset = 0;
    }
    string_number.erase(string_number.find_last_not_of('0') + offset, std::string::npos);
    return string_number;
  }

  static string_type apply_mask(string_type mask, string_type number_string) {
    for (int i = mask.length() - 1; i >= 0; i--) {
      if (mask[i] == '0' && number_string.length() > 0) {
        mask[i] = number_string.back();
        number_string.pop_back();
      }
    }

    if (number_string.length() > 0) {
      mask.insert(0, number_string);
    }

    return mask;
  }

  FunMask::FunMask()
    :ICallback(cmFUNC, _T("mask"), -1)
  {}

  //------------------------------------------------------------------------------
  FunMask::~FunMask()
  {}

  //------------------------------------------------------------------------------
  /** \brief Returns the application of a mask into a integer. */
  void FunMask::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc)
  {
    if (a_iArgc < 2) {
      throw ParserError(ErrorContext(ecTOO_FEW_PARAMS, GetExprPos(), GetIdent()));
    } else if (a_iArgc > 2) {
      throw ParserError(ErrorContext(ecTOO_MANY_PARAMS, GetExprPos(), GetIdent()));
    }

    string_type mask = a_pArg[0]->GetString();
    long_double_type number = a_pArg[1]->GetFloat();

    *ret = apply_mask(mask, to_string(std::round(number)));
  }

  //------------------------------------------------------------------------------
  const char_type* FunMask::GetDesc() const
  {
    return _T("mask(a, b) - Returns the application of the a mask into the b integer.");
  }

  //------------------------------------------------------------------------------
  IToken* FunMask::Clone() const
  {
    return new FunMask(*this);
  }

  //------------------------------------------------------------------------------
  //                                                                             |
  //            Below we have the section related to Date functions              |
  //                                                                             |
  //------------------------------------------------------------------------------

  //------------------------------------------------------------------------------
  //                                                                             |
  //                         Date auxiliar functions!                            |
  //                                                                             |
  //------------------------------------------------------------------------------

  // https://en.wikipedia.org/wiki/Rata_Die
  int rata_die (int y, int m, int d) { /* Rata Die day one is 0001-01-01 */
    if (m < 3)
      y--, m += 12;
    return 365*y + y/4 - y/100 + y/400 + (153*m - 457)/5 + d - 306;
  }

  string_type format_date (struct tm time, bool is_date_time) {
    string_type year  = std::to_string(time.tm_year + 1900);
    string_type month = std::to_string(time.tm_mon + 1);
    string_type day   = std::to_string(time.tm_mday);
    string_type hours = std::to_string(time.tm_hour);
    string_type min   = std::to_string(time.tm_min);

    month = month.length() > 1 ? month : '0' + month;
    day = day.length() > 1 ? day : '0' + day;
    hours = hours.length() > 1 ? hours : '0' + hours;
    min = min.length() > 1 ? min : '0' + min;

    if (is_date_time) {
      return(year + "-" + month + "-" + day + "T" + hours + ":" + min);
    } else {
      return(year + "-" + month + "-" + day);
    }
  }

  void add_days (struct tm* date, float_type days) {
    // Avoid mismatch between winter and summer time on mktime() conversion
    date->tm_isdst = -1;

    // Seconds since start of epoch
    time_t new_day = mktime(date) + (days * ONE_DAY);

    // Update caller's date
    // localtime is used because mktime converts from localtime to UTC and we need to convert it back
    *date = *localtime(&new_day);
  }

  void raise_error (EErrorCodes error, int position, const ptr_val_type *arguments) {
    ErrorContext err;
    err.Errc = error;
    err.Arg = position;
    err.Type1 = arguments[position - 1]->GetType();
    err.Type2 = 's';
    throw ParserError(err);
  }

  string_type localized_weekday(int week_day, const ptr_val_type *a_pArg) {
    string_type locale = a_pArg[1]->GetString();
    string_type ret = "";
    string_type localized_weekdays[8][7] = {
      {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"},
      {"Søndag", "Mandag", "Tirsdag", "Onsdag", "Torsdag", "Fredag", "Lørdag"},
      {"Domingo", "Segunda-Feira", "Terça-feira", "Quarta-feira", "Quinta-feira", "Sexta-feira", "Sábado"},
      {"Domingo", "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sabado"},
      {"Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi"},
      {"Sonntag", "Montag", "Dienstag", "Mittwoch", "Donnerstag", "Freitag", "Samstag"},
      {"星期天", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"},
      {"วันอาทิตย์", "วันจันทร์", "วันอังคาร", "วันพุธ", "วันพฤหัสบดี", "วันศุกร์", "วันเสาร์"}
    };
    string_type locales[8] = {"en", "nb", "pt-BR", "es-ES", "fr-FR", "de-DE", "zh-CN", "th-TH"};

    for (int i = 0; i < 8; i++) {
      if(locale == locales[i]) {
        ret = localized_weekdays[i][week_day];
      }
    }

    if(ret == ""){
      raise_error(ecUKNOWN_LOCALE, 2, a_pArg);
    }

    return ret;
  }

  //------------------------------------------------------------------------------
  //
  // class FunDaysDiff
  //
  //------------------------------------------------------------------------------

  FunDaysDiff::FunDaysDiff()
    :ICallback(cmFUNC, _T("daysdiff"), -1)
  {}

  //------------------------------------------------------------------------------
  /** \brief Returns the number of days between two dates.
      \param a_pArg Pointer to an array of Values
      \param a_iArgc Number of values stored in a_pArg
  */
  void FunDaysDiff::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc)
  {
    if (a_iArgc < 2) {
      throw ParserError(ErrorContext(ecTOO_FEW_PARAMS, GetExprPos(), GetIdent()));
    } else if (a_iArgc > 2) {
      throw ParserError(ErrorContext(ecTOO_MANY_PARAMS, GetExprPos(), GetIdent()));
    }

    string_type date_a = a_pArg[0]->GetString();
    string_type date_b = a_pArg[1]->GetString();

    struct tm tm, tm2;
    //http://man7.org/linux/man-pages/man3/strptime.3.html
    if (!strptime(date_a.c_str(), "%Y-%m-%d", &tm)) {
      raise_error(ecINVALID_DATE_FORMAT, 1, a_pArg);
    }
    if (!strptime(date_b.c_str(), "%Y-%m-%d", &tm2)) {
      raise_error(ecINVALID_DATE_FORMAT, 2, a_pArg);
    }

    int total_days1 = rata_die(tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    int total_days2 = rata_die(tm2.tm_year + 1900, tm2.tm_mon + 1, tm2.tm_mday);

    *ret = abs(total_days1 - total_days2);
  }

  const char_type* FunDaysDiff::GetDesc() const
  {
    return _T("daysdiff(a,b) - Returns the difference in days between two dates.");
  }

  IToken* FunDaysDiff::Clone() const
  {
    return new FunDaysDiff(*this);
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
    if (a_iArgc < 2) {
      throw ParserError(ErrorContext(ecTOO_FEW_PARAMS, GetExprPos(), GetIdent()));
    } else if (a_iArgc > 2) {
      throw ParserError(ErrorContext(ecTOO_MANY_PARAMS, GetExprPos(), GetIdent()));
    }

    string_type date_a = a_pArg[0]->GetString();
    string_type date_b = a_pArg[1]->GetString();

    // Matches exactly: "yyyy-mm-dd"
    std::regex basic_date ("^\\d{4}-\\d{1,2}-\\d{1,2}$");
    if (std::regex_match (date_a, basic_date) && std::regex_match (date_b, basic_date)) {
      date_a += "T00:00";
      date_b += "T00:00";
    } else if (std::regex_match (date_a, basic_date) || std::regex_match (date_b, basic_date)) {
      raise_error(ecDATE_AND_DATETIME, 1, a_pArg);
    }

    // http://man7.org/linux/man-pages/man3/strptime.3.html
    struct tm tm, tm2;
    if (!strptime(date_a.c_str(), "%Y-%m-%dT%H:%M", &tm)) {
      raise_error(ecINVALID_DATETIME_FORMAT, 1, a_pArg);
    }
    if (!strptime(date_b.c_str(), "%Y-%m-%dT%H:%M", &tm2)) {
      raise_error(ecINVALID_DATETIME_FORMAT, 2, a_pArg);
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

    (void)*a_pArg;
    std::time_t t = std::time(0); // get time now
    std::tm now = *std::localtime(&t);

    *ret = format_date(now, false);
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
  // class FunAddDays
  //
  //------------------------------------------------------------------------------

  FunAddDays::FunAddDays()
    :ICallback(cmFUNC, _T("add_days"), -1)
  {}
  //------------------------------------------------------------------------------
  /** \brief Returns the sum of a date/date_time with with an integer value representing the days.
      \param a_pArg Pointer to an array of Values
      \param a_iArgc Number of values stored in a_pArg
  */
  void FunAddDays::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc)
  {
    if (a_iArgc < 2) {
      throw ParserError(ErrorContext(ecTOO_FEW_PARAMS, GetExprPos(), GetIdent()));
    } else if (a_iArgc > 2) {
      throw ParserError(ErrorContext(ecTOO_MANY_PARAMS, GetExprPos(), GetIdent()));
    }

    string_type date = a_pArg[0]->GetString();
    float_type days = a_pArg[1]->GetFloat(); // Accept both integer or float numbers! :D

    std::regex date_regex ("^\\d{4}-\\d{1,2}-\\d{1,2}$"); // "yyyy-mm-dd"
    std::regex date_time_regex ("^\\d{4}-\\d{1,2}-\\d{1,2}T\\d{1,2}:\\d{1,2}$"); // "yyyy-mm-ddTHH:MM"
    bool is_date_time = false;

    // http://man7.org/linux/man-pages/man3/strptime.3.html
    struct tm time = {0};
    if (std::regex_match (date, date_regex)) {
      if (!strptime(date.c_str(), "%Y-%m-%d", &time)) {
        raise_error(ecADD_HOURS_DATE, 1, a_pArg);
      }
      is_date_time = false;
    } else if (std::regex_match (date, date_time_regex)) {
      if (!strptime(date.c_str(), "%Y-%m-%dT%H:%M", &time)) {
        raise_error(ecADD_HOURS_DATETIME, 1, a_pArg);
      }
      is_date_time = true;
    } else {
      raise_error(ecADD_HOURS, 1, a_pArg);
    }

    add_days(&time, days);

    *ret = format_date(time, is_date_time);
  }

  //------------------------------------------------------------------------------
  const char_type* FunAddDays::GetDesc() const
  {
    return _T("add_days() - Returns sum of a date/date_time with a days quantity.");
  }

  //------------------------------------------------------------------------------
  IToken* FunAddDays::Clone() const
  {
    return new FunAddDays(*this);
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

  //------------------------------------------------------------------------------
  //                                                                             |
  //            Below we have the section related to Time functions              |
  //                                                                             |
  //------------------------------------------------------------------------------

  FunTimeDiff::FunTimeDiff()
    :ICallback(cmFUNC, _T("timediff"), -1)
  {}

  void FunTimeDiff::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc)
  {
    if (a_iArgc < 2) {
      throw ParserError(ErrorContext(ecTOO_FEW_PARAMS, GetExprPos(), GetIdent()));
    } else if (a_iArgc > 2) {
      throw ParserError(ErrorContext(ecTOO_MANY_PARAMS, GetExprPos(), GetIdent()));
    }

    string_type time_a = a_pArg[0]->GetString();
    string_type time_b = a_pArg[1]->GetString();

    // http://man7.org/linux/man-pages/man3/strptime.3.html
    struct tm tm, tm2;
    if (!strptime(time_a.c_str(), "%T", &tm)) {
      raise_error(ecINVALID_TIME_FORMAT, 1, a_pArg);
    }
    if (!strptime(time_b.c_str(), "%T", &tm2)) {
      raise_error(ecINVALID_TIME_FORMAT, 2, a_pArg);
    }

    float_type startx = tm.tm_sec / 60.0 / 60.0 + tm.tm_min / 60.0 + tm.tm_hour;
    float_type endx = tm2.tm_sec / 60.0 / 60.0 + tm2.tm_min / 60.0 + tm2.tm_hour;

    float_type timediff = endx - startx;

    if (timediff < 0)
      timediff = timediff + 24;

    // Should be rounded with precision 2. (Ex: 3.67)
    *ret = (std::round(timediff * 100)) / 100;
  }

  ////------------------------------------------------------------------------------
  const char_type* FunTimeDiff::GetDesc() const
  {
    return _T("timediff(a,b) - Returns the difference in hours between two times.");
  }

  ////------------------------------------------------------------------------------
  IToken* FunTimeDiff::Clone() const
  {
    return new FunTimeDiff(*this);
  }

  //------------------------------------------------------------------------------
  //                                                                             |
  //            Functions for regex matching                                     |
  //            Usage: regex("string", "regex")                                  |
  //                                                                             |
  //------------------------------------------------------------------------------

  FunRegex::FunRegex()
    :ICallback(cmFUNC, _T("regex"), -1)
  {}

  std::vector<std::vector<std::string>> capture_regex_groups(const std::string& input, const std::string& pattern) {
    std::vector<std::vector<std::string>> captured_groups;
    std::smatch match;
    std::regex re(pattern);
    std::string::const_iterator search_start(input.cbegin());

    while (std::regex_search(search_start, input.cend(), match, re)) {
        std::vector<std::string> groups;
        for (size_t i = 1; i < match.size(); ++i) {
            groups.push_back(match[i].str());
        }
        captured_groups.push_back(groups);
        search_start = match.suffix().first;
    }

    return captured_groups;
  }

  void FunRegex::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc)
  {
    if (a_iArgc < 2) {
      throw ParserError(ErrorContext(ecTOO_FEW_PARAMS, GetExprPos(), GetIdent()));
    } else if (a_iArgc > 2) {
      throw ParserError(ErrorContext(ecTOO_MANY_PARAMS, GetExprPos(), GetIdent()));
    }

     string_type input = a_pArg[0]->GetString();
     string_type pattern = a_pArg[1]->GetString();

     auto captured_groups = capture_regex_groups(input, pattern);

     if (captured_groups.size() == 0 || captured_groups[0].size() == 0) {
       *ret = (string_type) "";
     } else {
       *ret = (string_type) captured_groups[0][0];
     }
  }

  ////------------------------------------------------------------------------------
  const char_type* FunRegex::GetDesc() const
  {
    return _T("regex(a,b) - Returns the first match of a regex pattern.");
  }

  ////------------------------------------------------------------------------------
  IToken* FunRegex::Clone() const
  {
    return new FunRegex(*this);
  }

  //------------------------------------------------------------------------------
  //                                                                             |
  //            Function return the week of year of a date                       |
  //            Usage: weekyear("2022-04-20")                                    |
  //                                                                             |
  //------------------------------------------------------------------------------

  FunWeekYear::FunWeekYear()
    :ICallback(cmFUNC, _T("weekyear"), -1)
  {}

  void FunWeekYear::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc)
  {
    if (a_iArgc < 1) {
      throw ParserError(ErrorContext(ecTOO_FEW_PARAMS, GetExprPos(), GetIdent()));
    } else if (a_iArgc > 1) {
      throw ParserError(ErrorContext(ecTOO_MANY_PARAMS, GetExprPos(), GetIdent()));
    }

    string_type date_time = a_pArg[0]->GetString();

    struct tm date;
    if (!strptime(date_time.c_str(), "%Y-%m-%d", &date)) {
      raise_error(ecINVALID_DATE_FORMAT, 1, a_pArg);
    }

    int year = date.tm_year + 1900; // tm_year is the number of years since 1900

    // Get ordinal day of the year
    int day_of_year = date.tm_yday + 1; // tm_yday is the number of days since January 1st

    // Get weekday number (0 is Sunday)
    int weekday = date.tm_wday;

    // Calculate week number
    int week_number = (day_of_year - weekday + 10) / 7;

    // Check if week belongs to previous year
    if (week_number == 0) {
        year--;
        week_number = 52;
        if (std::tm{0,0,0,1,0,year-1900}.tm_wday < 4) { // January 1st of the previous year is before Thursday
            week_number = 53;
        }
    }

    // Check if week belongs to following year
    if (week_number == 53) {
        if (std::tm{0,0,0,1,0,year+1-1900}.tm_wday >= 4) { // January 1st of the following year is on or after Thursday
            week_number = 1;
        }
    }

    *ret = week_number;
  }

  ////------------------------------------------------------------------------------
  const char_type* FunWeekYear::GetDesc() const
  {
    return _T("weekyear(date) - Returns the week number of the year.");
  }

  ////------------------------------------------------------------------------------
  IToken* FunWeekYear::Clone() const
  {
    return new FunWeekYear(*this);
  }

  //------------------------------------------------------------------------------
  //                                                                             |
  //            Function return the week day of a date                           |
  //            Usage: weekday("2022-04-20")                                     |
  //            Optional locale: weekday("2022-04-20", "en")                     |
  //                                                                             |
  //------------------------------------------------------------------------------

  FunWeekDay::FunWeekDay()
    :ICallback(cmFUNC, _T("weekday"), -1)
  {}

  void FunWeekDay::Eval(ptr_val_type &ret, const ptr_val_type *a_pArg, int a_iArgc)
  {
    if (a_iArgc < 1) {
      throw ParserError(ErrorContext(ecTOO_FEW_PARAMS, GetExprPos(), GetIdent()));
    } else if (a_iArgc > 2) {
      throw ParserError(ErrorContext(ecTOO_MANY_PARAMS, GetExprPos(), GetIdent()));
    }

    string_type date_time = a_pArg[0]->GetString();

    struct tm date;
    if (!strptime(date_time.c_str(), "%Y-%m-%d", &date)) {
      raise_error(ecINVALID_DATETIME_FORMAT, 1, a_pArg);
    }

    bool has_locale = false;
    if (a_iArgc == 2) {
      has_locale = true;
    }

    int week_day = date.tm_wday;

    if(has_locale) {
      *ret = localized_weekday(week_day, a_pArg);
    } else {
      *ret = week_day;
    }
  }

  ////------------------------------------------------------------------------------
  const char_type* FunWeekDay::GetDesc() const
  {
    return _T("weekday(date) - Returns the week day of the date.");
  }

  ////------------------------------------------------------------------------------
  IToken* FunWeekDay::Clone() const
  {
    return new FunWeekDay(*this);
  }

MUP_NAMESPACE_END
