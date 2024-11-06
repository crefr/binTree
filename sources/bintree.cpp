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

int cmpInt(void * first, void * second)
{
    int first_int  = *((int *)first);
    int second_int = *((int *)second);
    return first_int - second_int;
}

node_t * newNode(void * data, size_t elem_size)
{
    assert(data);
    node_t * new_node = (node_t *)calloc(1, sizeof(*new_node));

    new_node->elem_size = elem_size;
    new_node->data = calloc(1, elem_size);
    memcpy(new_node->data, data, elem_size);

    new_node->left   = NULL;
    new_node->right  = NULL;
    new_node->parent = NULL;

    return new_node;
}

void delNode(node_t * node)
{
    assert(node);

    free(node->data);
    free(node);
}

void treeSortAddNode(node_t * node, void * data, size_t elem_size, compare_func_t cmp)
{
    if (cmp(data, node->data) <= 0){
        if (node->left == NULL){
            node_t * new_node = newNode(data, elem_size);
            node->left = new_node;
            new_node->parent = node;
        }
        else
            treeSortAddNode(node->left, data, elem_size, cmp);
    }
    else {
        if (node->right == NULL){
            node_t * new_node = newNode(data, elem_size);
            node->right = new_node;
            new_node->parent = node;
        }
        else
            treeSortAddNode(node->right, data, elem_size, cmp);
    }
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

void treeDestroy(node_t * node)
{
    if (node == NULL)
        return;

    if (node->left  != NULL){
        treeDestroy(node->left);
        node->left  = NULL;
    }
    if (node->right != NULL){
        treeDestroy(node->right);
        node->right = NULL;
    }
    delNode(node);
}
