#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

typedef struct Node{
    int type; // 0: SyntaxNode, 1: TokenNode
    char* name;
    char* attr; // for SyntaxNode line_num, for TokenNode val or type
    struct Node* child[20];
    int num; // children num
}Node; 

char buffer[256];

Node* root;

Node* create(int type, char* name, char* attr) {
    Node* node = (Node*)malloc(sizeof(struct Node));
    node->type = type;
    node->name = (char*)malloc(strlen(name) + 1);
    strcpy(node->name, name);
    node->attr = (char*)malloc(strlen(attr) + 1);
    strcpy(node->attr, attr);
    return node;
}

Node* create_syntax(int type, char* name, int line) {
    sprintf(buffer, "%d", line); 
    return create(type, name, buffer);
}

void add(int num, Node* parent, ...) {
    va_list args;
    va_start(args, parent);
    Node* child = va_arg(args, Node*);
    for (int i = 0; i < num; i++) {
        parent->child[parent->num] = child;
        parent->num++;
	child = va_arg(args, Node*);
    }
}

void preorder(Node* node, int depth) {
    if (node == NULL) return ;
    for (int i = 0; i < depth; i++) printf("  ");
    depth++;
    if (node->type == 0) {
        printf("%s (%s)\n", node->name, node->attr);
    } else if (strcmp(node->name, "ID") == 0 || strcmp(node->name, "TYPE") == 0 || 
              strcmp(node->name, "INT") == 0 || strcmp(node->name, "FLOAT") == 0) {
        printf("%s: %s\n", node->name, node->attr);    
    } else {
        printf("%s\n", node->name);
    }

    for (int i = 0; i < node->num; i++) {
        preorder(node->child[i], depth);
    }
}

void display() {
    preorder(root, 0);
}


