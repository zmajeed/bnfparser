#ifndef BNFPARSER_NO_ACTIONS_LEXER_H
#define BNFPARSER_NO_ACTIONS_LEXER_H
// lexer.h

/*
MIT License

Copyright (c) 2024 Zartaj Majeed

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "bnfparser.bison.h"

#include "bnfparser_guard_flexlexer.h"

namespace bnfparser {
using namespace std;

class Lexer: public yyFlexLexer {
public:

// can only declare here since flex generates the implementation
// dummy parameter because virtual int yyFlexLexer::yylex() cannot be overridden due to conflicting return type
// would not be a problem if yylex was being generated with some parameters
  BnfParser::symbol_type yylex(location&);

  Lexer() = default;

  explicit Lexer(istream* yyin_arg): yyFlexLexer(yyin_arg) {}

private:

// fix gcc-13 warning -Woverloaded-virtual that virtual int BnfParserFlexLexer::yylex() was hidden
  using yyFlexLexer::yylex;
  
};

}

#endif

