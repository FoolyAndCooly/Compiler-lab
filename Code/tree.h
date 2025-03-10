#ifndef _TREE_H_
#define _TREE_H_

typedef struct Node{
    int type; // 0: SyntaxNode, 1: TokenNode
    char* name;
    char* attr; // for SyntaxNode line_num, for TokenNode val or type
    struct Node* child[20];
    int num; // children num
    int depth; // used in display
}Node; 

Node* create(int type, char* name, char* attr);
Node* create_syntax(int type, char* name, int line);
void add(int, Node* parent, ...);
void display();
extern Node* root;

#endif
