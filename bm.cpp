#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <list>
#include <list>
#include <limits.h>
#include <algorithm>
#include <queue>
#include <string.h>

using namespace std;

// int M,N;
// bool bipartite_matching(bool **bipartite_graph,int u, bool *seen,int *matchR)
// {
// 	for (int v = 0; v < N; ++v)
// 	{
// 		if(bipartite_graph[u][v] && !seen[v])
// 		{
// 			seen[v]=true;
// 			if (matchR[v] < 0 || bipartite_matching(bipartite_graph, matchR[v], seen, matchR))
//             {
//                 matchR[v] = u;
//                 cout<<u<<" "<<v+M<<endl;
//                 return true;
//             }
// 		}
// 	}
// 	return false;
// }

// void maximum_Bipartite_matching(bool **bipartite_graph)
// {
// 	int matchR[N];
// 	memset(matchR, -1, sizeof(matchR));
//     int result = 0;
//     for (int u = 0; u < M; u++)
//     {
//         bool seen[N];
//         memset(seen, 0, sizeof(seen));
//         if (bipartite_matching(bipartite_graph, u, seen, matchR))
//         {
//             result++;
//         }
//     }   
// }
// void try1()
// {
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
// }
typedef struct temp
{
	int u,v;
}Edge;
bool Compare(const Edge &a, const Edge &b){
	if(a.u == b.u) return a.v < b.v;
	return a.u < b.u;
}


class Graph
{
	int V,E;
	int S,T;
	int K;
	std::list<int> *adj;
	int **weight;
	int **residual_graph;
	public:
	Graph(int V)
	{
		this->V=V;
		this->S=0;
		this->T = V-1;
		adj = new std::list<int>[V];
		weight = (int **)calloc(V,sizeof(int *));
		for (int i = 0; i < V; ++i)
		{
			weight[i]=(int*)calloc(V,sizeof(int));
		}
		residual_graph = (int ** ) calloc(V,sizeof(int *));
		for (int i = 0; i < V; ++i)
		{
			residual_graph[i]=(int *)calloc(V,sizeof(int));
		}
		E=0;
		input_graph_edges();
	}	
	void add_an_edge(int u,int v)
	{
		adj[u].push_back(v);
		weight[u][v]=1;
		E++;
	}
	void input_graph_edges()
	{
		int u,v;
		scanf("%d",&K);
		while(scanf("%d%d",&u,&v) !=EOF)
		{
			add_an_edge(u+1,v+1);
		}
		for (int i = 0; i < K; ++i)
		{
			add_an_edge(S,u+1);
		}
		for (int i = K; i < V-1; ++i)
		{
			add_an_edge(u+1,T);
		}
	}
	void print(int i,int * parent)
	{
		if(i==S)
		{
			printf("%d\n",S );
			return;
		}
		print(parent[i],parent);
		printf("%d\n",i );
	}
	int breadth_first_search(int *parent)
		{
			// int source=S;
			int s=S;
			bool *visited = new bool[V];
			memset(visited, 0, sizeof(visited));
			// int sink;
			// if(source==sink)
			// {
			// 	printf("%d\n%d\n",source,sink );
			// 	exit(0);
			// }
			// pathexists=false;
			// int parent[number_of_vertices];
			//printed the source
			
			for (int i = 0; i < V; ++i)
			{
				visited[i]= false;
				parent[i]=-1;
			}
			list<int> queue;
			visited[s]= true;
			list<int>::iterator i;
			queue.push_back(s);
			parent[V] = -1;
			while((!queue.empty()))
			{
				s=queue.front();
				// if(sink == s)
				// 	{
				// 		pathexists=true;
				// 		// print(sink,parent);
				// 		// break;
				// 	}
				queue.pop_front();
				for (i = adj[s].begin(); i != adj[s].end(); ++i)
				{
					if((visited[*i]==false) && residual_graph[s][*i])
					{
						visited[*i] = true;
						queue.push_back(*i);
						parent[*i]=s;
					}
				}
			}
			// if(!pathexists)
			// {
			// 	printf("-1\n");
			// }
			if(!visited[T]) return 0;
			return 1; 
		}
		void ford_fulkerson()
		{
			int u,v;
			int t=T,s=S;
			int count=0;
			Edge result[E];
			// int residual_graph[V][V];
			// int **residual_graph=new int*[V];
			// // int** ary = new int*[sizeX];
			// for(int i = 0; i < V; ++i)
			//     residual_graph[i] = new int[V];
			for (int i = 0; i < V; ++i)
			{
				for (int j = 0; j < V; ++j)
				{
					residual_graph[i][j]=weight[i][j];
				}
			}
			int parent[V];
			int max_flow=0;
			while(breadth_first_search(parent))
			{
				// cout<<"h1 2 \n";
				int path_flow = INT_MAX;
				for ( v = t; v!=s; v=parent[v])
				{
					u=parent[v];
					path_flow = min(path_flow,residual_graph[u][v]);
				}
				for ( v = t; v!=S; v=parent[v])
				{
					u=parent[v];
					residual_graph[u][v] -= path_flow;
					residual_graph[v][u] += path_flow;
					if(u>0 && u<=K && v>K && v<V) adj[v].push_back(u);
				}
				max_flow +=path_flow;
			}
			for (u = 0; u < V; u++){
				for (v = 0; v < V; v++){
					if(residual_graph[u][v] < weight[u][v]){
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
				// cout<<"coming here\n";
				if(result[j].u == S) continue;
				if(result[j].v == T) break;
				
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
	int V;
	cin>>V;
	Graph g(V+2);
	g.ford_fulkerson();
    return 0;
}
