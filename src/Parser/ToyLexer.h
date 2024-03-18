#pragma once
#if !defined(yyFlexLexerOnce)

#include <FlexLexer.h>

#endif

#include <Parser/Parser.tab.h>
#include <Exception/Common.h>

class ToyLexer : public yyFlexLexer {
public:
    ToyLexer(std::istream* in) : yyFlexLexer(in) {};

    virtual ~ToyLexer() {}

    using FlexLexer::yylex;

    virtual yy::Parser::symbol_type yylex(int __marker);

private:
    /* yyval ptr */
    yy::Parser::semantic_type* yylval = nullptr;
};