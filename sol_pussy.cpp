#include <iostream>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <algorithm>

using namespace std;

typedef struct M{
	int u,v;
} Edge;

bool Compare(const Edge &a, const Edge &b){
	if(a.u == b.u) return a.v < b.v;
	return a.u < b.u;
}

class Graph
{
	int V,E;    // No. of vertices
	int S,T;
	int K;
	list<int> *adj;    // Pointer to an array containing adjacency lists
	int ** weight;
	int ** rGraph;

	public:
	Graph(int V){
		this->V = V;
		this->S = 0;
		this->T = V-1;
		adj = new list<int>[V];
		weight = (int**)calloc(V,sizeof(int*));
		int i;
		for(i=0;i<V;i++) weight[i] = (int*)calloc(V,sizeof(int));

		rGraph = (int**)calloc(V,sizeof(int*));
		for(i=0;i<V;i++) rGraph[i] = (int*)calloc(V,sizeof(int));

		E=0;

		readGraph();
	}

	void addEdge(int u, int v){
		adj[u].push_back(v); // Add v to u’s list.
		//adj[v].push_back(u); // Add v to u’s list.
		weight[u][v] = 1;
		E++;
	}

	void readGraph(){
		int u,v;
		scanf("%d",&K);
		while(scanf("%d %d",&u, &v) != EOF) addEdge(u+1,v+1);
		for(u=0;u<K;u++) addEdge(S,u+1);
		for(u=K;u<V-1;u++) addEdge(u+1,T);
	}

	void print(int i, int * parent){
		if(i==S){
			printf("%d\n",S);
			return;
		}
		print(parent[i],parent);
		printf("%d\n",i);
	}

	int BFS(int *parent){
		int s = S;

		// Mark all the vertices as not visited
		bool visited[V];
		memset(visited, 0, sizeof(visited));
		//int delta[V];
		//int parent[V];
		//shortestPath result[V-1];
		//int count=0;

		for(int i = 0; i < V; i++){
			visited[i] = false;
			//delta[i] = INT_MAX;
			parent[i] = -1;
		}

		// Create a queue for BFS
		list<int> queue;


		// Mark the current node as visited and enqueue it
		visited[s] = true;
		queue.push_back(s);
		//delta[s] = 0;
		parent[V] = -1;

		// 'i' will be used to get all adjacent vertices of a vertex
		list<int>::iterator i;

		while(!queue.empty())
		{
			// Dequeue a vertex from queue and print it
			s = queue.front();
			//cout << s << " " << delta[s] << '\n';

			//result[count].first = s;
			//result[count].second = delta[s];
			//count++;

			queue.pop_front();

			/*for (int v=0; v<V; v++){
				if (visited[v]==false && rGraph[s][v] > 0){
					queue.push_back(v);
					parent[v] = s;
					visited[v] = true;
				}
			}*/

			for(i = adj[s].begin(); i != adj[s].end(); ++i){
				if(!visited[*i] && rGraph[s][*i]){
					visited[*i] = true;
					queue.push_back(*i);
					parent[*i] = s;
					//delta[*i] = delta[s] + weight[s][*i];
				}
			}
		}

		if(!visited[T]) return 0;
		return 1;
		//sort(result,result+count,Compare);
		//for(int j=0;j<count;j++) printf("%d %d\n",result[j].first,result[j].second);
	}

	void fordFulkerson(){
		int u, v;
		int t=T,s=S;
		Edge result[E];
		int count=0;

		for (u = 0; u < V; u++){
			for (v = 0; v < V; v++){
				rGraph[u][v] = weight[u][v];
			}
		}

		int parent[V];

		int max_flow = 0;

		while (BFS(parent)){
			//putchar('1');
			int path_flow = INT_MAX;

			for (v=t; v!=s; v=parent[v]){
				u = parent[v];
				path_flow = min(path_flow, rGraph[u][v]);
			}

			for (v=t; v != s; v=parent[v]){
				u = parent[v];
				rGraph[u][v] -= path_flow;
				rGraph[v][u] += path_flow;
				if(u>0 && u<=K && v>K && v<V) adj[v].push_back(u); // Add v to u’s list.
			}

			max_flow += path_flow;
		}

		for (u = 0; u < V; u++){
			for (v = 0; v < V; v++){
				if(rGraph[u][v] < weight[u][v]){
					result[count].u = u;
					result[count].v = v;
					//result[count].w = weight[u][v]-rGraph[u][v];
					count++;
				}
			}
		}

		//printf("%d\n",max_flow);
		sort(result,result+count,Compare);
		for(int j=0;j<count;j++){
			if(result[j].u == S) continue;
			if(result[j].v == T) break;
			printf("%d %d\n",result[j].u-1,result[j].v-1);
		}
	}
};

int main()
{
	int V;
	scanf("%d",&V);
	//scanf("%d %d",&S,&T);
	Graph g(V+2);

	g.fordFulkerson();

	return 0;
}
