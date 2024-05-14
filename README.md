# bnfparser
A Bison grammar and parser for the extended BNF (EBNF) syntax used to specify the GQL language in the ISO-39075 standard

This Bison grammar was written to parse the extended BNF notation used to specify the GQL Graph Query Language in the ISO-39075:2024 standard.

The GQL EBNF grammar is briefly described in the XML representation of the grammar at https://standards.iso.org/iso-iec/39075/ed-1/en/ISO_IEC_39075(en).bnf.xml. The grammar is available in EBNF form at https://standards.iso.org/iso-iec/39075/ed-1/en/ISO_IEC_39075(en).bnf.txt.

The grammar is fully described in Section 5.2 of ISO-39075:2024.

< >
A character string enclosed in angle brackets is the name of a syntactic element (i.e., the name of a BNF non-terminal symbol) of the GQL language.

::=
The definition operator is used in a production rule to separate the element defined by the rule from its definition. The element being defined appears to the left of the operator and the formula that defines the element appears to the right.

[ ]
Square brackets indicate optional elements in a formula. The portion of the formula within the brackets may be explicitly specified or may be omitted.

{ }
Braces group elements in a formula. The portion of the formula within the braces shall be explicitly specified.

|
The alternative operator. The vertical bar indicates that the portion of the formula following the bar is an alternative to the portion preceding the bar. If the vertical bar appears at a position where it is not enclosed in braces or square brackets, it specifies a complete alternative for the element defined by the production rule. If the vertical bar appears in a portion of a formula enclosed in braces or square brackets, it specifies alternatives for the content of the innermost pair of such braces or brackets.

...
The ellipsis indicates that the element to which it applies in a formula may be repeated any number of times. If the ellipsis appears immediately after a closing brace “}”, then it applies to the portion of the formula enclosed between that closing brace and the corresponding opening brace “{”. If an ellipsis appears after any other element, then it applies only to that element. In Syntax Rules, General Rules, and Conformance Rules, a reference to the n-th element in such a list assumes the order in which these are specified, unless otherwise stated.

!!
Introduces either a reference to the Syntax Rules, used when the definition of a syntactic element is not expressed in BNF, or the Unicode code point or code point sequence that define the character(s) of the BNF production.


I could not find a BNF or Bison grammar for GQL when I wrote this parser. The parser fully parses the GQL specification in docs/gqlgrammar.txt which is a slightly modified and corrected version of https://standards.iso.org/iso-iec/39075/ed-1/en/ISO_IEC_39075(en).bnf.txt. The goal is to use the parser to generate a Bison grammar from GQL EBNF. The generated Bison grammar can then be used to write parsers for graph queries in GQL.

The differences between ISO_IEC_39075(en).bnf.txt and docs/gqlgrammar.txt are

- Literal strings and characters are quoted
- A stray alternative operator in the rule for <pre-reserved word> has been deleted
- The definition of <space> has been corrected to U+0020 from U+00A0

Even though the grammar for GQL EBNF is specific to ISO-39075, its features are typical of other extended BNF syntaxes. It should be easy to modify to represent other EBNF notations.

