%{
    #include <stdio.h>
    #include <tree.h>
    #include <assert.h>
    #include "lex.yy.c"
    void yyerror(char* msg);
%}
%union
{
    struct Node* node;
}

%token <node> LETTER
%token <node> SEMI COMMA
%token <node> TYPE
%token <node> STRUCT
%token <node> IF WHILE RETURN
%token <node> INT FLOAT
%token <node> ID
%token <node> SINGLE_LINE_COMMENT MULTI_LINE_COMMENT
%token <node> UNKNOWN_CHAR
%token <node> ASSIGNOP
%token <node> OR
%token <node> AND
%token <node> RELOP
%token <node> PLUS MINUS
%token <node> STAR DIV
%token <node> NOT
%token <node> LP RP LB RB DO LC RC DOT
%token <node> ELSE

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DO LC RC DOT

%type <node> Program ExtDefList ExtDef Specifier ExtDecList FunDec CompSt VarDec
%type <node> StructSpecifier OptTag DefList Tag 
%type <node> VarList ParamDec
%type <node> StmtList Stmt Exp Args DecList Dec Def

%%
Program : ExtDefList {$$ = create_syntax(0, "Program", @$.first_line); add(1, $$, $1); root = $$;}
    ;
ExtDefList : ExtDef ExtDefList {$$ = create_syntax(0, "ExtDefList", @$.first_line); add(2, $$, $1, $2);}
    | %empty {$$ = NULL;}
    ;
ExtDef : Specifier ExtDecList SEMI {$$ = create_syntax(0, "ExtDef", @$.first_line); add(3, $$, $1, $2, $3);}
    | Specifier SEMI {$$ = create_syntax(0, "ExtDef", @$.first_line); add(2, $$, $1, $2);}
    | Specifier FunDec CompSt {$$ = create_syntax(0, "ExtDef", @$.first_line); add(3, $$, $1, $2, $3);}
    ;
ExtDecList : VarDec {$$ = create_syntax(0, "ExtDecList", @$.first_line); add(1, $$, $1);}
    | VarDec COMMA ExtDecList {$$ = create_syntax(0, "ExtDecList", @$.first_line); add(3, $$, $1, $2, $3);}
    ;

Specifier : TYPE {$$ = create_syntax(0, "Specifier", @$.first_line); add(1, $$, $1);}
    | StructSpecifier {$$ = create_syntax(0, "Specifier", @$.first_line); add(1, $$, $1);}
    ;
StructSpecifier : STRUCT OptTag LC DefList RC {$$ = create_syntax(0, "StructSpecifier", @$.first_line); add(5, $$, $1, $2, $3, $4, $5);}
    | STRUCT Tag {$$ = create_syntax(0, "StructSpecifier", @$.first_line); add(2, $$, $1, $2);}
    ;
OptTag : ID {$$ = create_syntax(0, "OptTag", @$.first_line); add(1, $$, $1);}
    | %empty {$$ = NULL;}
    ;
Tag : ID {$$ = create_syntax(0, "Tag", @$.first_line); add(1, $$, $1);}
    ;

VarDec : ID {$$ = create_syntax(0, "VarDec", @$.first_line); add(1, $$, $1);}
    | VarDec LB INT RB {$$ = create_syntax(0, "VarDec", @$.first_line); add(4, $$, $1, $2, $3, $4);}
    ;
FunDec : ID LP VarList RP {$$ = create_syntax(0, "FunDec", @$.first_line); add(4, $$, $1, $2, $3, $4);}
    | ID LP RP {$$ = create_syntax(0, "FunDec", @$.first_line); add(3, $$, $1, $2, $3);}
    ;
VarList : ParamDec COMMA VarList {$$ = create_syntax(0, "VarList", @$.first_line); add(3, $$, $1, $2, $3);}
    | ParamDec {$$ = create_syntax(0, "VarList", @$.first_line); add(2, $$, $1);}
    ;
ParamDec : Specifier VarDec {$$ = create_syntax(0, "ParamDec", @$.first_line); add(2, $$, $1, $2);}
    ;

CompSt : LC DefList StmtList RC {$$ = create_syntax(0, "CompSt", @$.first_line); add(4, $$, $1, $2, $3, $4);}
    ;
StmtList : Stmt StmtList {$$ = create_syntax(0, "StmtList", @$.first_line); add(2, $$, $1, $2);}
    | %empty {$$ = NULL;}
    ;
Stmt : Exp SEMI {$$ = create_syntax(0, "Stmt", @$.first_line); add(2, $$, $1, $2);}
    | CompSt {$$ = create_syntax(0, "Stmt", @$.first_line); add(1, $$, $1);}
    | RETURN Exp SEMI {$$ = create_syntax(0, "Stmt", @$.first_line); add(3, $$, $1, $2, $3);}
    | IF LP Exp RP Stmt %prec LOWER_THAN_ELSE {$$ = create_syntax(0, "Stmt", @$.first_line); add(5, $$, $1, $2, $3, $4, $5);}
    | IF LP Exp RP Stmt ELSE Stmt {$$ = create_syntax(0, "Stmt", @$.first_line); add(7, $$, $1, $2, $3, $4, $5, $6, $7);}
    | WHILE LP Exp RP Stmt {$$ = create_syntax(0, "Stmtc", @$.first_line); add(5, $$, $1, $2, $3, $4, $5);}
    ;

DefList : Def DefList {$$ = create_syntax(0, "DefList", @$.first_line); add(2, $$, $1, $2);}
    | %empty {$$ = NULL;}
    ;
Def : Specifier DecList SEMI {$$ = create_syntax(0, "Def", @$.first_line); add(3, $$, $1, $2, $3);}
    ;
DecList : Dec {$$ = create_syntax(0, "DecList", @$.first_line); add(1, $$, $1);}
    | Dec COMMA DecList {$$ = create_syntax(0, "DecList", @$.first_line); add(3, $$, $1, $2, $3);}
    ;
Dec : VarDec {$$ = create_syntax(0, "Dec", @$.first_line); add(1, $$, $1);}
    | VarDec ASSIGNOP Exp {$$ = create_syntax(0, "Dec", @$.first_line); add(3, $$, $1, $2, $3);}
    ;

Exp : Exp ASSIGNOP Exp {$$ = create_syntax(0, "Exp", @$.first_line); add(3, $$, $1, $2, $3);}
    | Exp AND Exp {$$ = create_syntax(0, "Exp", @$.first_line); add(3, $$, $1, $2, $3);}
    | Exp OR Exp {$$ = create_syntax(0, "Exp", @$.first_line); add(3, $$, $1, $2, $3);}
    | Exp RELOP Exp {$$ = create_syntax(0, "Exp", @$.first_line); add(3, $$, $1, $2, $3);}
    | Exp PLUS Exp {$$ = create_syntax(0, "Exp", @$.first_line); add(3, $$, $1, $2, $3);}
    | Exp MINUS Exp {$$ = create_syntax(0, "Exp", @$.first_line); add(3, $$, $1, $2, $3);}
    | Exp STAR Exp {$$ = create_syntax(0, "Exp", @$.first_line); add(3, $$, $1, $2, $3);}
    | Exp DIV Exp {$$ = create_syntax(0, "Exp", @$.first_line); add(3, $$, $1, $2, $3);}
    | LP Exp RP {$$ = create_syntax(0, "Exp", @$.first_line); add(3, $$, $1, $2, $3);}
    | MINUS Exp {$$ = create_syntax(0, "Exp", @$.first_line); add(2, $$, $1, $2);}
    | NOT Exp {$$ = create_syntax(0, "Exp", @$.first_line); add(2, $$, $1, $2);}
    | ID LP Args RP {$$ = create_syntax(0, "Exp", @$.first_line); add(4, $$, $1, $2, $3, $4);}
    | ID LP RP {$$ = create_syntax(0, "Exp", @$.first_line); add(3, $$, $1, $2, $3);}
    | Exp LB Exp RB {$$ = create_syntax(0, "Exp", @$.first_line); add(4, $$, $1, $2, $3, $4);}
    | Exp DOT ID {$$ = create_syntax(0, "Exp", @$.first_line); add(3, $$, $1, $2, $3);}
    | ID {$$ = create_syntax(0, "Exp", @$.first_line); add(1, $$, $1);}
    | INT {$$ = create_syntax(0, "Exp", @$.first_line); add(1, $$, $1);}
    | FLOAT {$$ = create_syntax(0, "Exp", @$.first_line); add(1, $$, $1);}
    ;
Args : Exp COMMA Args {$$ = create_syntax(0, "Args", @$.first_line); add(3, $$, $1, $2, $3);}
    | Exp {$$ = create_syntax(0, "Args", @$.first_line); add(1, $$, $1);}
    ;
%%
void yyerror(char* msg)
{
        ;
}
