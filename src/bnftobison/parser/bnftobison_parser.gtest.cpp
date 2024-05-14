// bnftobison.gtest.cpp

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
#include <gmock/gmock.h>

#include "lexer/bnftobison_lexer.h"
#include "bnftobison.bison.h"

using namespace std;

using namespace ::testing;

namespace bnftobison::testing {

TEST(BnfToBison, test_0) {

  stringstream s("<true literal> ::= TRUE");
  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 1);
  EXPECT_TRUE(result.contains("true_literal"));
  EXPECT_EQ(result, (map<string, set<vector<string>>>{ {"true_literal", { {"TRUE"} } } }) );
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{ {"true_literal", { {"TRUE"} } } }) ));
}

TEST(BnfToBison, test_1) {

  stringstream s(R"%(<true literal> ::=
  TRUE
| FALSE
| UNKNOWN
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 1);
  EXPECT_TRUE(result.contains("true_literal"));
  EXPECT_EQ(result["true_literal"].size(), 3);
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{ {"true_literal", { {"TRUE"}, {"FALSE"}, {"UNKNOWN"} } } }) ));
}

TEST(BnfToBison, test_2) {

  stringstream s("<nested query specification> ::= <left brace> <query specification> <right brace>");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 1);
  EXPECT_EQ(result["nested_query_specification"].size(), 1);
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{ {"nested_query_specification", { {"left_brace", "query_specification", "right_brace"} } } }) ));
}

TEST(BnfToBison, test_3) {

  stringstream s(R"%(
<GQL-program> ::=
    <program activity> [ <session close command> ]
  | <session close command>
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 1);
  EXPECT_TRUE(result.contains("GQL_program"));
  EXPECT_EQ(result["GQL_program"].size(), 3);
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "GQL_program",
      {
        {"program_activity"},
        {"program_activity", "session_close_command"},
        {"session_close_command"}
      }
    }
  }) ));
}

TEST(BnfToBison, test_4) {

  stringstream s(R"%(
<session activity> ::=
    <session reset command>...
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 2);
  EXPECT_TRUE(result.contains("session_activity"));
  EXPECT_TRUE(result.contains("session_reset_command_list"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "session_activity",
      {
        {"session_reset_command_list"},
      }
    },
    {
      "session_reset_command_list",
      {
        {"session_reset_command"},
        {"session_reset_command_list", "session_reset_command"},
      }
    }
  }) ));
}

TEST(BnfToBison, test_5) {

  stringstream s(R"%(
<session activity> ::=
    <session reset command>...
  | <session set command>...
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 3);
  EXPECT_TRUE(result.contains("session_activity"));
  EXPECT_TRUE(result.contains("session_reset_command_list"));
  EXPECT_TRUE(result.contains("session_set_command_list"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "session_activity",
      {
        {"session_reset_command_list"},
        {"session_set_command_list"},
      }
    },
    {
      "session_reset_command_list",
      {
        {"session_reset_command"},
        {"session_reset_command_list", "session_reset_command"},
      }
    },
    {
      "session_set_command_list",
      {
        {"session_set_command"},
        {"session_set_command_list", "session_set_command"},
      }
    }
  }) ));
}

TEST(BnfToBison, test_6) {

  stringstream s(R"%(
<session activity> ::=
  <session set command>... [ <session reset command>... ]
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 3);
  EXPECT_TRUE(result.contains("session_activity"));
  EXPECT_TRUE(result.contains("session_reset_command_list"));
  EXPECT_TRUE(result.contains("session_set_command_list"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "session_activity",
      {
        {"session_set_command_list"},
        {"session_set_command_list", "session_reset_command_list"},
      }
    },
    {
      "session_set_command_list",
      {
        {"session_set_command"},
        {"session_set_command_list", "session_set_command"},
      }
    },
    {
      "session_reset_command_list",
      {
        {"session_reset_command"},
        {"session_reset_command_list", "session_reset_command"},
      }
    }
  }) ));
}

TEST(BnfToBison, test_7) {

  stringstream s(R"%(
<session activity> ::=
    <session reset command>...
  | <session set command>... [ <session reset command>... ]
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 3);
  EXPECT_TRUE(result.contains("session_activity"));
  EXPECT_TRUE(result.contains("session_reset_command_list"));
  EXPECT_TRUE(result.contains("session_set_command_list"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "session_activity",
      {
        {"session_reset_command_list"},
        {"session_set_command_list"},
        {"session_set_command_list", "session_reset_command_list"},
      }
    },
    {
      "session_reset_command_list",
      {
        {"session_reset_command"},
        {"session_reset_command_list", "session_reset_command"},
      }
    },
    {
      "session_set_command_list",
      {
        {"session_set_command"},
        {"session_set_command_list", "session_set_command"},
      }
    }
  }) ));
}

TEST(BnfToBison, test_8) {

  stringstream s(R"%(
<transaction characteristics> ::=
    <transaction mode 1> { <comma> <transaction mode 2> }
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 1);
  EXPECT_TRUE(result.contains("transaction_characteristics"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "transaction_characteristics",
      {
        {"transaction_mode_1", "comma", "transaction_mode_2"},
      }
    },
  }) ));
}

TEST(BnfToBison, test_9) {

  stringstream s(R"%(
<transaction characteristics> ::=
    <transaction mode 1> { <comma> <transaction mode 2> }...
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 2);
  EXPECT_TRUE(result.contains("transaction_characteristics"));
  EXPECT_TRUE(result.contains("comma_transaction_mode_2_list"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "transaction_characteristics",
      {
        {"transaction_mode_1", "comma_transaction_mode_2_list"},
      }
    },
    {
      "comma_transaction_mode_2_list",
      {
        {"comma", "transaction_mode_2"},
        {"comma_transaction_mode_2_list", "comma", "transaction_mode_2"},
      }
    },
  }) ));
}

TEST(BnfToBison, test_10) {

  stringstream s(R"%(
<transaction characteristics> ::=
    <transaction mode 1> [ { <comma> <transaction mode 2> }... ]
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 2);
  EXPECT_TRUE(result.contains("transaction_characteristics"));
  EXPECT_TRUE(result.contains("comma_transaction_mode_2_list"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "transaction_characteristics",
      {
        {"transaction_mode_1"},
        {"transaction_mode_1", "comma_transaction_mode_2_list"},
      }
    },
    {
      "comma_transaction_mode_2_list",
      {
        {"comma", "transaction_mode_2"},
        {"comma_transaction_mode_2_list", "comma", "transaction_mode_2"},
      }
    },
  }) ));
}

TEST(BnfToBison, test_11) {

  stringstream s(R"%(
<create graph statement> ::=
    CREATE { GRAPH
  | OR REPLACE GRAPH }
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 1);
  EXPECT_TRUE(result.contains("create_graph_statement"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "create_graph_statement",
      {
        {"CREATE", "GRAPH"},
        {"CREATE", "OR", "REPLACE", "GRAPH"},
      }
    },
  }) ));
}

TEST(BnfToBison, test_12) {

  stringstream s(R"%(
<create graph statement> ::=
    CREATE { [ PROPERTY ] GRAPH [ IF NOT EXISTS ]
  | OR REPLACE [ PROPERTY ] GRAPH }
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 1);
  EXPECT_TRUE(result.contains("create_graph_statement"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "create_graph_statement",
      {
        {"CREATE", "GRAPH"},
        {"CREATE", "PROPERTY", "GRAPH"},
        {"CREATE", "GRAPH", "IF", "NOT", "EXISTS"},
        {"CREATE", "PROPERTY", "GRAPH"},
        {"CREATE", "PROPERTY", "GRAPH", "IF", "NOT", "EXISTS"},
        {"CREATE", "OR", "REPLACE", "GRAPH"},
        {"CREATE", "OR", "REPLACE", "PROPERTY", "GRAPH"},
      }
    },
  }) ));
}

TEST(BnfToBison, test_13) {

  stringstream s(R"%(
<create graph statement> ::=
    CREATE { [ PROPERTY ] GRAPH [ IF NOT EXISTS ]
  | OR REPLACE [ PROPERTY ] GRAPH }
         <catalog graph parent and name>
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 1);
  EXPECT_TRUE(result.contains("create_graph_statement"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "create_graph_statement",
      {
        {"CREATE", "GRAPH", "catalog_graph_parent_and_name"},
        {"CREATE", "PROPERTY", "GRAPH", "catalog_graph_parent_and_name"},
        {"CREATE", "GRAPH", "IF", "NOT", "EXISTS", "catalog_graph_parent_and_name"},
        {"CREATE", "PROPERTY", "GRAPH", "catalog_graph_parent_and_name"},
        {"CREATE", "PROPERTY", "GRAPH", "IF", "NOT", "EXISTS", "catalog_graph_parent_and_name"},
        {"CREATE", "OR", "REPLACE", "GRAPH", "catalog_graph_parent_and_name"},
        {"CREATE", "OR", "REPLACE", "PROPERTY", "GRAPH", "catalog_graph_parent_and_name"},
      }
    },
  }) ));
}

TEST(BnfToBison, test_14) {

  stringstream s(R"%(
<create graph statement> ::=
    CREATE { [ PROPERTY ] GRAPH [ IF NOT EXISTS ]
  | OR REPLACE [ PROPERTY ] GRAPH }
         <catalog graph parent and name> { <open graph type>
  | <of graph type> }
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 1);
  EXPECT_TRUE(result.contains("create_graph_statement"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "create_graph_statement",
      {
        {"CREATE", "GRAPH", "catalog_graph_parent_and_name", "open_graph_type"},
        {"CREATE", "PROPERTY", "GRAPH", "catalog_graph_parent_and_name", "open_graph_type"},
        {"CREATE", "GRAPH", "IF", "NOT", "EXISTS", "catalog_graph_parent_and_name", "open_graph_type"},
        {"CREATE", "PROPERTY", "GRAPH", "catalog_graph_parent_and_name", "open_graph_type"},
        {"CREATE", "PROPERTY", "GRAPH", "IF", "NOT", "EXISTS", "catalog_graph_parent_and_name", "open_graph_type"},
        {"CREATE", "OR", "REPLACE", "GRAPH", "catalog_graph_parent_and_name", "open_graph_type"},
        {"CREATE", "OR", "REPLACE", "PROPERTY", "GRAPH", "catalog_graph_parent_and_name", "open_graph_type"},

        {"CREATE", "GRAPH", "catalog_graph_parent_and_name", "of_graph_type"},
        {"CREATE", "PROPERTY", "GRAPH", "catalog_graph_parent_and_name", "of_graph_type"},
        {"CREATE", "GRAPH", "IF", "NOT", "EXISTS", "catalog_graph_parent_and_name", "of_graph_type"},
        {"CREATE", "PROPERTY", "GRAPH", "catalog_graph_parent_and_name", "of_graph_type"},
        {"CREATE", "PROPERTY", "GRAPH", "IF", "NOT", "EXISTS", "catalog_graph_parent_and_name", "of_graph_type"},
        {"CREATE", "OR", "REPLACE", "GRAPH", "catalog_graph_parent_and_name", "of_graph_type"},
        {"CREATE", "OR", "REPLACE", "PROPERTY", "GRAPH", "catalog_graph_parent_and_name", "of_graph_type"},
      }
    },
  }) ));
}

TEST(BnfToBison, test_15) {

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

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 1);
  EXPECT_TRUE(result.contains("create_graph_statement"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "create_graph_statement",
      {
        {"CREATE", "GRAPH", "catalog_graph_parent_and_name", "open_graph_type"},
        {"CREATE", "PROPERTY", "GRAPH", "catalog_graph_parent_and_name", "open_graph_type"},
        {"CREATE", "GRAPH", "IF", "NOT", "EXISTS", "catalog_graph_parent_and_name", "open_graph_type"},
        {"CREATE", "PROPERTY", "GRAPH", "catalog_graph_parent_and_name", "open_graph_type"},
        {"CREATE", "PROPERTY", "GRAPH", "IF", "NOT", "EXISTS", "catalog_graph_parent_and_name", "open_graph_type"},
        {"CREATE", "OR", "REPLACE", "GRAPH", "catalog_graph_parent_and_name", "open_graph_type"},
        {"CREATE", "OR", "REPLACE", "PROPERTY", "GRAPH", "catalog_graph_parent_and_name", "open_graph_type"},

        {"CREATE", "GRAPH", "catalog_graph_parent_and_name", "of_graph_type"},
        {"CREATE", "PROPERTY", "GRAPH", "catalog_graph_parent_and_name", "of_graph_type"},
        {"CREATE", "GRAPH", "IF", "NOT", "EXISTS", "catalog_graph_parent_and_name", "of_graph_type"},
        {"CREATE", "PROPERTY", "GRAPH", "catalog_graph_parent_and_name", "of_graph_type"},
        {"CREATE", "PROPERTY", "GRAPH", "IF", "NOT", "EXISTS", "catalog_graph_parent_and_name", "of_graph_type"},
        {"CREATE", "OR", "REPLACE", "GRAPH", "catalog_graph_parent_and_name", "of_graph_type"},
        {"CREATE", "OR", "REPLACE", "PROPERTY", "GRAPH", "catalog_graph_parent_and_name", "of_graph_type"},

        {"CREATE", "GRAPH", "catalog_graph_parent_and_name", "open_graph_type", "graph_source"},
        {"CREATE", "PROPERTY", "GRAPH", "catalog_graph_parent_and_name", "open_graph_type", "graph_source"},
        {"CREATE", "GRAPH", "IF", "NOT", "EXISTS", "catalog_graph_parent_and_name", "open_graph_type", "graph_source"},
        {"CREATE", "PROPERTY", "GRAPH", "catalog_graph_parent_and_name", "open_graph_type", "graph_source"},
        {"CREATE", "PROPERTY", "GRAPH", "IF", "NOT", "EXISTS", "catalog_graph_parent_and_name", "open_graph_type", "graph_source"},
        {"CREATE", "OR", "REPLACE", "GRAPH", "catalog_graph_parent_and_name", "open_graph_type", "graph_source"},
        {"CREATE", "OR", "REPLACE", "PROPERTY", "GRAPH", "catalog_graph_parent_and_name", "open_graph_type", "graph_source"},

        {"CREATE", "GRAPH", "catalog_graph_parent_and_name", "of_graph_type", "graph_source"},
        {"CREATE", "PROPERTY", "GRAPH", "catalog_graph_parent_and_name", "of_graph_type", "graph_source"},
        {"CREATE", "GRAPH", "IF", "NOT", "EXISTS", "catalog_graph_parent_and_name", "of_graph_type", "graph_source"},
        {"CREATE", "PROPERTY", "GRAPH", "catalog_graph_parent_and_name", "of_graph_type", "graph_source"},
        {"CREATE", "PROPERTY", "GRAPH", "IF", "NOT", "EXISTS", "catalog_graph_parent_and_name", "of_graph_type", "graph_source"},
        {"CREATE", "OR", "REPLACE", "GRAPH", "catalog_graph_parent_and_name", "of_graph_type", "graph_source"},
        {"CREATE", "OR", "REPLACE", "PROPERTY", "GRAPH", "catalog_graph_parent_and_name", "of_graph_type", "graph_source"},
      }
    },
  }) ));
}

TEST(BnfToBison, test_16) {

  stringstream s(R"%(
<delete statement> ::=
    [ DETACH
  | NODETACH ] DELETE <delete item list>
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 1);
  EXPECT_TRUE(result.contains("delete_statement"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "delete_statement",
      {
        {"DELETE", "delete_item_list"},
        {"DETACH", "DELETE", "delete_item_list"},
        {"NODETACH", "DELETE", "delete_item_list"},
      }
    },
  }) ));
}

TEST(BnfToBison, test_17) {

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

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 1);
  EXPECT_TRUE(result.contains("exists_predicate"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "exists_predicate",
      {
        {"EXISTS", "left_brace", "graph_pattern", "right_brace"},
        {"EXISTS", "left_paren", "graph_pattern", "right_paren"},
        {"EXISTS", "left_brace", "match_statement_block", "right_brace"},
        {"EXISTS", "left_paren", "match_statement_block", "right_paren"},
        {"EXISTS", "nested_query_specification"},
      }
    },
  }) ));
}

TEST(BnfToBison, test_18) {

  stringstream s(R"%(
<transaction activity> ::=
    <start transaction command>
         [ <procedure specification> [ <end transaction command> ] ]
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 1);
  EXPECT_TRUE(result.contains("transaction_activity"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "transaction_activity",
      {
        {"start_transaction_command"},
        {"start_transaction_command", "procedure_specification"},
        {"start_transaction_command", "procedure_specification", "end_transaction_command"},
      }
    },
  }) ));
}

TEST(BnfToBison, test_19) {

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

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 1);
  EXPECT_TRUE(result.contains("session_set_command"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "session_set_command",
      {
        {"SESSION", "SET", "session_set_schema_clause"},
        {"SESSION", "SET", "session_set_graph_clause"},
        {"SESSION", "SET", "session_set_time_zone_clause"},
        {"SESSION", "SET", "session_set_parameter_clause"},
      }
    },
  }) ));
}

TEST(BnfToBison, test_20) {

  stringstream s(R"%(
<single quoted character representation> ::=
    <character representation>
  | <double single quote>!! See the Syntax Rules.
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 1);
  EXPECT_TRUE(result.contains("single_quoted_character_representation"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "single_quoted_character_representation",
      {
        {"character_representation"},
        {"double_single_quote"},
      }
    },
  }) ));
}

TEST(BnfToBison, test_21) {

  stringstream s(R"%(
<double single quote> ::=
    <quote> <quote>
    !! See the Syntax Rules.
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 1);
  EXPECT_TRUE(result.contains("double_single_quote"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "double_single_quote",
      {
        {"quote", "quote"},
      }
    },
  }) ));
}

TEST(BnfToBison, test_22) {

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

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 2);
  EXPECT_TRUE(result.contains("GQL_program"));
  EXPECT_TRUE(result.contains("program_activity"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "GQL_program",
      {
        {"program_activity"},
        {"program_activity", "session_close_command"},
        {"session_close_command"},
      }
    },
    {
      "program_activity",
      {
        {"session_activity"},
        {"transaction_activity"},
      }
    },
  }) ));
}

TEST(BnfToBison, test_23) {

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

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 2);
  EXPECT_TRUE(result.contains("GQL_program"));
  EXPECT_TRUE(result.contains("program_activity"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "GQL_program",
      {
        {"program_activity"},
        {"program_activity", "session_close_command"},
        {"session_close_command"},
      }
    },
    {
      "program_activity",
      {
        {"session_activity"},
        {"transaction_activity"},
      }
    },
  }) ));
}

TEST(BnfToBison, test_24) {

  stringstream s(R"%(
<implementation-defined access mode> ::=
    !! See the Syntax Rules.

)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 1);
  EXPECT_TRUE(result.contains("implementation_defined_access_mode"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "implementation_defined_access_mode",
      {}
    },
  }) ));
}

TEST(BnfToBison, test_25) {

  stringstream s(R"%(
<implementation-defined access mode> ::=
    !! See the Syntax Rules.

<rollback command> ::=
    ROLLBACK

)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 2);
  EXPECT_TRUE(result.contains("implementation_defined_access_mode"));
  EXPECT_TRUE(result.contains("rollback_command"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "implementation_defined_access_mode",
      {}
    },
    {
      "rollback_command",
      {
        {"ROLLBACK"},
      }
    },
  }) ));
}

TEST(BnfToBison, test_26) {

  stringstream s(R"%(
<pre-reserved word> ::=
    
    ABSTRACT

  | AGGREGATE

  | AGGREGATES
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 1);
  EXPECT_TRUE(result.contains("pre_reserved_word"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "pre_reserved_word",
      {
        {"ABSTRACT"},
        {"AGGREGATE"},
        {"AGGREGATES"},
      }
    },
  }) ));
}

TEST(BnfToBison, test_27) {

  stringstream s(R"%(
<pre-reserved word> ::=
    
  | ABSTRACT

  | AGGREGATE

  | AGGREGATES
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_NE(parser(), 0);
  EXPECT_EQ(result.size(), 0);
}

TEST(BnfToBison, test_28) {

  stringstream s(R"%(
<space> ::=
    " "
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 1);
  EXPECT_TRUE(result.contains("space"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "space",
      {
        {"\" \""},
      }
    },
  }) ));
}

TEST(BnfToBison, test_29) {

  stringstream s(R"%(
<right brace> ::=
    "}"
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 1);
  EXPECT_TRUE(result.contains("right_brace"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "right_brace",
      {
        {"\"}\""},
      }
    },
  }) ));
}

TEST(BnfToBison, test_30) {

  stringstream s(R"%(
<reverse solidus> ::=
    "\\"

<right brace> ::=
    "}"
)%");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_EQ(parser(), 0);
  EXPECT_EQ(result.size(), 2);
  EXPECT_TRUE(result.contains("reverse_solidus"));
  EXPECT_TRUE(result.contains("right_brace"));
  EXPECT_THAT(result, UnorderedElementsAreArray( (map<string, set<vector<string>>>{
    {
      "reverse_solidus",
      {
        {"\"\\\\\""},
      }
    },
    {
      "right_brace",
      {
        {"\"}\""},
      }
    },
  }) ));
}

TEST(BnfToBison, test_31) {

  stringstream s("<a> := b");

  Lexer lexer(&s);

  location loc{};
  BisonParam bisonParam;

  BnfToBison parser([&lexer](location& loc) -> BnfToBison::symbol_type {
    return lexer.yylex(loc);
  },
  bisonParam,
  loc);

  auto& result = bisonParam.result;

  EXPECT_NE(parser(), 0);
  EXPECT_EQ(result.size(), 0);
}

}

