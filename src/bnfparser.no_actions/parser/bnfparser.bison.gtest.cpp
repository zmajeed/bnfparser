// bnfparser.gtest.cpp

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

#include <sstream>
#include <string>

#include <gtest/gtest.h>

#include "lexer/bnfparser_lexer.h"
#include "bnfparser.bison.h"

using namespace std;

using namespace ::testing;

namespace bnfparser::testing {

TEST(BisonParser, test_0) {

  stringstream s("<true literal> ::= TRUE");
  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfParser parser([&lexer](location& loc) -> BnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_1) {

  stringstream s(R"%(<true literal> ::=
  TRUE
| FALSE
| UNKNOWN
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfParser parser([&lexer](location& loc) -> BnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_2) {

  stringstream s("<nested query specification> ::= <left brace> <query specification> <right brace>");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfParser parser([&lexer](location& loc) -> BnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_3) {

  stringstream s(R"%(
<GQL-program> ::=
    <program activity> [ <session close command> ]
  | <session close command>
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfParser parser([&lexer](location& loc) -> BnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_4) {

  stringstream s(R"%(
<session activity> ::=
    <session reset command>...
  | <session set command>... [ <session reset command>... ]
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfParser parser([&lexer](location& loc) -> BnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_5) {

  stringstream s(R"%(
<transaction characteristics> ::=
    <transaction mode 1> [ { <comma> <transaction mode 2> }... ]
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfParser parser([&lexer](location& loc) -> BnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_6) {

  stringstream s(R"%(
<create graph statement> ::=
    CREATE { [ PROPERTY ] GRAPH [ IF NOT EXISTS ]
  | OR REPLACE [ PROPERTY ] GRAPH }
         <catalog graph parent and name> { <open graph type>
  | <of graph type> }
         [ <graph source> ]
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfParser parser([&lexer](location& loc) -> BnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_7) {

  stringstream s(R"%(
<delete statement> ::=
    [ DETACH
  | NODETACH ] DELETE <delete item list>
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfParser parser([&lexer](location& loc) -> BnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_8) {

  stringstream s(R"%(
<exists predicate> ::=
    EXISTS { <left brace> <graph pattern> <right brace>
  | <left paren> <graph pattern> <right paren>
  | <left brace> <match statement block> <right brace>
  | <left paren> <match statement block> <right paren>
  | <nested query specification> }
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfParser parser([&lexer](location& loc) -> BnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_9) {

  stringstream s(R"%(
<transaction activity> ::=
    <start transaction command>
         [ <procedure specification> [ <end transaction command> ] ]
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfParser parser([&lexer](location& loc) -> BnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_10) {

  stringstream s(R"%(
<session set command> ::=
    SESSION SET { <session set schema clause>
  | <session set graph clause>
  | <session set time zone clause>
  | <session set parameter clause> }
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfParser parser([&lexer](location& loc) -> BnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_11) {

  stringstream s(R"%(
<single quoted character representation> ::=
    <character representation>
  | <double single quote>!! See the Syntax Rules.
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfParser parser([&lexer](location& loc) -> BnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_12) {

  stringstream s(R"%(
<double single quote> ::=
    <quote> <quote>
    !! See the Syntax Rules.
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfParser parser([&lexer](location& loc) -> BnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_13) {

  stringstream s(R"%(
<GQL-program> ::=
    <program activity> [ <session close command> ]
  | <session close command>

<program activity> ::=
    <session activity>
  | <transaction activity>
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfParser parser([&lexer](location& loc) -> BnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_14) {

  stringstream s(R"%(

<GQL-program> ::=
    <program activity> [ <session close command> ]
  | <session close command>

<program activity> ::=
    <session activity>
  | <transaction activity>


)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfParser parser([&lexer](location& loc) -> BnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_15) {

  stringstream s(R"%(
<implementation-defined access mode> ::=
    !! See the Syntax Rules.

)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfParser parser([&lexer](location& loc) -> BnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_16) {

  stringstream s(R"%(
<implementation-defined access mode> ::=
    !! See the Syntax Rules.

<rollback command> ::=
    ROLLBACK

)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfParser parser([&lexer](location& loc) -> BnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_17) {

  stringstream s(R"%(
<pre-reserved word> ::=
    
    ABSTRACT

  | AGGREGATE

  | AGGREGATES
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfParser parser([&lexer](location& loc) -> BnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_18) {

  stringstream s(R"%(
<space> ::=
    " "
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfParser parser([&lexer](location& loc) -> BnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_19) {

  stringstream s(R"%(
<right brace> ::=
    "}"
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfParser parser([&lexer](location& loc) -> BnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_20) {

  stringstream s(R"%(
<reverse solidus> ::=
    "\\"

<right brace> ::=
    "}"
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfParser parser([&lexer](location& loc) -> BnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  EXPECT_EQ(parser(), 0);
}

TEST(BisonParser, test_21) {

  stringstream s("<a> := b");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfParser parser([&lexer](location& loc) -> BnfParser::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  EXPECT_NE(parser(), 0);
}

}

