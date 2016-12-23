//TODO - some general questions - when am i creating with malloc and when with create
// Created by harel on 12/15/2016.
//typedef struct List_* PList;#
#include <stdio.h>
#include <stdlib.h>
#include "list.h"

typedef struct _Node{
    PElem pElem;
    struct _Node *pNext;
} *PNode, Node;

typedef struct List_{
    PNode pFirstNode;
    PNode pIterator;
    int listSize;
    CLONE_FUNC cloneFunc;
    DESTROY_FUNC destroyFunc;
} List;

// are those next three needed?
//typedef void* PElem;
//typedef PElem (*CLONE_FUNC)(PElem);
//typedef void (*DESTROY_FUNC)(PElem);
//typedef enum{FAIL, SUCCESS} Result;

/*User functions*/

/*Interface functions*/
PList ListCreate(CLONE_FUNC cloneFunc, DESTROY_FUNC destroyFunc){ // we added the instances
    if(cloneFunc == NULL || destroyFunc == NULL){
        return NULL;
    }
    PList pList = (PList)malloc(sizeof(List)); //TODO PList or PList*
    if(pList != NULL){
        pList->pFirstNode = NULL;
        pList->pIterator = NULL;
        pList->cloneFunc = cloneFunc;
        pList->destroyFunc = destroyFunc;
        pList->listSize = 0;
    }
    return pList;
}

void ListDestroy(PList pList){
    if(pList == NULL){
        return;
    }
    DESTROY_FUNC destroyFunc = pList->destroyFunc; // TODO is there another way to use pointer to func
    PNode pNodeNext = pList->pFirstNode;
    while(pNodeNext != NULL){
        PNode pCurrentNode = pNodeNext;
        pNodeNext = pNodeNext->pNext;
        destroyFunc(pCurrentNode->pElem);
        free(pCurrentNode);
    }
    free(pList);// TODO - do i care about the itarator/? - omri: I think not bcause its just pointer - so what you wrote is fine
}
Result ListAdd(PList pList,PElem pElem){
    if(pList == NULL || pElem == NULL){
        return FAIL;
    }
    CLONE_FUNC cloneFunc = pList->cloneFunc;
    PElem pElemNew = cloneFunc(pElem);
    if(pElemNew == NULL){
        return FAIL;
    }
    PNode pNodeNew = (PNode)malloc(sizeof(Node));
    if(pNodeNew == NULL){
        return FAIL;
    }
    pNodeNew->pElem = pElemNew;
    pNodeNew->pNext = pList->pFirstNode; // even if there are no nodes yet, the added one next's will be null
    pList->pFirstNode = pNodeNew;
    pList->pIterator = NULL;
    pList->listSize ++; // TODO - check sintax correctness
    return SUCCESS;
}

PElem ListGetFirst(PList pList){
    if(pList == NULL || pList->pFirstNode == NULL){
        return NULL;
    }
    pList->pIterator = pList->pFirstNode;
    return pList->pIterator->pElem;
}

PElem ListGetNext(PList pList){
    if(pList == NULL || pList->pIterator == NULL){// second cond for not even trying to check iterator fields in case hes null
        return NULL;
    }
    if(pList->pIterator->pNext == NULL){ // meaning we are in the end of the list.
        return NULL;
    }
    pList->pIterator = pList->pIterator->pNext;
    return pList->pIterator->pElem;
}

Result ListRemove(PList pList){
    if(pList == NULL || pList->pIterator == NULL){
        return FAIL;
    }
    DESTROY_FUNC destroyFunc = pList->destroyFunc;
    PNode pNodeTmp = pList->pFirstNode;//tmp node for running on tje list

    if (pNodeTmp == pList->pIterator){// if the iterator point on the first node remove it immidiately
        pList->pFirstNode = pList->pFirstNode->pNext;
        destroyFunc(pNodeTmp->pElem);
        free(pNodeTmp);
        pList->pIterator = NULL;
        pList->listSize --;
        return SUCCESS;
    }
    // in case the iterator point's a node somewhere else in the list

    while(pNodeTmp->pNext != NULL){
        if(pNodeTmp->pNext == pList->pIterator){
            pNodeTmp->pNext = pNodeTmp->pNext->pNext;
            destroyFunc(pList->pIterator->pElem);
            free(pList->pIterator);
            pList->pIterator = NULL;
            pList->listSize --;
            return SUCCESS;
        }
        pNodeTmp = pNodeTmp->pNext;
    }
    return FAIL;
}
int ListGetSize(PList pList){
    if(pList == NULL){
        return -1; // TODO make sure we accept this well
    }
    return pList->listSize;
}


////*************** TESTS ZONE ***************//
//
//typedef struct _TstElement {
//    int size;
//}*PTstElement,TstElement;
//
//PTstElement createTstElement(int size){
//    PTstElement pTstElement = (PTstElement)malloc(sizeof(TstElement));
//    if(pTstElement == NULL){
//        return NULL;
//    }
//    pTstElement->size = size;
//    return pTstElement;
//}
//PTstElement cloneFunc(PTstElement pTstElement){
////void* cloneFunc(PTstElement pTstElement){
//    PTstElement pTstElementNew = (PTstElement)malloc(sizeof(TstElement));
//    if (pTstElementNew == NULL){
//        return NULL;
//    }
//    pTstElementNew->size = pTstElement->size;
//    return pTstElementNew;
//}
//void destroyFunc(PTstElement pTstElement){
//    if(pTstElement == NULL){
//        return;
//    }
//    free(pTstElement);
//    return;
//}
//void listCreateTest(){
//    PList pTstList = ListCreate(cloneFunc,destroyFunc);
//    assert(pTstList->listSize == 0);
//    assert(pTstList->pIterator == NULL);
//    assert(pTstList->cloneFunc != NULL);
//    assert(pTstList->pFirstNode == NULL);
//    printf("create is ok\n");
//    ListDestroy(pTstList);
//    //assert(pTstList == NULL); TODO - how to test destroy
//}
//
//void listAddFirstNextTest(){
//    PList pTstList = ListCreate(cloneFunc,destroyFunc);
//
//    //ListAdd(pTstList,createTstElement(3));
//    assert(SUCCESS == ListAdd(pTstList,createTstElement(5)));
//    assert(SUCCESS == ListAdd(pTstList,createTstElement(4)));
//    assert(SUCCESS == ListAdd(pTstList,createTstElement(3)));
//    assert(SUCCESS == ListAdd(pTstList,createTstElement(2)));
//    PTstElement pTstElementFirst = ListGetFirst(pTstList);
//    assert(2 == pTstElementFirst->size);
//    PTstElement pTstElementNext = ListGetNext(pTstList);
//    assert(3 == pTstElementNext->size);
//    pTstElementNext = ListGetNext(pTstList);
//    assert(4 == pTstElementNext->size);
//    pTstElementNext = ListGetNext(pTstList);
//    assert(5 == pTstElementNext->size);
//    printf("Add First Next ok\n");
//    // tests for get size
//    int cur_size = ListGetSize(pTstList);
//    assert(4 == cur_size);
//    PList pTstList2 = ListCreate(cloneFunc,destroyFunc);
//    cur_size = ListGetSize(pTstList2);
//    assert(0 == cur_size);
//    printf("ListGetSize ok\n");
//
//    // tests for remove, the iterator now is on the 4th and last node
//    ListRemove(pTstList);
//  //  ListRemove(pTstList);
//  //  ListRemove(pTstList);
//    assert(3 == ListGetSize(pTstList));
//    printf("ListRemove ok\n");
//    ListDestroy(pTstList);
//    ListDestroy(pTstList2);
//}
//
///*****  Vertex Functions *****/
//
//PVertex VertexCreateT(int serialNumber){
//
//    if(serialNumber < 0){
//        return NULL;
//    }
//
//    PVertex pVertex = (PVertex) malloc(sizeof(Vertex));
//    if(pVertex == NULL){
//        return NULL;
//    }
//
//    pVertex->serialNumber = serialNumber;
//    return pVertex;
//}
//
//void VertexDestroyT(PElem pVertex){
//
//    if(pVertex == NULL){
//        return;
//    }
//
//    free(pVertex);
//
//}
//
//Bool VertexComparetForT(PElem pElem1, PElem pElem2){
//
//    PVertex pVertex1 = pElem1;
//    PVertex pVertex2 = pElem2;
//
//    if(pVertex1 == NULL || pVertex2 == NULL){
//        return FALSE;
//    }
//
//    if(pVertex1->serialNumber == pVertex2->serialNumber){
//        return TRUE;
//    }else{
//        return FALSE;
//    }
//}
//
//PElem VertexCloneForT(PElem pElem){
//
//    PVertex pVertex = pElem;
//
//    if(pVertex == NULL){
//        return NULL;
//    }
//
//    PVertex pVertexClone = VertexCreate(pVertex->serialNumber);
//    if(pVertexClone == NULL){
//        return NULL;
//    }
//
//    return pVertexClone;
//}
//
//void test_ListRemove(){
//    PList pList = ListCreate(VertexCloneForT,VertexDestroyT);
//    assert(pList != NULL);
//
//    ListAdd(pList,VertexCreateT(1));
//    assert(NULL != pList->pFirstNode->pElem);
//
//    ListAdd(pList,VertexCreateT(1));
//    assert(NULL != pList->pFirstNode->pNext->pElem);
//
//    ListAdd(pList,VertexCreateT(1));
//    assert(NULL != pList->pFirstNode->pNext->pNext->pElem);
//    assert(NULL == pList->pFirstNode->pNext->pNext->pNext);
//
//    printf("%s : OK\n",__FUNCTION__);
//}
