parser grammar ToyParser;

options {
    tokenVocab = ToyLexer;
}

program : declaration* EOF;

signature : Identifier Colon expr;
impl : Let Identifier bindList? Eq expr;

declaration
    : impl
    | signature
    ;

bindList : Identifier+;

atomExpr
    : Identifier
    | LeftParen expr RightParen
    | Univ
    | Level
    ;

appExpr
    : appExpr atomExpr
    | appExpr As expr
    | atomExpr
    ;

expr
    : Lambda bindList Arrow expr
    | LeftParen Identifier Colon expr RightParen Arrow expr
    | appExpr Arrow expr
    | appExpr
    ;
