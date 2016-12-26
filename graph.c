//
// Created by Omri Carmi on 16/12/2016.
//

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include "set.h"
#include "graph.h"

typedef struct _Graph {
    PSet vertexElements;
    PSet edgeElements;
} Graph;

void VertexDestroy(PElem pVertex);

PElem VertexClone(PElem pElem);

/*****  Helper Functions *****/
//validate weight is legal
Bool isValidEdgeWeight(int weight){
    if(0 <= weight && weight <= 10){
        return TRUE;
    }else{
        return FALSE;
    }
}

/**
 * Pull out vertex with min distance from set of vertices
 * @param vertices - Set of vertice to check
 * @param dist - array of distance for the above Set
 * @return the vertex with min distance
 */
PVertex ExtractMinVertex(PSet vertices, int *dist){
    if(vertices == NULL || dist == NULL){
        return NULL;
    }
    PVertex pVertex = SetGetFirst(vertices);
    if(pVertex == NULL){
        return  NULL;
    }
    //init the min dist to the first vertex
    int min = dist[pVertex->serialNumber];
    PVertex pVertexMin = pVertex;

    //find min vertex
    while( (pVertex = SetGetNext(vertices)) != NULL ){
        if(min > dist[pVertex->serialNumber]){
            min = dist[pVertex->serialNumber];
            pVertexMin = pVertex;
        }
    }

    //extract min and return it
    PVertex pVertexMinClone = VertexClone(pVertexMin);
    if(pVertexMinClone == NULL){
        return NULL;
    }
    //remove the min vertex from set
    if( SetRemoveElement(vertices,pVertexMin) == FALSE){
        VertexDestroy(pVertexMinClone);
        return NULL;
    }
    return pVertexMinClone;
}

/*****  Vertex Functions *****/

/**
 * create a vertex
 * @param serialNumber - the desire serail num for the vertex
 * @return pointer to the created vertex
 */
PVertex VertexCreate(int serialNumber){

    if(serialNumber < 0){
        return NULL;
    }

    PVertex pVertex = (PVertex) malloc(sizeof(Vertex));
    if(pVertex == NULL){
        return NULL;
    }

    pVertex->serialNumber = serialNumber;
    return pVertex;
}

/**
 * destroy girven vertex
 * @param pVertex - pointer to vertex to destroy
 */
void VertexDestroy(PElem pVertex){

    if(pVertex == NULL){
        return;
    }

    free(pVertex);

}

/**
 * compare between 2 vertex if they have the same serial num
 * @param pElem1 - vertex 1 to compare
 * @param pElem2 - vertex 2 to compare
 * @return true if they are identical
 */
Bool VertexCompare(PElem pElem1, PElem pElem2){

    PVertex pVertex1 = pElem1;
    PVertex pVertex2 = pElem2;

    if(pVertex1 == NULL || pVertex2 == NULL){
        return FALSE;
    }

    if(pVertex1->serialNumber == pVertex2->serialNumber){
        return TRUE;
    }else{
        return FALSE;
    }
}

/**
 * create a copy of the given vertex
 * @param pElem - vertex to clone
 * @return the cloened vertex
 */
PElem VertexClone(PElem pElem){

    PVertex pVertex = pElem;

    if(pVertex == NULL){
        return NULL;
    }

    PVertex pVertexClone = VertexCreate(pVertex->serialNumber);
    if(pVertexClone == NULL){
        return NULL;
    }

    return pVertexClone;
}

/*****  Edge Functions *****/

/**
 * create Edge
 * @param weight - desired weight
 * @param serialNumber1 - of the vertex 1 in the edge
 * @param serialNumber2 - of the vertex 2 in the edge
 * @return a pointer to the created edge
 */
PEdge EdgeCreate(int weight, int serialNumber1, int serialNumber2){
    //TODO write this func again
    if( isValidEdgeWeight(weight) == FALSE || serialNumber1 == serialNumber2 ){
        return NULL;
    }

    PEdge pEdge = (PEdge) malloc(sizeof(Edge));
    if(pEdge == NULL){
        return NULL;
    }

    //creating 2 vertex
    PVertex pVertex1 = VertexCreate(serialNumber1);
    PVertex pVertex2 = VertexCreate(serialNumber2);
    if(pVertex1 == NULL || pVertex2 == NULL){
        //free what already allocated
        VertexDestroy(pVertex1);
        VertexDestroy(pVertex2);
        free(pEdge);
        return NULL;
    }

    //init Edge fields
    pEdge->weight = weight;
    pEdge->nodeA = pVertex1;
    pEdge->nodeB = pVertex2;

    return pEdge;
}

/**
 * destroy given edge
 * @param pElem pointer of edge to destroy
 */
void EdgeDestroy(PElem pElem){

    if(pElem == NULL){
        return;
    }

    PEdge pEdge = pElem;

    VertexDestroy(pEdge->nodeA);
    pEdge->nodeA = NULL;
    VertexDestroy(pEdge->nodeB);
    pEdge->nodeB = NULL;
    free(pEdge);
}

/**
 * compare if 2 edge have the same vertices
 * @param pElem1 - edge 1 to compare
 * @param pElem2 - edge 2 to compare
 * @return
 */
Bool EdgeCompare(PElem pElem1, PElem pElem2) {

    PEdge pEdge1 = pElem1;
    PEdge pEdge2 = pElem2;

    if(pEdge1 == NULL || pEdge2 == NULL){
        return FALSE;
    }

    //compare nodes between edges
    if(VertexCompare(pEdge1->nodeA, pEdge2->nodeA) == FALSE &&
            VertexCompare(pEdge1->nodeA, pEdge2->nodeB) == FALSE){
        return FALSE;
    }
    if(VertexCompare(pEdge1->nodeB, pEdge2->nodeA) == FALSE &&
            VertexCompare(pEdge1->nodeB, pEdge2->nodeB) == FALSE){
        return FALSE;
    }

    //got here just if nodes identical
    return TRUE;
}

/**
 * create a clone of a given edge
 * @param pElem - pointer to edge to clone
 * @return pointer to the created copy
 */
PElem EdgeClone(PElem pElem) {

    PEdge pEdge = pElem;

    if(pEdge == NULL){
        return NULL;
    }

    PEdge pEdgeClone = EdgeCreate(pEdge->weight, pEdge->nodeA->serialNumber, pEdge->nodeB->serialNumber);
    if (pEdgeClone == NULL){
        return NULL;
    }

    return pEdgeClone;
}

/***** Graph Helper methods *****/
/**
 * decide if given set contain vertex with given serial num
 * @param vertexList - Set to check in
 * @param serialNumber - serial num to look for
 * @return ture if such vertex exist
 */
Bool isVertexExist(PSet vertexList,int serialNumber){
    if(vertexList == NULL){
        return FALSE;
    }

    PVertex pVertex = VertexCreate(serialNumber);
    if(pVertex == NULL){
        return FALSE;
    }

    if(SetFindElement(vertexList,pVertex) != NULL){
        VertexDestroy(pVertex);
        return TRUE;
    }else{
        VertexDestroy(pVertex);
        return FALSE;
    }
}

/**
 * deicde if given set contain edge with specific 2 vertices
 * @param edgeList - set to check in
 * @param serialNumber1 - of vertex to look for
 * @param serialNumber2 - of vertex to look for
 * @return true is such edge exist
 */
Bool isEdgeExist(PSet edgeList, int serialNumber1, int serialNumber2){
    if(edgeList == NULL){
        return FALSE;
    }
    PEdge pEdge = EdgeCreate(0,serialNumber1,serialNumber2);
    if(pEdge == NULL){
        return FALSE;
    }

    if(SetFindElement(edgeList,pEdge) != NULL){
        EdgeDestroy(pEdge)  ;
        return TRUE;
    }else{
        EdgeDestroy(pEdge)  ;
        return FALSE;
    }
}

/**
 * calculate edge wieght in a given graph
 * @param pGraph - graph to check
 * @param vertex1 - vertex 1 in the edge
 * @param vertex2 - vertex 2 in the edge
 * @return the weight of the edge
 *         (-1) - if the dge dont exust or if the func fail
 */
int GraphGetEdgeWeight(PGraph pGraph, int vertex1, int vertex2){
    if( pGraph == NULL){
        return -1;
    }
    PEdge pEdgeTmp = EdgeCreate(0, vertex1, vertex2);
    PEdge pEdge = SetFindElement(pGraph->edgeElements,pEdgeTmp); // 0 is arbitrary
    if(pEdge == NULL){
        EdgeDestroy(pEdgeTmp);
        return -1;
    }else{
        EdgeDestroy(pEdgeTmp);
        return pEdge->weight;
    }
}

/********* Interface impl **************/

/**
 * create an empty graph
 * @return pointer to the graph
 */
PGraph GraphCreate(){

    PGraph pGraph = (PGraph)malloc(sizeof(Graph));
    if(pGraph == NULL){
        return NULL;
    }

    pGraph->vertexElements = SetCreate(VertexCompare, VertexClone, VertexDestroy);
    pGraph->edgeElements = SetCreate(EdgeCompare, EdgeClone, EdgeDestroy);
    if(pGraph->vertexElements == NULL || pGraph->edgeElements == NULL){
        GraphDestroy(pGraph);
        return NULL;
    }

    return pGraph;
}

/**
 * Destroy a given graph and all of his vertices and edges
 * @param pGraph - to destroy
 */
void GraphDestroy(PGraph pGraph){
    if(pGraph == NULL){
        return;
    }

    SetDestroy(pGraph->vertexElements);
    SetDestroy(pGraph->edgeElements);

    free(pGraph);
}

/**
 * create vertex with given serial num and add it to a given graph
 * @param pGraph - to add to
 * @param serialNumber - desired for the new vertex
 * @return true if success to add vertex else return false
 */
Bool GraphAddVertex(PGraph pGraph, int serialNumber){

    if(pGraph == NULL){
        return FALSE;
    }
    if(serialNumber != SetGetSize(pGraph->vertexElements)){
        return FALSE;
    }

    //create new Vertex
    PVertex pVertexTmp = VertexCreate(serialNumber);
    if(pVertexTmp == NULL){
        return FALSE;
    }
    //add it to the vertex set
    if(SetAdd(pGraph->vertexElements,pVertexTmp) == FALSE){
        return FALSE;
    }

    //free the tmp vertex after was added
    VertexDestroy(pVertexTmp);

    //got here if vertex added
    return TRUE;
}
/**
 * create edge with given params and add it to a given graph
 * @param pGraph - to add to
 * @param vertex1 - serial of 1 vertex
 * @param vertex2 - serial of 2 vertex
 * @param weight - of the edge
 * @return true if success to add edge else return false
 */
Bool GraphAddEdge(PGraph pGraph, int vertex1, int vertex2, int weight){
    if(pGraph == NULL){
        return FALSE;
    }
    if(isValidEdgeWeight(weight) == FALSE){
        return FALSE;
    }

    //validate that the vertex exist in graph
    if(isVertexExist(pGraph->vertexElements,vertex1) == FALSE ||
            isVertexExist(pGraph->vertexElements,vertex2) == FALSE){
        return FALSE;
    }

    //check if edge already exist in graph
    if(isEdgeExist(pGraph->edgeElements,vertex1,vertex2) == TRUE){
        return FALSE;
    }

    //create edge
    PEdge pEdge = EdgeCreate(weight,vertex1,vertex2);
    if(pEdge == NULL){
        return FALSE;
    }

    //add the edge to the graph
    if(SetAdd(pGraph->edgeElements,pEdge) == FALSE){
        EdgeDestroy(pEdge);
        return FALSE;
    }

    //got here if add was success
    EdgeDestroy(pEdge);
    return TRUE;
}

/**
 * calculate who is the neughbors of a vertex with the given serial num
 * @param pGraph - to search in
 * @param serialNumber - of the vertex who we search his neighbors
 * @return pointer to a Set contains the founded neighbors vertices
 */
PSet GraphNeighborVertices(PGraph pGraph, int serialNumber){

    //validate param
    if(pGraph == NULL || serialNumber < 0){
        return NULL;
    }

    //validate vertex exist
    if(isVertexExist(pGraph->vertexElements,serialNumber) == FALSE){
        return NULL;
    }

    //init place to keep all neighbors
    PSet vertexList = SetCreate(VertexCompare,VertexClone,VertexDestroy);
    if(vertexList == NULL){
        return NULL;
    }

    //iterate over all vertex and if they are neighbor to source add them to neighbor list
    int vertexSize = SetGetSize(pGraph->vertexElements);
    int i = 0;
    for (i = 0; i < vertexSize; ++i) {
        //check if they are neighbors
        if(isEdgeExist(pGraph->edgeElements,serialNumber,i) == TRUE){
            //create vertex to add to the list
            PVertex pVertexTmp = VertexCreate(i);
            if(pVertexTmp == NULL){
                SetDestroy(vertexList);
                return NULL;
            }
            //add it to the list
            if( SetAdd(vertexList,pVertexTmp) == FALSE ){
                SetDestroy(vertexList);
                VertexDestroy(pVertexTmp);
                return NULL;
            }
            //free the tmp vertex after was added to list
            VertexDestroy(pVertexTmp);
        }
    }

    //return the neighbor list
    return vertexList;
}

/**
 * implemention of Dijkstra algorithm - find to shortest path to each vertex in the graph from given soure
 * @param pGraph - to search in
 * @param source - serial num of vertex to start from
 * @param dist - array of distances
 * @param prev - prev[i] holds the vertex who led to vertex i in his shortest path
 * @return if algo finished succesfuly
 */
Bool GraphFindShortestPath(PGraph pGraph, int source, int* dist, int* prev){
    if(pGraph == NULL || source < 0 || dist == NULL || prev == NULL){
        return FALSE;
    }
    int numOfVertices = GraphGetNumberOfVertices(pGraph);
    if( source >= numOfVertices ){
        return FALSE;
    }

    PSet Q = SetCreate(VertexCompare,VertexClone,VertexDestroy);
    if(Q == NULL){
        return FALSE;
    }

    int i = 0;
    for (i = 0; i < numOfVertices; ++i) {

        //init the prev array to -1 and dist infinity
        prev[i] = -1;
        dist[i] = INT_MAX;

        //create vertex to add to the list
        PVertex pVertex = VertexCreate(i);
        if(pVertex == NULL){
            SetDestroy(Q);
            return FALSE;
        }
        //add it to the list
        if( SetAdd(Q,pVertex) == FALSE ){
            SetDestroy(Q);
            VertexDestroy(pVertex);
            return FALSE;
        }

        //free the copy vertex
        VertexDestroy(pVertex);
    }

    //init source dist to 0 and prev to itself
    dist[source] = 0;
    prev[source] = source;

    while(SetGetSize(Q) > 0 ){
        //extract min from Q
        PVertex u = ExtractMinVertex(Q,dist);
        if(u == NULL){
            SetDestroy(Q);
            return FALSE;
        }

        //get list of u neighbors
        PSet neighbors = GraphNeighborVertices(pGraph,u->serialNumber);
        if(neighbors == NULL){
            VertexDestroy(u);
            SetDestroy(Q);
            return FALSE;
        }
        // check first neighbor and update their dist and prev if needed
        PVertex v = SetGetFirst(neighbors);
        if(v == NULL){
            SetDestroy(neighbors);
            VertexDestroy(u);
            continue;
        }else{
            int uvEdgeWeight = GraphGetEdgeWeight(pGraph,u->serialNumber,v->serialNumber);
            if( uvEdgeWeight == -1) {
                VertexDestroy(u);
                SetDestroy(Q);
                SetDestroy(neighbors);
                return FALSE;
            }
            int alt = dist[u->serialNumber] + uvEdgeWeight;
            if(alt < dist[v->serialNumber] ){
                dist[v->serialNumber] = alt;
                prev[v->serialNumber] = u->serialNumber;
            }
        }
        //iterate over neighbor and update their dist and prev if needed
        while( (v = SetGetNext(neighbors)) != NULL ){
            int uvEdgeWeight = GraphGetEdgeWeight(pGraph,u->serialNumber,v->serialNumber);
            if( uvEdgeWeight == -1) {
                VertexDestroy(u);
                SetDestroy(Q);
                SetDestroy(neighbors);
                return FALSE;
            }
            int alt = dist[u->serialNumber] + uvEdgeWeight;
            if(alt < dist[v->serialNumber] ){
                dist[v->serialNumber] = alt;
                prev[v->serialNumber] = u->serialNumber;
            }
        }

        //after finish work on neighbors of u free them
        VertexDestroy(u);
        SetDestroy(neighbors);
    }

    //got here if algo finished successful
    SetDestroy(Q);
    return TRUE;
}

/**
 * calculate the amount of edge s in the graph
 * @param pGraph
 * @return amount of edges in graph
 */
int GraphGetNumberOfEdges(PGraph pGraph){

    if(pGraph == NULL){
        return 0;
    }

    return SetGetSize(pGraph->edgeElements);
}
/**
 * calculate the amount of vertices in the graph
 * @param pGraph
 * @return amount of vertices in the graph
 */
int GraphGetNumberOfVertices(PGraph pGraph){

    if(pGraph == NULL){
        return 0;
    }

    return SetGetSize(pGraph->vertexElements);
}

/**
 * @param pGraph
 * @return the vertices Set of the given graph
 */
PSet GraphVerticesStatus(PGraph pGraph){

    if(pGraph == NULL){
        return NULL;
    }

    return pGraph->vertexElements;
}
r/**
 * @param pGraph
 * @return the edges Set of the given graph
 */
PSet GraphEdgesStatus(PGraph pGraph){

    if(pGraph == NULL){
        return NULL;
    }

    return pGraph->edgeElements;
}

///*****  Test Functions *****/
//
///*****  Test Helper *****/
//void test_isValidEdgeWeight(){
//    assert(FALSE == isValidEdgeWeight(-1567));
//    assert(FALSE == isValidEdgeWeight(-10));
//    assert(FALSE == isValidEdgeWeight(-3));
//    assert(FALSE == isValidEdgeWeight(-1));
//
//    assert(TRUE == isValidEdgeWeight(0));
//    assert(TRUE == isValidEdgeWeight(3));
//    assert(TRUE == isValidEdgeWeight(10));
//
//    assert(FALSE == isValidEdgeWeight(11));
//    assert(FALSE == isValidEdgeWeight(15));
//    assert(FALSE == isValidEdgeWeight(120));
//    assert(FALSE == isValidEdgeWeight(1345));
//
//    printf("%s : OK\n",__FUNCTION__);
//}
//
///*****  Test Vertex*****/
//void test_VertexCreate(){
//
//    assert(NULL != VertexCreate(0));
//    assert(NULL != VertexCreate(1));
//    assert(NULL != VertexCreate(5));
//    assert(NULL != VertexCreate(10));
//    assert(NULL != VertexCreate(1000));
//
//    assert(NULL == VertexCreate(-1));
//    assert(NULL == VertexCreate(-2));
//    assert(NULL == VertexCreate(-5));
//    assert(NULL == VertexCreate(-10));
//    assert(NULL == VertexCreate(-1000));
//
//    PVertex pVertex = VertexCreate(1);
//    assert(1 == pVertex->serialNumber);
//    pVertex = VertexCreate(0);
//    assert(0 == pVertex->serialNumber);
//    pVertex = VertexCreate(10);
//    assert(10 == pVertex->serialNumber);
//    pVertex = VertexCreate(1000);
//    assert(1000 == pVertex->serialNumber);
//    pVertex = VertexCreate(5);
//    assert(5 == pVertex->serialNumber);
//    printf("%s : OK\n",__FUNCTION__);
//}
//
//void test_VertexDestroy(){
//
//    //FIXME not good check decide replacement
////    PVertex pVertex = VertexCreate(1);
////    VertexDestroy(pVertex);
////    assert(NULL == pVertex);
//
//    VertexDestroy(NULL);
//    printf("%s : OK\n",__FUNCTION__);
//}
//
//void test_VertexCompare(){
//
//    PVertex pVertex1a  = VertexCreate(1);
//    PVertex pVertex1b  = VertexCreate(1);
//    PVertex pVertex2a  = VertexCreate(2);
//    PVertex pVertex2b  = VertexCreate(2);
//
//    assert( FALSE == VertexCompare(NULL,NULL));
//    assert( FALSE == VertexCompare(pVertex1a,NULL));
//    assert( FALSE == VertexCompare(NULL,pVertex1a));
//
//    assert( FALSE == VertexCompare(pVertex1a,pVertex2a));
//    assert( FALSE == VertexCompare(pVertex2a,pVertex1a));
//    assert( FALSE == VertexCompare(VertexCreate(10),VertexCreate(105)));
//
//    assert( TRUE == VertexCompare(pVertex1a,pVertex1a));
//    assert( TRUE == VertexCompare(pVertex1a,pVertex1b));
//    assert( TRUE == VertexCompare(pVertex2a,pVertex2a));
//    assert( TRUE == VertexCompare(pVertex2a,pVertex2b));
//    assert( TRUE == VertexCompare(VertexCreate(10),VertexCreate(10)));
//
//    printf("%s : OK\n",__FUNCTION__);
//}
//
//void test_VertexClone(){
//
//    assert( NULL == VertexClone(NULL));
//
//    PVertex pv1 =VertexCreate(1);
//    PVertex pv2 = VertexClone(pv1);
//    assert( 1 == pv2->serialNumber);
//    assert( pv1 != pv2 );
//
//    pv1 =VertexCreate(-1);
//    pv2 = VertexClone(pv1);
//    assert( pv2 == NULL );
//
//
//    for (int i = 0; i < 100; ++i) {
//        pv1 =VertexCreate(i);
//        pv2 = VertexClone(pv1);
//        assert( i == pv2->serialNumber);
//    }
//
//    printf("%s : OK\n",__FUNCTION__);
//}
//
///***** Test Edge *****/
//void test_EdgeCreate(){
//
//    assert(NULL == EdgeCreate(0,0,0));
//    assert(NULL == EdgeCreate(1,3,3));
//    assert(NULL == EdgeCreate(-1,1,2));
//    assert(NULL == EdgeCreate(-2,1,2));
//    assert(NULL == EdgeCreate(-1,1,2));
//    assert(NULL == EdgeCreate(1,-1,2));
//    assert(NULL == EdgeCreate(1,1,-2));
//
//    assert(NULL != EdgeCreate(0,1,2));
//    assert(NULL != EdgeCreate(10,1,2));
//    assert(NULL != EdgeCreate(1,1,2));
//    assert(NULL != EdgeCreate(5,1,2));
//
//    PEdge pEdge = EdgeCreate(3,1,2);
//    assert(1 == pEdge->nodeA->serialNumber);
//    assert(2 == pEdge->nodeB->serialNumber);
//    assert(3 == pEdge->weight);
//
//    printf("%s : OK\n",__FUNCTION__);
//}
//
//void test_EdgeDestroy(){
//
//    //FIXME not good check decide replacement
//    PEdge pEdge = EdgeCreate(3,1,2);
//    assert( NULL != pEdge );
//    assert( NULL != pEdge->nodeA );
//    assert( NULL != pEdge->nodeB );
//
//    EdgeDestroy(pEdge);
//    assert( NULL == pEdge->nodeA );
//    assert( NULL == pEdge->nodeB );
//
//    printf("%s : OK\n",__FUNCTION__);
//}
//
//void test_EdgeCompare(){
//
//    assert( FALSE == EdgeCompare(EdgeCreate(1,0,0),EdgeCreate(1,0,0)));
//    assert( FALSE == EdgeCompare(EdgeCreate(1,1,0),EdgeCreate(1,1,2)));
//    assert( FALSE == EdgeCompare(EdgeCreate(1,0,1),EdgeCreate(1,1,2)));
//    assert( FALSE == EdgeCompare(EdgeCreate(1,0,1),EdgeCreate(1,2,1)));
//    assert( FALSE == EdgeCompare(EdgeCreate(1,1,0),EdgeCreate(1,2,1)));
//    assert( FALSE == EdgeCompare(EdgeCreate(1,1,0),NULL));
//    assert( FALSE == EdgeCompare(NULL,EdgeCreate(1,2,1)));
//    assert( FALSE == EdgeCompare(NULL,NULL));
//
//    assert( TRUE == EdgeCompare(EdgeCreate(1,1,0),EdgeCreate(1,0,1)));
//    assert( TRUE == EdgeCompare(EdgeCreate(1,1,0),EdgeCreate(3,0,1)));
//    assert( TRUE == EdgeCompare(EdgeCreate(1,1,0),EdgeCreate(3,1,0)));
//
//    PEdge pEdge110 = EdgeCreate(1,1,0);
//    assert( TRUE == EdgeCompare(pEdge110,pEdge110));
//
//    printf("%s : OK\n",__FUNCTION__);
//}
//
//void test_EdgeClone(){
//
//    assert( NULL == EdgeClone(NULL));
//
//    PEdge pEdge = EdgeClone(EdgeCreate(3,1,2));
//    assert(1 == pEdge->nodeA->serialNumber);
//    assert(2 == pEdge->nodeB->serialNumber);
//    assert(3 == pEdge->weight);
//
//    printf("%s : OK\n",__FUNCTION__);
//}
//
///***** Test Graph Helper *****/
//
//void test_isVertexExist(){
//
//    PSet vertexList = GraphCreate()->vertexElements;
//    for (int i = 0; i < 100; ++i) {
//        SetAdd(vertexList,VertexCreate(i));
//    }
//    for (int i = 0; i < 100; ++i) {
//        assert( TRUE == isVertexExist(vertexList,i));
//    }
//    for (int i = 100; i < 200; ++i) {
//        assert( FALSE == isVertexExist(vertexList,i));
//    }
//    for (int i = -100; i < -1; ++i) {
//        assert( FALSE == isVertexExist(vertexList,i));
//    }
//
//    for (int i = 0; i < 100; i=+4) {
//        SetRemoveElement(vertexList,VertexCreate(i));
//    }
//
//
//    printf("%s : OK\n",__FUNCTION__);
//}
//
//




