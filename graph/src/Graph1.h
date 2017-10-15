/*
 * Graph.h
 *
 *  Created on: Jun 23, 2017
 *      Author: krishnasaianirudhkatamreddy
 */

#ifndef GRAPH1_H_
#define GRAPH1_H_

#include <iostream>
#include <list>

using namespace std;

class Graph
{
public:
	Graph();
	Graph(int v);
	void add_edge(int v, int w);
	void DFS(int v);     // Depth first search for particular vertex.
	void BFS(int v);      // Breadth first search for all the Graph.
	bool search(int start, int end);

private:
	int v;
	list<int> *adj;
	void DFSUntil(int v, bool visited[]);
};



#endif /* GRAPH1_H_ */
