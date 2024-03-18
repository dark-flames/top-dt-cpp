%{
#include <Parser/ToyLexer.h>

#undef  YY_DECL
#define YY_DECL yy::Parser::symbol_type ToyLexer::yylex(int __marker)
#define MEET_TOKEN if (YYSTATE)

using helper = yy::Parser;

#define yyterminate() return helper::make_END();
%}

%option debug
%option nodefault
%option yyclass="ToyLexer"
%option noyywrap
%option c++
%option outfile="Lexer.yy.cpp"



NEWLINE      (\r?\n)|(\r)
WHITE_SPACE  [ \t]
WHITE_SPACES [ \t]+
SPECIAL      [\?\.\+\/\(\)\{\}\\\:\-]
EMPTY_LINE   \s*((\r?\n)|(\r))

DIGIT         [0-9]
ID_CHAR       [a-zA-Z0-9\_]
ID_CHAR_START [a-zA-Z0-9]

%s SUB_LEVEL
%x COMMENT

%{
int comment_caller;
%}

%%

"Univ"                         { BEGIN(SUB_LEVEL); return helper::make_UNIV(); }
"Level"                        { BEGIN(SUB_LEVEL); return helper::make_LEVEL(); }
"as"                           { BEGIN(SUB_LEVEL); return helper::make_AS(); }
{DIGIT}+                       { BEGIN(SUB_LEVEL); return helper::make_LNAT(atoi(yytext)); }
{ID_CHAR_START}{ID_CHAR}*      {
                                    BEGIN(SUB_LEVEL);
                                    return helper::make_ID(yytext);
                               }
"+"                            { BEGIN(SUB_LEVEL); return helper::make_PLUS(); }
"\\/"                          { BEGIN(SUB_LEVEL); return helper::make_CUP(); }
"->"                           { BEGIN(SUB_LEVEL); return helper::make_ARROW(); }
":"                            { BEGIN(SUB_LEVEL); return helper::make_COLON(); }
"("                            { BEGIN(SUB_LEVEL); return helper::make_LEFT_PAREN(); }
")"                            { BEGIN(SUB_LEVEL); return helper::make_RIGHT_PAREN(); }
"\\"                           { BEGIN(SUB_LEVEL); return helper::make_LAMBDA(); }
"="                            { BEGIN(SUB_LEVEL); return helper::make_EQ(); }

<<EOF>>                        { yyterminate(); }

"{-"                           { comment_caller = YYSTATE; BEGIN(COMMENT); }
<COMMENT>"\\-}"                ;
<COMMENT>"-}"                  { BEGIN(comment_caller); }
<COMMENT>.                     ;
<SUB_LEVEL>{WHITE_SPACE}+                        ;
<INITIAL>{EMPTY_LINE}+                           ;
<SUB_LEVEL>{NEWLINE}+{EMPTY_LINE}*{WHITE_SPACE}+ ;
<SUB_LEVEL>{NEWLINE}+{EMPTY_LINE}*                { BEGIN(INITIAL); return helper::make_SPLIT(); }
%%