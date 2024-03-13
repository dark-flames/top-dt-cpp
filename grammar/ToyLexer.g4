lexer grammar ToyLexer;

fragment NewLine: ('\r'? '\n' | '\r');
fragment Tab: '\t';
fragment WhiteSpace: [\u0020\u00a0\u1680\u2000\u200a\u202f\u205f\u3000];

// 规范化空格和制表符
fragment SpecialWhiteSpace : NewLine | Tab | WhiteSpace;


fragment Special : [->:(){}];

fragment MultiSpecialWhiteSpace : SpecialWhiteSpace+;

fragment CommentChar : ~[\r\n];
fragment Comment : '{-' CommentChar* '-}';

As : 'as';
Let : 'let';
Arrow : '->';
Colon : ':';
LeftParen: '(';
RightParen: ')';
Univ : 'U';
Level : 'Level';
At : '@';
Lambda : '\\';
Eq : '=';

fragment IdChar : ~[->:(){}@\\=\t\r\n ];
fragment IdCharStart : ~[->:(){}@\\=\t\r\n_ ];
Identifier: IdCharStart IdChar*;


fragment Skipped : SpecialWhiteSpace | Comment;
Skip : Skipped -> skip;