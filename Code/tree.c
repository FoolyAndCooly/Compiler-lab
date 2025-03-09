#include <string.h>
#include <stdio.h>

struct Node{
    int type; // 0: SyntaxNode, 1: TokenNode
    char* name;
    char* attr; // for SyntaxNode line_num, for TokenNode val or type
    struct Node* child[20];
    struct int num; // children num
    int depth; // used in display
}; 

char buffer[256];

Node* root;

Node* create(int type, char* name, char* attr) {
    node = (Node*)malloc(sizeof(struct Node));
    node->type = type;
    strcpy(node->name, name);
    strcpy(node->attr, attr);
    node->depth = 0;
    return node;
}

Node* create_syntax(int type, char* name, int line) {
    sprintf(buffer, "%d", line); 
    return create(type, name, buffer);
}

void add(Node* parent, ...) {
    va_list args;
    va_start(args, parent);
    Node* child = va_arg(args, Node*);
    while (child != NULL) {
        patent->child[parent->num] = chlid;
        child->depth = parent->depth + 1;
        patent->num++;
	child = va_arg(args, Node*);
    }
}

void preorder(Node* node) {
    if (node == NULL) return ;
    for (int i = 0; i < depth; i++) printf(" ");
    if (node->type == 0) {
        printf("%s (%s)\n", node->name, node->attr);
    } else {
        if (strcmp(node->name, "ID") == 0 || strcmp(node->name, "TYPE") == 0 || 
	    strcmp(node->name, "INT") == 0 || strcmp(node->name, "FLOAT") == 0) {
            printf("%s: %s\n", node->name, node->attr);    
	} else {
            printf("%s\n", node->name);
	}
    }
}

void display() {
    preorder(root);
}


