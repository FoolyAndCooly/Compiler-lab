#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "node.h"

Node *root;
char buffer[256];

Node* create_node(NodeType type, char* name, char* attr, unsigned int line) {
    Node* node = (Node*)calloc(1, sizeof(struct Node));
    node->type = type;
    node->name = strdup(name);
    node->attr = strdup(attr);
    node->lineNum = line;
    return node;
}

Node* create_syntax(NodeType type, char* name,unsigned int line) {
    sprintf(buffer, "%d", line); 
    return create_node(type, name, buffer, line);
}

void add_node(unsigned int num, Node* parent, ...) {
    if(num == 0)    return;
    va_list args;
    va_start(args, parent);
    for (int i = 0; i < num; i++) {
        Node* child = va_arg(args, Node*);
        parent->child[parent->num] = child;
        parent->num++;
    }
    va_end(args);
}

void preorder(Node* node, int depth) {
    if (node == NULL) return ;
    for (int i = 0; i < depth; i++) printf("  ");
    depth++;
    if (node->type == SYNTAX_NODE) {
        printf("%s (%s)\n", node->name, node->attr);
    } else if (strcmp(node->name, "ID") == 0 || strcmp(node->name, "TYPE") == 0 || 
              strcmp(node->name, "INT") == 0 || strcmp(node->name, "FLOAT") == 0) {
        if(strcmp(node->name, "FLOAT") == 0){
            float float_num;
            sscanf(node->attr, "%f", &float_num);
            printf("%s: %f\n", node->name, float_num);  
        }
        else
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

void display_node(Node* node) {
    printf("child num: %d\n", node->num);
    for (int i = 0; i < node->num; i++) {
        printf("%s\n", node->child[i]->name);
    }
}
