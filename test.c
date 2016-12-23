//
// Created by Omri Carmi on 16/12/2016.
//

#include "graph.h"
#include <stdio.h>

int main() {

    printf("LIST:\n");
    test_ListRemove();

    printf("HELPER:\n");
    test_isValidEdgeWeight();
    test_isVertexExist();

    printf("VERTEX:\n");
    test_VertexCreate();
    test_VertexDestroy();
    test_VertexCompare();
    test_VertexClone();

    printf("EDGE:\n");
    test_EdgeCreate();
    test_EdgeDestroy();
    test_EdgeCompare();
    test_EdgeClone();
}