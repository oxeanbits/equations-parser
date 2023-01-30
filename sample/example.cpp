/** \example example.cpp
    This is a simple version of muparserx that works as follows...
    Input: Equation(s)
    Output: Result type and value of the equation(s)

<pre>
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
</pre>
*/

/** \brief This macro will enable mathematical constants like M_PI. */
#define _USE_MATH_DEFINES

/** \brief Needed to ensure successfull compilation on Unicode systems with MinGW. */
#undef __STRICT_ANSI__

//--- Standard include ------------------------------------------------------
#if defined(_WIN32)
  // Memory leak dumping
  #if defined(_DEBUG)
    #define _CRTDBG_MAP_ALLOC
    #include <stdlib.h>
    #include <crtdbg.h>
    #define CREATE_LEAKAGE_REPORT
  #endif

  // Needed for windows console UTF-8 support
  #include <fcntl.h>
  #include <io.h>
#endif

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <limits>
#include <string>
#include <typeinfo>

//--- muparserx framework -------------------------------------------------------------------------
#include "mpParser.h"
#include "mpDefines.h"
#include "equationsParser.h"

//--- other includes ------------------------------------------------------------------------------
#include "timer.h"

using namespace std;
using namespace mup;

#if defined(CREATE_LEAKAGE_REPORT)

// Dumping memory leaks in the destructor of the static guard
// guarantees I won't get false positives from the ParserErrorMsg
// class wich is a singleton with a static instance.
struct DumpLeaks
{
 ~DumpLeaks()
  {
    _CrtDumpMemoryLeaks();
  }
} static LeakDumper;

#endif

const string_type sPrompt = _T("parsec> ");

//---------------------------------------------------------------------------
/** \brief Check for external keywords.
*/
int CheckKeywords(const char_type *a_szLine)
{
  string_type sLine(a_szLine);

  if (sLine==_T("exit"))
  {
    return -1;
  }

  return 0;
}

//---------------------------------------------------------------------------
void Calc()
{
  ParserX  parser(pckALL_NON_COMPLEX);
  //  ParserX  parser(pckALL_COMPLEX);

  Value ans;
  parser.DefineVar(_T("ans"), Variable(&ans));
  // parser.DefineFun(new MySine);


  /*
   Implicit creation of variables refers to the creation of parser variables
   at parser runtime. With this feature you can create variables on the fly
   without any additional client code. Since this is usefull only for applications requiring
   direct user interaction it is turned off by default.
   In order to use it you have to activate it first by calling the EnableAutoCreateVar member function
   Reference: muparserx website (beltoforion)
  */

  // parser.EnableAutoCreateVar(true);

    for(;;)
    {
    try
    {
      console() << sPrompt;

      string_type sLine;
      std::getline(mup::console_in(), sLine);

      switch(CheckKeywords(sLine.c_str()))
      {
      case  0: break;
      case -1: return;
      }

      parser.SetExpr(sLine);

      // The returned result is of type Value, value is a Variant like
      // type that can be either a boolean, an integer or a floating point value
      ans = parser.Eval();

      {
        // Value supports C++ streaming like this:
        console() << _T("Result (type: '") << ans.GetType() <<  _T("'):\n");
        console() << _T("ans = ") << ans << _T("\n");
      }

    }
    catch(ParserError &e)
    {
      if (e.GetPos()!=-1)
      {
        string_type sMarker;
        sMarker.insert(0, sPrompt.size() + e.GetPos(), ' ');
        console() << sMarker;
      }

      console() << e.GetMsg() << _T(" (Errc: ") << std::dec << e.GetCode() << _T(")") << _T("\n\n");
    } // try / catch
  } // for (;;)
} // Calc

void EquationsParserSample() {
  console() << _T(EquationsParser::CalcJson("sin(0.57) * 33")) << endl;
  console() << _T(EquationsParser::Calc("sin(0.57) * 33")) << endl;

  vector<string> equations = {"5*7", "40+2"};
  vector<string> results;
  EquationsParser::CalcArray(equations, results);
  for (string r : results) {
    console() << _T(r) << endl;
  }
}

//---------------------------------------------------------------------------
int main(int /*argc*/, char** /*argv*/)
{
#if defined(_UNICODE)

  #if _MSC_VER
    // Set console to utf-8 mode, if this is not done language specific
    // characters will be rendered incorrectly
    if (_setmode(_fileno(stdout), _O_U8TEXT)==-1)
      throw std::runtime_error("Can't set \"stdout\" to UTF-8");
  #endif

  //// Internationalization requires UNICODE as translations do contain non ASCII
  //// Characters.
  //ParserX::ResetErrorMessageProvider(new mup::ParserMessageProviderGerman);
#endif

  EquationsParserSample();

  try
  {
    Calc();
  }
  catch(ParserError &e)
  {
    // Only errors raised during the initialization will end up here
    // expression related errors are treated in Calc()
    console() << _T("Initialization error:  ") << e.GetMsg() << endl;
  }
  catch(std::runtime_error &)
  {
    console() << _T("aborting...") << endl;
  }

#ifdef MUP_LEAKAGE_REPORT
  IToken::LeakageReport();
#endif

  return 0;
}
