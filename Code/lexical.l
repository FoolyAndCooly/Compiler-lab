%{
    #include <stdio.h>
    #include <errno.h>
    #include <stdlib.h>
    #include <string.h>
    #include "syntax.tab.h"
    #include "node.h"
    extern int has_error; 
    // position
    int yycolumn = 1;
    #define YY_USER_ACTION \
    yylloc.first_line = yylloc.last_line = yylineno; \
    yylloc.first_column = yycolumn; \
    yylloc.last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;
%}

%option yylineno

LETTER [a-zA-Z]
SEMI ;
COMMA ,
ASSIGNOP =
RELOP (>|<|>=|<=|==|!=)
PLUS \+
MINUS -
STAR \*
DIV \/
AND &&
OR \|\|
NOT !
TYPE (int|float)
LP \(
RP \)
LB \[
RB \]
LC \{
RC \}
DOT \.
STRUCT struct
RETURN return
IF if
ELSE else
WHILE while
INT ([1-9][0-9]*|0)
FLOAT (0[.][0-9]+|[1-9][0-9]*\.[0-9]+)
ID [_a-zA-Z][_a-zA-Z0-9]*
SINGLE_LINE_COMMENT \/\/.*
MULTI_LINE_COMMENT \/\*([^*]|\*+[^*/])*\*+\/
UNKNOWN_CHAR .

%%
{SEMI}     {yylval.node = create_node(1, "SEMI"    , ""    , yylineno); return SEMI;}
{COMMA}    {yylval.node = create_node(1, "COMMA"   , ""    , yylineno); return COMMA;}
{ASSIGNOP} {yylval.node = create_node(1, "ASSIGNOP", ""    , yylineno); return ASSIGNOP;}
{RELOP}    {yylval.node = create_node(1, "RELOP"   , yytext    , yylineno); return RELOP;}
{PLUS}     {yylval.node = create_node(1, "PLUS"    , ""    , yylineno); return PLUS;}
{MINUS}    {yylval.node = create_node(1, "MINUS"   , ""    , yylineno); return MINUS;}
{STAR}     {yylval.node = create_node(1, "STAR"    , ""    , yylineno); return STAR;}
{DIV}      {yylval.node = create_node(1, "DIV"     , ""    , yylineno); return DIV;}
{AND}      {yylval.node = create_node(1, "AND"     , ""    , yylineno); return AND;}
{OR}       {yylval.node = create_node(1, "OR"      , ""    , yylineno); return OR;}
{NOT}      {yylval.node = create_node(1, "NOT"     , ""    , yylineno); return NOT;}
{TYPE}     {yylval.node = create_node(1, "TYPE"    , yytext, yylineno); return TYPE;}
{DOT}      {yylval.node = create_node(1, "DOT"     , ""    , yylineno); return DOT;}
{LP}       {yylval.node = create_node(1, "LP"      , ""    , yylineno); return LP;}
{RP}       {yylval.node = create_node(1, "RP"      , ""    , yylineno); return RP;}
{LB}       {yylval.node = create_node(1, "LB"      , ""    , yylineno); return LB;}
{RB}       {yylval.node = create_node(1, "RB"      , ""    , yylineno); return RB;}
{LC}       {yylval.node = create_node(1, "LC"      , ""    , yylineno); return LC;}
{RC}       {yylval.node = create_node(1, "RC"      , ""    , yylineno); return RC;}
{STRUCT}   {yylval.node = create_node(1, "STRUCT"  , ""    , yylineno); return STRUCT;}
{RETURN}   {yylval.node = create_node(1, "RETURN"  , ""    , yylineno); return RETURN;}
{IF}       {yylval.node = create_node(1, "IF"      , ""    , yylineno); return IF;}
{ELSE}     {yylval.node = create_node(1, "ELSE"    , ""    , yylineno); return ELSE;}
{WHILE}    {yylval.node = create_node(1, "WHILE"   , ""    , yylineno); return WHILE;}
{FLOAT}    {yylval.node = create_node(1, "FLOAT"   , yytext, yylineno); return FLOAT;}
{INT}      {yylval.node = create_node(1, "INT"     , yytext, yylineno); return INT;}
{ID}       {yylval.node = create_node(1, "ID"      , yytext, yylineno); return ID;}
{SINGLE_LINE_COMMENT} {}
{MULTI_LINE_COMMENT} {}
\n  {yycolumn = 1;}
[ \t\r]+ {}
{UNKNOWN_CHAR} {has_error = 1; printf("Error type A at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext);}
%%

