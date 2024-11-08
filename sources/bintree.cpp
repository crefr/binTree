#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "logger.h"
#include "bintree.h"

void printInt(void * data)
{
    assert(data);
    printf("%d", *((int *)data));
}

int cmpInt(void * first, void * second)
{
    assert(first);
    assert(second);
    int first_int  = *((int *)first);
    int second_int = *((int *)second);
    return first_int - second_int;
}

void intToStr(char * str, void * data)
{
    assert(str);
    assert(data);
    int data_int = *((int *)data);
    sprintf(str, "%d", data_int);
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

void treeDumpGraph(node_t * root_node, elemtostr_func_t elemToStr)
{
    assert(root_node);

    const int IMG_SIZE_IN_PERCENTS = 85;

    static size_t dump_count = 0;

    const size_t MAX_FILE_NAME = 256;
    char dot_file_name[MAX_FILE_NAME] = "";
    char img_file_name[MAX_FILE_NAME] = "";

    system("mkdir -p logs/dots/");
    system("mkdir -p logs/imgs/");
    sprintf(dot_file_name, "logs/dots/graph_%zu.dot", dump_count);
    sprintf(img_file_name, "logs/imgs/graph_%zu.png", dump_count);

    FILE * dot_file = fopen(dot_file_name, "w");
    treeMakeDot(root_node, elemToStr, dot_file);
    fclose(dot_file);

    char sys_dot_cmd[MAX_FILE_NAME] = "";
    sprintf(sys_dot_cmd, "dot %s -Tpng -o %s", dot_file_name, img_file_name);
    system(sys_dot_cmd);

    char img_file_name_log[MAX_FILE_NAME] = "";
    sprintf(img_file_name_log, "imgs/graph_%zu.png", dump_count);
    logPrint(LOG_DEBUG, "<img src = %s width = \"%d%%\">", img_file_name_log, IMG_SIZE_IN_PERCENTS);

    logPrint(LOG_DEBUG, "<hr>");

    dump_count++;
}

static void nodeMakeDot(node_t * node, elemtostr_func_t elemToStr, FILE * dot_file);

void treeMakeDot(node_t * node, elemtostr_func_t elemToStr, FILE * dot_file)
{
    assert(node);
    assert(dot_file);
    fprintf(dot_file, "digraph {\n");
    nodeMakeDot(node, elemToStr, dot_file);
    fprintf(dot_file, "}\n");
}

static void nodeMakeDot(node_t * node, elemtostr_func_t elemToStr, FILE * dot_file)
{
    assert(node);
    assert(dot_file);

    size_t node_num = (size_t)node; //TODO: cringe?
    //static size_t node_num = 0;
    const size_t MAX_ELEM_STR_LEN = 64;
    char elem_str[MAX_ELEM_STR_LEN] = "";
    elemToStr(elem_str, node->data);
    fprintf(dot_file, "node_%zu"
                      "[shape=Mrecord,label="
                      "\"{node at %p | left = %p | right = %p | parent = %p | data = %s}\","
                      "color=\"#7229c4\"];\n",
                      node_num, node, node->left, node->right, node->parent, elem_str);
    if (node->parent != NULL){
        size_t node_parent_num = (size_t)(node->parent); //TODO: cringe?
        fprintf(dot_file, "node_%zu->node_%zu;\n", node_parent_num, node_num);
    }
    if (node->left  != NULL)
        nodeMakeDot(node->left,  elemToStr, dot_file);
    if (node->right != NULL)
        nodeMakeDot(node->right, elemToStr, dot_file);
}
