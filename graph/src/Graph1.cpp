/*
s * Graph1.cpp

 *
 *  Created on: Jun 23, 2017
 *      Author: krishnasaianirudhkatamreddy
 */

#include "Graph1.h"
#include <iterator>
#include <deque>

Graph::Graph() : v(0), adj(nullptr) {}

Graph::Graph(int v) : v(v), adj(new list<int>[v]) {}


void Graph::add_edge(int v, int w)
{
	adj[v].push_back(w);
}

void Graph::DFS(int s)
{
	bool *visited = new bool[v];

	for(int i = 0; i < v; i++)
	{
		visited[i] = false;
	}

	DFSUntil(s, visited);

}

void Graph::DFSUntil(int s, bool visited[])
{
	visited[s] = true;
	cout << s << " ";

	list<int>::iterator it;
	for(it = adj[s].begin(); it != adj[s].end(); it++)
	{
		if(!visited[*it])
			DFSUntil(*it, visited);
	}
}

void Graph::BFS(int s)
{
//	bool *visited = new bool[v];
//	deque<int> q;
//	list<int>::iterator it;
//
//	for(int i = 0; i < v; i++)
//	{
//		visited[i] = false;
//	}
//
//	for(int i = 0; i < v; i++)
//	{
//		if(!visited[i])
//		{
//			q.push_front(i);
//			visited[i] = true;
//			//cout << i << " ";
//			while(!q.empty())
//			{
//				int f = q.front();
//				cout << f << " ";
//				q.pop_front();
//				for(it = adj[f].begin(); it != adj[f].end(); it++)
//				{
//					if(!visited[*it])
//					{
//						q.push_front(*it);
//						visited[*it] = true;
//						//cout << " " << *it << " ";
//					}
//
//				}
//			}
//		}
//	}
//
//	delete [] visited;
    bool *visited = new bool[v];
    for(int i = 0; i < v; i++)
        visited[i] = false;

    // Create a queue for BFS
    list<int> queue;

    // Mark the current node as visited and enqueue it
    visited[s] = true;
    queue.push_back(s);

    // 'i' will be used to get all adjacent vertices of a vertex
    list<int>::iterator i;

    while(!queue.empty())
    {
        // Dequeue a vertex from queue and print it
        s = queue.front();
        cout << s << " ";
        queue.pop_front();

        // Get all adjacent vertices of the dequeued vertex s
        // If a adjacent has not been visited, then mark it visited
        // and enqueue it
        for(i = adj[s].begin(); i != adj[s].end(); ++i)
        {
            if(!visited[*i])
            {
                visited[*i] = true;
                queue.push_back(*i);
            }
        }
        //visited[s] = true;
    }
}

bool Graph::search(int start, int end)
{
	bool *visited = new bool[v];

	for(int i = 0; i < v; i++)
	{
		visited[i] = false;
	}

	list<int> q;

	visited[start] = true;
	q.push_back(start);

	list<int>::iterator it;

	while(!q.empty())
	{
		int s = q.front();
		it = q.begin();
		q.pop_front();
		for(int i : adj[s])
		{
			if(!visited[i])
			{
				if(i == end)
					return true;
			    else
			    {
			         visited[i] = true;
			         q.push_back(i);
			    }
			}
		}
		//visited[s] = true;

	}
	return false;
}


