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
BLANK [ \t\r\n]+
INT ([1-9][0-9]*|0)
FLOAT (0[.][0-9]+|[1-9][0-9]*[.][0-9]+)
ID [_a-zA-Z][_a-zA-Z0-9]*
SINGLE_LINE_COMMENT \/\/.*
MULTI_LINE_COMMENT \/\*([^*]|\*+[^*/])*\*+\/
UNKNOWN_CHAR .

%%

{SEMI} {return SEMI;}
{COMMA} {return COMMA;}
{ASSIGNOP} {return ASSIGNOP;}
{RELOP} {return RELOP;}
{PLUS} {return PLUS;}
{MINUS} {return MINUS;}
{STAR} {return STAR;}
{DIV} {return DIV;}
{AND} {return AND;}
{OR} {return OR;}
{NOT} {return NOT;}
{TYPE} {return TYPE;}
{LP} {return LP;}
{RP} {return RP;}
{LC} {return LC;}
{RC} {return RC;}
{STRUCT} {return STRUCT;}
{RETURN} {return RETURN;}
{IF} {return IF;}
{ELSE} {return ELSE;}
{WHILE} {return WHILE;}
{FLOAT} {return FLOAT;}
{INT} {return INT;}
{ID} {insert_symbol(table, yytext, yylineno, TYPE_UNKNOWN); return ID;}
{BLANK} {}
{SINGLE_LINE_COMMENT} {}
{MULTI_LINE_COMMENT} {}
{UNKNOWN_CHAR} {printf("Error type A at Line %d: Mysterious characters \'%s\'\n", yylineno, yytext);}
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