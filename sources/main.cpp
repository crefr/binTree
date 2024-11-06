#include <stdio.h>
#include <stdlib.h>

#include "logger.h"
#include "bintree.h"

int main()
{
    system("mkdir -p logs/");
    logStart("logs/log.html", LOG_DEBUG, LOG_HTML);
    int a = 52;
    int b = 53;
    int c = 54;
    node_t * node_1 = newNode(&a, sizeof(a));
    node_t * node_2 = newNode(&b, sizeof(b));
    node_t * node_3 = newNode(&c, sizeof(c));

    node_1->left =  node_2;
    node_1->right = node_3;

    printTree(node_1, printInt);
    putchar('\n');
    logExit();
    return 0;
}
