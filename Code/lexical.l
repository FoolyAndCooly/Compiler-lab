%{
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

//
    #define SYMBOL_TABLE_SIZE 97
    typedef enum { TYPE_INT, TYPE_FLOAT, TYPE_UNKNOWN } SymbolType;
    typedef struct SymbolEntry {
        char *name;          
        SymbolType type;    
        unsigned int scope_level;     
        unsigned int first_occur_line;
        struct SymbolEntry *next; 
        // maybe more ... 
    } SymbolEntry;
    typedef struct SymbolTable{
        SymbolEntry* buckets[SYMBOL_TABLE_SIZE];
        // maybe more ...
    }SymbolTable;
    SymbolTable* table;

    SymbolTable* create_symbol_table(){
        SymbolTable *table = malloc(sizeof(SymbolTable));
        memset(table->buckets, 0, sizeof(table->buckets));
        return table;
    }
    
    unsigned int hashmap(const char* name){
        unsigned int seed = 113;
        unsigned int c;
        while((c = *name++))
            seed = ((seed << 5) + seed) + c;
        return seed % SYMBOL_TABLE_SIZE;
    }
    
    void insert_symbol(SymbolTable *table, const char* name,unsigned int line, SymbolType type){   // maybe more paras...
        unsigned int bucket = hashmap(name);
        SymbolEntry *entry = table->buckets[bucket];
        while(entry != NULL){
            if(strcmp(entry->name, name) == 0)
        // maybe more ...
                return;
        }
        SymbolEntry *new_entry = malloc(sizeof(SymbolEntry));
        new_entry->name = strdup(name);
        new_entry->first_occur_line = line;
        new_entry->type = type;
        new_entry->next = table->buckets[bucket];
        table->buckets[bucket] = new_entry;
    }
    
    SymbolEntry* lookup_symbol(SymbolTable *table, const char *name) {
        unsigned int bucket = hashmap(name);
        SymbolEntry *entry = table->buckets[bucket];
        while (entry != NULL) {
            if (strcmp(entry->name, name) == 0) {
        // maybe more ... 
                return entry; 
            }
            entry = entry->next;
        }
        return NULL;
    }
    
    void FreeSymbolTable(SymbolTable* table){
        for(int i = 0; i < SYMBOL_TABLE_SIZE; i++){
            SymbolEntry* entry = table->buckets[i];
            while(entry != NULL){
                SymbolEntry* tmp = entry;
                entry = entry->next;
                free(tmp->name);
                free(tmp);
            } 
        }
        free(table);
    }

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
STRUCT struct
RETURN return
IF if
ELSE else
WHILE while
INT ([1-9][0-9]*|0)
FLOAT (0[.][0-9]+|[1-9][0-9]*[.][0-9]+)
ID [_a-zA-Z][_a-zA-Z0-9]*
SINGLE_LINE_COMMENT \/\/.*
MULTI_LINE_COMMENT \/\*([^*]|\*+[^*/])*\*+\/
UNKNOWN_CHAR .

%%

{SEMI}     {yylval.node = create(1, "SEMI"    , NULL  ); return SEMI;}
{COMMA}    {yylval.node = create(1, "COMMA"   , NULL  ); return COMMA;}
{ASSIGNOP} {yylval.node = create(1, "ASSIGNOP", NULL  ); return ASSIGNOP;}
{RELOP}    {yylval.node = create(1, "RELOP"   , NULL  ); return RELOP;}
{PLUS}     {yylval.node = create(1, "PLUS"    , NULL  ); return PLUS;}
{MINUS}    {yylval.node = create(1, "MINUS"   , NULL  ); return MINUS;}
{STAR}     {yylval.node = create(1, "STAR"    , NULL  ); return STAR;}
{DIV}      {yylval.node = create(1, "DIV"     , NULL  ); return DIV;}
{AND}      {yylval.node = create(1, "AND"     , NULL  ); return AND;}
{OR}       {yylval.node = create(1, "OR"      , NULL  ); return OR;}
{NOT}      {yylval.node = create(1, "NOT"     , NULL  ); return NOT;}
{TYPE}     {yylval.node = create(1, "TYPE"    , yytext); return TYPE;}
{LP}       {yylval.node = create(1, "LP"      , NULL  ); return LP;}
{RP}       {yylval.node = create(1, "RP"      , NULL  ); return RP;}
{LC}       {yylval.node = create(1, "LC"      , NULL  ); return LC;}
{RC}       {yylval.node = create(1, "RC"      , NULL  ); return RC;}
{STRUCT}   {yylval.node = create(1, "STRUCT"  , NULL  ); return STRUCT;}
{RETURN}   {yylval.node = create(1, "RETURN"  , NULL  ); return RETURN;}
{IF}       {yylval.node = create(1, "IF"      , NULL  ); return IF;}
{ELSE}     {yylval.node = create(1, "ELSE"    , NULL  ); return ELSE;}
{WHILE}    {yylval.node = create(1, "WHILE"   , NULL  ); return WHILE;}
{FLOAT}    {yylval.node = create(1, "FLOAT"   , yytext); return FLOAT;}
{INT}      {yylval.node = create(1, "INT"     , yytext); return INT;}
{ID}       {yylval.node = create(1, "ID"      , yytext); return ID;}
{SINGLE_LINE_COMMENT} {}
{MULTI_LINE_COMMENT} {}
{UNKNOWN_CHAR} {printf("Error type A at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext);}
\n  { yycolumn = 1; }
%%


int main(int argc, char** argv){
    table = create_symbol_table();
    if(argc > 1){
        for(int i = 1; i < argc; i++){
            FILE* f;
            if(!(f = fopen(argv[i], "r"))){
                perror(argv[i]);
                FreeSymbolTable(table);
                return -1;
            }
            yylineno = 1;
            yyrestart(f);
            yylex();
            fclose(f);
        }
    }
    FreeSymbolTable(table);
    return 0;
}
