#ifndef BINTREE_INCLUDED
#define BINTREE_INCLUDED

#include <wchar.h>

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
typedef void(*elemtostr_func_t)(char *, void *);
typedef void(*elemtowcs_func_t)(wchar_t *, void *);

node_t * newNode(void * data, size_t elem_size);

void delNode(node_t * node);

void treeDestroy(node_t * node);

void printTree(node_t * node, printfunc_t printElem);

void treeSortAddNode(node_t * node, void * data, size_t elem_size, compare_func_t cmp);

node_t * treeFindNode(node_t * node, void * data, compare_func_t cmp);


void treeDumpGraph(node_t * root_node, elemtostr_func_t elemToStr);

void treeDumpGraphWcs(node_t * root_node, elemtowcs_func_t elemToStr);

void treeMakeDot(node_t * node, elemtostr_func_t elemToStr, FILE * dot_file);

void treeMakeDotWcs(node_t * node, elemtowcs_func_t elemToStr, FILE * dot_file);

/*---------------------------------------------------------------------------------*/
int cmpInt(void * first, void * second);

void intToStr(char * str, void * data);

void printInt(void * data);
/*---------------------------------------------------------------------------------*/

#endif
