#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <math.h>
#include <stack>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <list>
#include <vector>
#include <algorithm>
#include <limits.h>
#include <queue>

using namespace std;


int M,N;
bool bipartite_matching(bool **bipartite_graph,int u, bool *seen,int *matchR)
{
	for (int v = 0; v < N; ++v)
	{
		if(bipartite_graph[u][v] && !seen[v])
		{
			seen[v]=true;
			if (matchR[v] < 0 || bipartite_matching(bipartite_graph, matchR[v], seen, matchR))
            {
                matchR[v] = u;
                cout<<u<<" "<<v+M<<endl;
                return true;
            }
		}
	}
	return false;
}

void maximum_Bipartite_matching(bool **bipartite_graph)
{
	int matchR[N];
	memset(matchR, -1, sizeof(matchR));
    int result = 0;
    for (int u = 0; u < M; u++)
    {
        bool seen[N];
        memset(seen, 0, sizeof(seen));
        if (bipartite_matching(bipartite_graph, u, seen, matchR))
        {
            result++;
        }
    }   
}

typedef struct temp
{
	int u,v;
} Edge;
bool Compare(const Edge &a, const Edge &b)
{
	if(a.u == b.u) return a.v < b.v;
	return a.u < b.u;
}


class Graph
{
	public:
	int vertices,edges;    
	int source,sink;
	int K;
	list<int> *adjacency_list;   
	int ** weight;
	int ** residual_graph;
	Graph(int vertices)
	{
		this->vertices = vertices;
		this->source = 0;
		this->sink = vertices-1;
		adjacency_list = new list<int>[vertices];
		weight = (int**)calloc(vertices,sizeof(int*));
		for(int i=0;i<vertices;i++) weight[i] = (int*)calloc(vertices,sizeof(int));
		residual_graph = (int**)calloc(vertices,sizeof(int*));
		for(int i=0;i<vertices;i++) residual_graph[i] = (int*)calloc(vertices,sizeof(int));
		edges=0;
		readGraph();
	}
	

	void add_an_edge(int u, int v)
	{
		adjacency_list[u].push_back(v); 
		weight[u][v] = 1;
		edges++;
	}

	void readGraph(){
		int u,v;
		scanf("%d",&K);
		while(scanf("%d %d",&u, &v) != EOF) add_an_edge(u+1,v+1);
		for(u=0;u<K;u++) add_an_edge(source,u+1);
		for(u=K;u<vertices-1;u++) add_an_edge(u+1,sink);
	}
	int breadth_first_search(int *parent){
		int s = source;
		// int sink;
		// if(source==sink)
		// {
		// 	printf("%d\n%d\n",source,sink );
		// 	exit(0);
		// }
		// pathexists=false;
		// int parent[number_of_vertices];
		//printed the source
		bool visited[vertices];
		memset(visited, 0, sizeof(visited));
		for(int i = 0; i < vertices; i++){
			visited[i] = false;
			parent[i] = -1;
		}
		list<int> queue;
		visited[s] = true;
		queue.push_back(s);
		parent[vertices] = -1;
		list<int>::iterator i;
		while(!queue.empty())
		{
			s = queue.front();
			// if(sink == s)
			// 	{
			// 		pathexists=true;
			// 		// print(sink,parent);
			// 		// break;
			// 	}
			queue.pop_front();
			for(i = adjacency_list[s].begin(); i != adjacency_list[s].end(); ++i){
				if(!visited[*i] && residual_graph[s][*i]){
					visited[*i] = true;
					queue.push_back(*i);
					parent[*i] = s;
				}
			}
		}
		// if(!pathexists)
		// {
		// 	printf("-1\n");
		// }

		if(!visited[sink]) return 0;
		return 1;
	}

	void fordFulkerson(){
		int u, v;
		int t=sink,s=source;
		Edge result[edges];
		int count=0;
		// int residual_graph[V][V];
		// int **residual_graph=new int*[V];
		// // int** ary = new int*[sizeX];
		// for(int i = 0; i < V; ++i)
		//     residual_graph[i] = new int[V];
		

		for (u = 0; u < vertices; u++){
			for (v = 0; v < vertices; v++){
				residual_graph[u][v] = weight[u][v];
			}
		}
		int parent[vertices];
		int max_flow = 0;
		while (breadth_first_search(parent)){
			int path_flow = INT_MAX;
			for (v=t; v!=s; v=parent[v]){
				u = parent[v];
				path_flow = min(path_flow, residual_graph[u][v]);
			}
			for (v=t; v != s; v=parent[v]){
				u = parent[v];
				residual_graph[u][v] -= path_flow;
				residual_graph[v][u] += path_flow;
				if(u>0 && u<=K && v>K && v<vertices) adjacency_list[v].push_back(u); 
			}
			max_flow += path_flow;
		}
		for (u = 0; u < vertices; u++){
			for (v = 0; v < vertices; v++){
				if(residual_graph[u][v] < weight[u][v]){
					result[count].u = u;
					result[count].v = v;
					count++;
				}
			}
		}
		// cout<<max_flow;
		// while(1);
		sort(result,result+count,Compare);
		for(int j=0;j<count;j++){
			if(result[j].u == source) continue;
			if(result[j].v == sink) break;
			printf("%d %d\n",result[j].u-1,result[j].v-1);
		}
		// return max_flow;
		// cout<<max_flow<<endl;
		// for (int i = 0; i < count; ++i)
		// {
		// 	/* code */
		// }
	}
};

int main()
{
	// 	int V;
	// 	cin>>V>>M;
	// 	N=V-M;
	// 	// cout<<"M=="<<M<<",N=="<<N<<endl;
	// 	bool **bipartite_graph=new bool*[M];
	// 	for(int i = 0; i < V; ++i)
	// 	    bipartite_graph[i] = new bool[N];
	// 	int x,y;
	// 	for (int i = 0; i < M; ++i)
	// 	{
	// 		for (int j = 0; j < N; ++j)
	// 			{
	// 				bipartite_graph[i][j]=0;
	// 			}	
	// 	}
	// 	while(scanf("%d%d",&x,&y)!=EOF)
	// 	{
	// 		bipartite_graph[x][y-M]=1;
	// 	}
	// 	for (int i = 0; i < M; ++i)
	// 	{
	// 		// cout<<endl;
	// 		for (int j = 0; j < N; ++j)
	// 			{
	// 				cout<<bipartite_graph[i][j]<<" ";
	// 			}	
	// 		cout<<endl<<endl;
	// 	}
	// 	maximum_Bipartite_matching(bipartite_graph);
	int vertices;
	scanf("%d",&vertices);
	Graph g(vertices+2);
	g.fordFulkerson();
	return 0;
}
