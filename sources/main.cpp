#include <stdio.h>
#include <stdlib.h>

#include "logger.h"
#include "bintree.h"

int main()
{
    system("mkdir -p logs/");
    logStart("logs/log.html", LOG_DEBUG, LOG_HTML);

    int start_val = 50;
    node_t * start_node = newNode(&start_val, sizeof(start_val));
    int a = 25;
    int b = 75;
    int c = 100;
    treeSortAddNode(start_node, &a, sizeof(a), cmpInt);
    treeSortAddNode(start_node, &b, sizeof(b), cmpInt);
    treeSortAddNode(start_node, &c, sizeof(c), cmpInt);

    printTree(start_node, printInt);
    treeDumpGraph(start_node, intToStr);

    putchar('\n');

    treeDestroy(start_node);
    logExit();
    return 0;
}
