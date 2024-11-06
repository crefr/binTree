#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "logger.h"
#include "bintree.h"

void printInt(void * data)
{
    printf("%d", *((int *)data));
}

node_t * newNode(void * data, size_t elem_size)
{
    assert(data);
    node_t * new_node = (node_t *)calloc(1, sizeof(*new_node));
    new_node->data = calloc(1, elem_size);
    memcpy(new_node->data, data, elem_size);

    new_node->left  = NULL;
    new_node->right = NULL;
    return new_node;
}

void delNode(node_t * node)
{
    assert(node);
    free(node->data);
    node->data = NULL;

    free(node);
}

void printTree(node_t * node, printfunc_t printElem)
{
    if (node == NULL)
        return;
    printf("(");
    printElem(node->data);
    if (node->left  != NULL)
        printTree(node->left, printElem);
    if (node->right != NULL)
        printTree(node->right, printElem);
    printf(")");
}
