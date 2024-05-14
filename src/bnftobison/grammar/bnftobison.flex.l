/* bnftobison/bnftobison.flex.l */

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

/* flex -Cfe -o bnftobison.flex.cpp bnftobison.flex.l */
/* g++ -O0 -Wall -Wextra -Werror -std=c++23 -o bnftobison.lexer bnftobison.flex.cpp */

 // flex directives

 // turn on warnings
%option warn

 // always generate 8bit lexer
%option 8bit

 // C++ lexer class
%option c++

 // custom C++ lexer classname
%option yyclass="Lexer"

 // custom prefix to turn yyFlexLexer into BnfToBisonFlexLexer
%option prefix="BnfToBison"

%option debug

 // disable default rule that echoes unmatched input, gives flex scanner jammed error for any unmatched input
%option nodefault

 // noninteractive lexer has better performance, use with -Cfe
%option never-interactive

%option yylineno

%option noyywrap
 // disable various options for interactive lexer
%option nounput noinput batch

 // flex start conditions ie states
%x RULES
%x RULE_START
%x COMMENT

 // top of generated .cpp file
%{

#include <string>

// bison generated header with C++ namespace and token definitions
#include "bnftobison.bison.h"

#include "lexer/bnftobison_lexer.h"

#undef YY_DECL
// dummy parameter because yyFlexLexer::yylex(void) in static flex C++ header yyFlexLexer.h cannot be overridden, only overloaded
// parameter has no name to avoid unused parameter warning
#define YY_DECL bnftobison::BnfToBison::symbol_type bnftobison::Lexer::yylex(location& loc)

// fix flex error could not convert 0 from int to symbol_type for #define YY_NULL 0
// caused by turning on bison %locations because symbol_type no longer has single int constructor for implicit conversion
#define yyterminate() return symbol_type(YY_NULL, loc)

using namespace std;

using namespace bnftobison;
using token = bnftobison::BnfToBison::token::token_kind_type;

// needed for custom yyterminate
using symbol_type = BnfToBison::symbol_type;

%}

 // flex rules
%%
 // code must be indented

  string headerLine;

  loc.step();

 /* first rule of grammar is assumed to start at beginning of line */
^"<"[-A-Za-z0-9 _/]+">" {
  BEGIN(RULES);
  loc.columns(yyleng);
  return BnfToBison::make_NONTERMINAL(yytext, loc);
}

 /* everything above first rule is header */
. {
  loc.columns(yyleng);
  headerLine += yytext;
}

 /* accumlate and send header line by line */
\n {
  loc.lines();
  headerLine += yytext;
  return BnfToBison::make_HEADER_LINE(headerLine, loc);
}

 /* patterns for rules body of grammar */
<RULES>{

 /* start of rule marks end of previous rule */
 /* put matched start of rule back to match again after returning rule separator token */
  "<"[-A-Za-z0-9 _/]+">"[ ]*::= {
    BEGIN(RULE_START);
    yyless(0);
    return BnfToBison::make_RULE_SEPARATOR(loc);
  }

 /* rematch start of rule that was matched earlier */
  <RULE_START>"<"[-A-Za-z0-9 _/]+">" {
    BEGIN(RULES);
    loc.columns(yyleng);
    return BnfToBison::make_NONTERMINAL(yytext, loc);
  }

 /* match rule assignment operator */
  ::= {
    loc.columns(yyleng);
    return BnfToBison::make_COLON_EQUAL(loc);
  }

 /* assumes multiline string literals are not allowed */
  (?x: ["] ( [^"\n] | \\["] | \\\\ )* ["] ) {
    loc.columns(yyleng);
    return BnfToBison::make_LITERAL(yytext, loc);
  }

 /* assumes typical identifier syntax for grammar token */
  [A-Za-z0-9_]+ {
    loc.columns(yyleng);
    return BnfToBison::make_TOKEN(yytext, loc);
  }

 /* alternative operator */
  "|" {
    loc.columns(yyleng);
    return BnfToBison::make_BAR(loc);
  }

 /* start of optional operator */
  "[" {
    loc.columns(yyleng);
    return BnfToBison::make_LEFT_BRACKET(loc);
  }

 /* end of optional operator */
  "]" {
    loc.columns(yyleng);
    return BnfToBison::make_RIGHT_BRACKET(loc);
  }

 /* start of group operator */
  "{" {
    loc.columns(yyleng);
    return BnfToBison::make_LEFT_BRACE(loc);
  }

 /* end of group operator */
  "}" {
    loc.columns(yyleng);
    return BnfToBison::make_RIGHT_BRACE(loc);
  }

 /* repetition operator */
  "..." {
    loc.columns(yyleng);
    return BnfToBison::make_ELLIPSIS(loc);
  }

 /* start of comment to end of line */
  "!!".* {
    loc.columns(yyleng);
    return BnfToBison::make_COMMENT(yytext, loc);
  }

 /* match newlines separately to correctly update line numbers */
  \n {
    loc.lines(yyleng);
  }

 /* whitespace except newline, same as [ \t\v\f\r] but easier to understand */
  ([[:space:]]{-}[\n])+ {
    loc.columns(yyleng);
  }

}

 /* all input must be consumed in every state to prevent flex scanner jammed error */
<*>. {
  loc.columns(yyleng);
  throw BnfToBison::syntax_error(loc, "bad input \""s + yytext + "\""s);
}

