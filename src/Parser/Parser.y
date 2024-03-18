%skeleton "lalr1.cc"
%output "Parser.tab.cpp"
%defines "Parser.tab.h"
%define parse.trace
%require "3.2"
%language "c++"

%code requires {
   #include <Declaration/Unchecked.h>
   #include <string>
   #include <vector>
   #include <memory>
   class ToyLexer;
   class ToyParser;
}
%parse-param { ToyLexer& lexer  }
%parse-param { ToyParser& driver  }
%code {
#include <Syntax/Helper.h>
#include <Parser/ToyParser.h>
#include <Exception/Common.h>
#include <sstream>

#undef yylex
#define yylex() lexer.yylex(0)
}

%define api.parser.class { Parser }
%define api.value.type variant
%define api.token.constructor
%define parse.assert


%token <Id>          ID
%token               UNIV
%token               LEVEL
%token               ARROW
%token               COLON
%token               LEFT_PAREN
%token               RIGHT_PAREN
%token               LAMBDA
%token               EQ
%token               AS
%token               CUP
%token               PLUS
%token <MetaNat>     LNAT
%token               SPLIT
%token               END

%type <UncheckedDecls> Program
%type <UncheckedPtr> Declaration
%type <SyntaxPtr> AtomExpr AppExpr Expr LevelAtomExpr LevelExpr
%type <std::vector<Id>> BindList

%start Program

%%

Program : Declaration { driver.push_decl(std::move($1)); }
    | Program Declaration { driver.push_decl(std::move($2)); }
    | Program END { return 0; }

Declaration : ID COLON Expr SPLIT  { $$ = raw_signature(std::move($1), std::move($3)); }
    | ID EQ Expr SPLIT             { $$ = raw_body(std::move($1), std::move($3)); }
    | ID BindList EQ Expr SPLIT    { $$ = raw_body(std::move($1), syntax::bind($2, std::move($4))); }

Expr : LAMBDA BindList ARROW Expr                     { $$ = syntax::bind($2, std::move($4)); }
    | LEFT_PAREN ID COLON Expr RIGHT_PAREN ARROW Expr { $$ = syntax::pi(std::move($2), std::move($4), std::move($7)); }
    | AppExpr ARROW Expr                              { $$ = syntax::pi("_", std::move($1), std::move($3)); }
    | AppExpr                                         { $$ = std::move($1); }

AppExpr : UNIV LevelExpr        { $$ = syntax::univ(std::move($2)); }
    | AppExpr AtomExpr         { $$ = syntax::app(std::move($1), std::move($2)); }
    | AtomExpr                 { $$ = std::move($1); }

AtomExpr : LEVEL                  { $$ = syntax::level(); }
    | ID                          { $$ = syntax::ref(std::move($1)); }
    | LEFT_PAREN Expr RIGHT_PAREN { $$ = std::move($2);  }

LevelExpr : LevelExpr PLUS LNAT    { $$ = syntax::l_incr(std::move($1), $3); }
    | LevelExpr CUP LevelAtomExpr  { $$ = syntax::l_max(std::move($1), std::move($3)); }
    | LevelAtomExpr                { $$ = std::move($1); }

LevelAtomExpr : ID                     { $$ = syntax::ref(std::move($1)); }
    | LNAT                             { $$ = syntax::l_nat($1); }
    | LEFT_PAREN LevelExpr RIGHT_PAREN { $$ = std::move($2);  }

BindList : ID      { std::vector<Id> s; s.push_back(std::move($1)); $$ = std::move(s); }
    | BindList ID { $1.push_back(std::move($2)); $$ = std::move($1); }
%%

void yy::Parser::error(
    const std::string &err_message
) {

   std::stringstream ss;
   ss << "Error: " << err_message << "\n";
   throw ParseException(ss.str());
}
