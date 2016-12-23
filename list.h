/*
 * list.h
 *
 */
#ifndef _LIST_T_H_
#define _LIST_T_H_


typedef struct List_* PList;
typedef void* PElem;
typedef enum{FAIL, SUCCESS} Result;

/*User functions*/
typedef PElem (*CLONE_FUNC)(PElem);
typedef void (*DESTROY_FUNC)(PElem);

/*Interface functions*/
PList ListCreate(CLONE_FUNC, DESTROY_FUNC);
void ListDestroy(PList);
Result ListAdd(PList, PElem);
PElem ListGetFirst(PList);
PElem ListGetNext(PList);

Result ListRemove(PList);
int ListGetSize(PList);

/* TESTS DECLARATIONS */
void listCreateTest();
typedef struct _TstElement *PTstElement,TstElement;
PTstElement createTstElement(int size);
void destroyFunc(PTstElement pTstElement);
PTstElement cloneFunc(PTstElement pTstElement);
//void destroyFunc(PTstElement pTstElement);
void listAddFirstNextTest();
void test_ListRemove();
#endif /* _LIST_T_H_ */