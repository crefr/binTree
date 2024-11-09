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

    for (int i = 0; i < 256; i++){
        int ans =  rand() % 256;
        treeSortAddNode(start_node, &ans, sizeof(ans), cmpInt);
    }

    printTree(start_node, printInt);
    treeDumpGraph(start_node, intToStr);

    putchar('\n');

    treeDestroy(start_node);
    logExit();
    return 0;
}
