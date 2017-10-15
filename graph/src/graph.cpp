//============================================================================
// Name        : graph.cpp
// Author      : anirudh
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Graph1.h"
using namespace std;

int main()
{

	Graph g(4);
	g.add_edge(0, 1);
	g.add_edge(0, 2);
	g.add_edge(1, 2);
	g.add_edge(2, 0);
	g.add_edge(2, 3);
	g.add_edge(3, 3);

	cout << "Depth is: ";
	g.DFS(2);
	cout << endl;
	g.BFS(2);
	cout << endl;
	if(g.search(0, 6))
		cout << "yes!" << endl;
	else
		cout << "No!" << endl;
	return 0;
}
