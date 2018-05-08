/** \example example.cpp
    This is example code showing you how to use muparserx.

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

//--- other includes ------------------------------------------------------------------------------
#include "timer.h"

using namespace std;
using namespace mup;

#if defined(CREATE_LEAKAGE_REPORT)

// Dumping memory leaks in the destructor of the static guard
// guarantees i won't get false positives from the ParserErrorMsg 
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
int CheckKeywords(const char_type *a_szLine, ParserXBase &a_Parser)
{
  string_type sLine(a_szLine);

  if (sLine==_T("quit"))
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

  // Create an array variable
  Value arr1(3, 0);
  arr1.At(0) = (float_type)1.0;
  arr1.At(1) = (float_type)2.0;
  arr1.At(2) = (float_type)3.0;

  Value arr2(3, 0);
  arr2.At(0) = (float_type)4.0;
  arr2.At(1) = (float_type)3.0;
  arr2.At(2) = (float_type)2.0;

  Value arr3(4, 0);
  arr3.At(0) = (float_type)1.0;
  arr3.At(1) = (float_type)2.0;
  arr3.At(2) = (float_type)3.0;
  arr3.At(3) = (float_type)4.0;

  Value arr4(3, 0);
  arr4.At(0) = (float_type)4.0;
  arr4.At(1) = false;
  arr4.At(2) = _T("hallo");

  // Create a 3x3 matrix with zero elements
  Value m1(3, 3, 0);
  m1.At(0, 0) = 1.0;
  m1.At(1, 1) = 1.0;
  m1.At(2, 2) = 1.0;

  Value m2(3, 3, 0);
  m2.At(0, 0) = 1.0;
  m2.At(0, 1) = 2.0;
  m2.At(0, 2) = 3.0;
  m2.At(1, 0) = 4.0;
  m2.At(1, 1) = 5.0;
  m2.At(1, 2) = 6.0;
  m2.At(2, 0) = 7.0;
  m2.At(2, 1) = 8.0;
  m2.At(2, 2) = 9.0;

  Value val[5];
  val[0] = (float_type)1.1;
  val[1] = 1.0;
  val[2] = false;
  val[3] = _T("Hello");
  val[4] = _T("World");

  Value fVal[3];
  fVal[0] = 1;
  fVal[1] = (float_type)2.22;
  fVal[2] = (float_type)3.33;

  Value sVal[3];
  sVal[0] = _T("hello");
  sVal[1] = _T("world");
  sVal[2] = _T("test");

  Value cVal[3];
  cVal[0] = mup::cmplx_type(1, 1);
  cVal[1] = mup::cmplx_type(2, 2);
  cVal[2] = mup::cmplx_type(3, 3);

  Value ans;
  parser.DefineVar(_T("ans"), Variable(&ans));

  // some tests for vectors
  parser.DefineVar(_T("va"), Variable(&arr1));
  parser.DefineVar(_T("vb"), Variable(&arr2));
  parser.DefineVar(_T("vc"), Variable(&arr3));
  parser.DefineVar(_T("vd"), Variable(&arr4));
  parser.DefineVar(_T("m1"), Variable(&m1));
  parser.DefineVar(_T("m2"), Variable(&m2));

  parser.DefineVar(_T("a"),  Variable(&fVal[0]));
  parser.DefineVar(_T("b"),  Variable(&fVal[1]));
  parser.DefineVar(_T("c"),  Variable(&fVal[2]));

  parser.DefineVar(_T("ca"), Variable(&cVal[0]));
  parser.DefineVar(_T("cb"), Variable(&cVal[1]));
  parser.DefineVar(_T("cc"), Variable(&cVal[2]));

  parser.DefineVar(_T("sa"), Variable(&sVal[0]));
  parser.DefineVar(_T("sb"), Variable(&sVal[1]));
 
  parser.EnableAutoCreateVar(true);

  Value x = 1.0;
  Value y = std::complex<double>(0, 1);
  parser.DefineVar(_T("x"), Variable(&x));
  parser.DefineVar(_T("y"), Variable(&y));

  for(;;)
  {
    try
    {
      console() << sPrompt;

      string_type sLine;
      std::getline(mup::console_in(), sLine);

      switch(CheckKeywords(sLine.c_str(), parser)) 
      {
      case  0: break;
      case  1: continue;
      case -1: return;
      }
    
      parser.SetExpr(sLine);

      // The returned result is of type Value, value is a Variant like
      // type that can be either a boolean an integer or a floating point value
      ans = parser.Eval();
      {
        // Value supports C++ streaming like this:
        console() << _T("Result (type: '") << ans.GetType() <<  _T("'):\n");
        console() << _T("ans = ") << ans << _T("\n");
/*
        // Or if you need the specific type use this:
        switch (ans.GetType())
        {
        case 's': { std::string s = ans.GetString();               console() << s << " (string)"  << "\n"; } break;
        case 'i': { int i = ans.GetInteger();                      console() << i << " (int)"     << "\n"; } break;
        case 'f': { float_type f = ans.GetFloat();                 console() << f << " (float)"   << "\n"; } break;
        case 'c': { std::complex<float_type> c = ans.GetComplex(); console() << c << " (complex)" << "\n"; } break;
        case 'b': break;
        }
*/
      }
    }
    catch(ParserError &e)
    {
      if (e.GetPos()!=-1)
      {
        string_type sMarker;
        sMarker.insert(0, sPrompt.size() + e.GetPos(), ' ');
        sMarker += _T("^\n");
        console() << sMarker;
      }

      console() << e.GetMsg() << _T(" (Errc: ") << std::dec << e.GetCode() << _T(")") << _T("\n\n");

      //if (e.GetContext().Ident.length()) 
      //  console() << _T("Ident.: ") << e.GetContext().Ident << _T("\n");

      //if (e.GetToken().length()) 
      //  console() << _T("Token: \"") << e.GetToken() << _T("\"\n");
    } // try / catch
  } // for (;;)
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

  try
  {
    Calc();
  }
  catch(ParserError &e)
  {
    // Only erros raised during the initialization will end up here
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

