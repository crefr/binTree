#ifndef BINTREE_INCLUDED
#define BINTREE_INCLUDED

typedef struct node {
    void * data;
    size_t elem_size;
    node * parent;
    node * left;
    node * right;
} node_t;

typedef struct tree {
    node_t * root;
    size_t elem_size;
} tree_t;

typedef void(*printfunc_t)(void *);
typedef int (*compare_func_t)(void *, void *);

node_t * newNode(void * data, size_t elem_size);

void delNode(node_t * node);

void treeDestroy(node_t * node);

void printInt(void * data);

void printTree(node_t * node, printfunc_t printElem);

void treeSortAddNode(node_t * node, void * data, size_t elem_size, compare_func_t cmp);

int cmpInt(void * first, void * second);

#endif
