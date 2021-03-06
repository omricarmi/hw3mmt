/*
 * graph.h
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include "set.h"

typedef struct _Graph* PGraph;

typedef struct _Vertex
{
	int serialNumber;
} Vertex;

typedef struct _Edge
{
	struct _Vertex* nodeA;
	struct _Vertex* nodeB;
	int weight;
} Edge;



typedef struct _Vertex* PVertex;
typedef struct _Edge* PEdge;

PGraph GraphCreate();
void GraphDestroy(PGraph);
Bool GraphAddVertex(PGraph, int);
Bool GraphAddEdge(PGraph pGraph, int vertex1, int vertex2, int weight);
PSet GraphNeighborVertices(PGraph, int);
Bool GraphFindShortestPath(PGraph pGraph, int source, int* dist, int* prev);

int GraphGetNumberOfEdges(PGraph);
int GraphGetNumberOfVertices(PGraph);

PSet GraphVerticesStatus(PGraph);
PSet GraphEdgesStatus(PGraph);

/***** DEBUG *****/
void test_isValidEdgeWeight();
void test_isVertexExist();

void test_VertexCreate();
void test_VertexDestroy();
void test_VertexCompare();
void test_VertexClone();

void test_EdgeCreate();
void test_EdgeDestroy();
void test_EdgeCompare();
void test_EdgeClone();

#endif /* GRAPH_H_ */
