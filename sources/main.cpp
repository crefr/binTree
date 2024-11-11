#include <stdio.h>
#include <stdlib.h>

#include "logger.h"
#include "bintree.h"

int main()
{
    system("mkdir -p logs/");
    logStart("logs/log.html", LOG_DEBUG, LOG_HTML);

    int start_val = 128;
    node_t * start_node = newNode(&start_val, sizeof(start_val));

    for (int i = 0; i < 8; i++){
        int ans =  rand() % 4;
        treeSortAddNode(start_node, &ans, sizeof(ans), cmpInt);
    }

    int find_ans = 2;
    node_t * search_node = treeFindNode(start_node, &find_ans, cmpInt);
    printTree(search_node, printInt);
    printf("\n");

    printTree(start_node, printInt);
    printf("\n");

    treeDumpGraph(start_node, intToStr);
    treeDumpGraph(search_node, intToStr);

    putchar('\n');

    treeDestroy(start_node);
    logExit();
    return 0;
}
