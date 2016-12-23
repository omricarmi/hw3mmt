//
// Created by Omri Carmi on 16/12/2016.
//

#include "list.h"

PList ListCreate(CLONE_FUNC c, DESTROY_FUNC d){}
void ListDestroy(PList p){}
Result ListAdd(PList p1, PElem p2){}
PElem ListGetFirst(PList p){}
PElem ListGetNext(PList p){}

Result ListRemove(PList p){}
int ListGetSize(PList p){}