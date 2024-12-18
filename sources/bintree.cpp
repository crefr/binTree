#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "logger.h"
#include "bintree.h"

dump_mode_t DUMP_mode = DUMP_HARD;

void treeSetDumpMode(dump_mode_t mode)
{
    DUMP_mode = mode;
}

node_t * newNode(void * data, size_t elem_size, node_t * left_child, node_t * right_child, uint32_t color)
{
    assert(data);

    logPrint(LOG_DEBUG_PLUS, "bintree: creating new node (elem_size = %zu)\n", elem_size);

    node_t * new_node = (node_t *)calloc(1, sizeof(*new_node));

    new_node->elem_size = elem_size;
    new_node->data = calloc(1, elem_size);
    memcpy(new_node->data, data, elem_size);

    new_node->left  =  left_child;
    new_node->right = right_child;

    if (left_child  != NULL)
        left_child-> parent = new_node;

    if (right_child != NULL)
        right_child->parent = new_node;

    new_node->color_for_dump = color;

    logPrint(LOG_DEBUG_PLUS, "bintree: created node at %p\n", new_node);

    return new_node;
}

void delNode(node_t * node)
{
    assert(node);

    logPrint(LOG_DEBUG_PLUS, "bintree: deleting node at %p\n", node);

    free(node->data);
    free(node);

    logPrint(LOG_DEBUG_PLUS, "bintree: deleted node\n");
}

void treeSortAddNode(node_t * node, void * data, size_t elem_size, compare_func_t cmp, uint32_t color)
{
    if (cmp(data, node->data) <= 0){
        if (node->left == NULL){
            node_t * new_node = newNode(data, elem_size, NULL, NULL, color);
            node->left = new_node;
            new_node->parent = node;
        }
        else
            treeSortAddNode(node->left, data, elem_size, cmp, color);
    }
    else {
        if (node->right == NULL){
            node_t * new_node = newNode(data, elem_size, NULL, NULL, color);
            node->right = new_node;
            new_node->parent = node;
        }
        else
            treeSortAddNode(node->right, data, elem_size, cmp, color);
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
    logPrint(LOG_DEBUG_PLUS, "bintree: destroying tree (subtree) with root at %p...\n", node);

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

    logPrint(LOG_DEBUG_PLUS, "bintree: destroyed tree (subtree) with root at %p\n", node);
}

node_t * treeCopy(node_t * node)
{
    if (node == NULL)
        return NULL;

    node_t * new_node = newNode(node->data, node->elem_size,
        treeCopy(node->left),
        treeCopy(node->right),
        node->color_for_dump);

    return new_node;
}

void treeDumpGraph(node_t * root_node, elemtostr_func_t elemToStr)
{
    assert(root_node);

    const int  IMG_WIDTH_IN_PERCENTS = 95;
    const int IMG_HEIGTH_IN_PERCENTS = 70;

    static size_t dump_count = 0;

    const size_t MAX_FILE_NAME = 256;
    char dot_file_name[MAX_FILE_NAME] = "";
    char img_file_name[MAX_FILE_NAME] = "";

    system("mkdir -p logs/dots/");
    system("mkdir -p logs/imgs/");

    sprintf(dot_file_name, "logs/dots/graph_%zu.dot", dump_count);
    sprintf(img_file_name, "logs/imgs/graph_%zu.svg", dump_count);

    FILE * dot_file = fopen(dot_file_name, "w");
    treeMakeDot(root_node, elemToStr, dot_file);
    fclose(dot_file);

    char sys_dot_cmd[MAX_FILE_NAME] = "";
    sprintf(sys_dot_cmd, "dot %s -Tsvg -o %s", dot_file_name, img_file_name);
    system(sys_dot_cmd);

    char img_file_name_log[MAX_FILE_NAME] = "";
    sprintf(img_file_name_log, "imgs/graph_%zu.svg", dump_count);
    logPrint(LOG_DEBUG, "<img src = %s width = \"%d%%\" height = \"%d%%\">",
                        img_file_name_log,
                        IMG_WIDTH_IN_PERCENTS,
                        IMG_HEIGTH_IN_PERCENTS);

    logPrint(LOG_DEBUG, "<hr>");

    dump_count++;
}

static void nodeMakeDot(node_t * node, elemtostr_func_t elemToStr, FILE * dot_file);

void treeMakeDot(node_t * node, elemtostr_func_t elemToStr, FILE * dot_file)
{
    assert(node);
    assert(dot_file);

    fprintf(dot_file, "digraph {\n");
    if (DUMP_mode == DUMP_MEDIUM || DUMP_mode == DUMP_SOFT)
        fprintf(dot_file, "splines = polyline\n");
    fprintf(dot_file, "node [style=filled,color=\"#000000\"]\n");

    nodeMakeDot(node, elemToStr, dot_file);

    fprintf(dot_file, "}\n");
}

static void dotPrintNode(FILE * dot_file, node_t * node, elemtostr_func_t elemToStr);

static void nodeMakeDot(node_t * node, elemtostr_func_t elemToStr, FILE * dot_file)
{
    assert(node);
    assert(dot_file);

    size_t node_num = (size_t)node;

    dotPrintNode(dot_file, node, elemToStr);

    if (node->parent != NULL){
        size_t node_parent_num = (size_t)(node->parent);

        if (node == node->parent->left)
            fprintf(dot_file, "node_%zu:f0->node_%zu;\n", node_parent_num, node_num);
        else
            fprintf(dot_file, "node_%zu:f1->node_%zu;\n", node_parent_num, node_num);
    }

    if (node->left  != NULL)
        nodeMakeDot(node->left,  elemToStr, dot_file);

    if (node->right != NULL)
        nodeMakeDot(node->right, elemToStr, dot_file);
}

static void dotPrintNode(FILE * dot_file, node_t * node, elemtostr_func_t elemToStr)
{
    size_t node_num = (size_t)node;

    const size_t MAX_ELEM_STR_LEN = 64;
    char elem_str[MAX_ELEM_STR_LEN] = "";
    elemToStr(elem_str, node->data);

    switch (DUMP_mode){
        case DUMP_HARD:
            fprintf(dot_file, "node_%zu"
                       "[shape=Mrecord,label="
                       "\"{node at %p | parent = %p | \\\"%s\\\" | {<f0> left = %p |<f1> right = %p}}\","
                       "fillcolor=\"#%08X\"];\n",
                       node_num, node, node->parent, elem_str, node->left, node->right, node->color_for_dump);
            break;

        case DUMP_MEDIUM:
            fprintf(dot_file, "node_%zu"
                       "[shape=Mrecord,label="
                       "\"{\\\"%s\\\" | {<f0> left|<f1> right}}\","
                       "fillcolor=\"#%08X\"];\n",
                       node_num, elem_str, node->color_for_dump);
            break;

        case DUMP_SOFT:
            fprintf(dot_file, "node_%zu"
                       "[shape=Mrecord,label="
                       "\"{<f0>\\\"%s\\\" | <f1>}\","
                       "fillcolor=\"#%08X\"];\n",
                       node_num, elem_str, node->color_for_dump);
            break;
    }
}

node_t * treeFindNode(node_t * node, void * data, compare_func_t cmp)
{
    assert(data);

    logPrint(LOG_DEBUG_PLUS, "finding node in tree (%p)...\n", node);

    if (node == NULL)
        return NULL;

    if (cmp(data, node->data) == 0)
        return node;

    node_t * next_node_result = treeFindNode(node->left, data, cmp);
    if (next_node_result != NULL)
        return next_node_result;

    next_node_result = treeFindNode(node->right, data, cmp);
    return next_node_result;
}


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
