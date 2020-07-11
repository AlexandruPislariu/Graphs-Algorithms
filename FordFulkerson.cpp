#include <fstream>
#include <iostream>
#include <algorithm>
#include <queue>
#define Nmax 1000
#define INF 2000000
using namespace std;

int graf[Nmax][Nmax];

void citireDate(int& N, int& M, char* fisier_intrare)
{
	ifstream in(fisier_intrare);
	in >> N >> M;

	int sursa, dest, capacitate;
	for (int i = 0; i < M; i++)
	{
		in >> sursa >> dest >> capacitate;
		graf[sursa][dest] = capacitate;
	}

	in.close();
}

bool bfs(int N,int residualGraf[Nmax][Nmax], int s, int t, int parent[])
{
    bool visited[Nmax] = { 0 };

    /*Coada in care retin calea de la s la t*/
    queue <int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;
 

    /* BFS */
    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (int v = 0; v < N; v++)
        {
            if (visited[v] == false && residualGraf[u][v] > 0)
            {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }

    /* Am gasit o cale de la s la t*/
    return (visited[t] == true);
}

int fordFulkerson(int N, int M, int s, int t)
{
    int u, v;
   /* 
    Creez un graf residual
    Retin capacitatile residuale ale arcelor
   */
    
    /* Folosesc acelasi graf ca si graf rezidual */
    int parent[Nmax];  /* Retin caile de la parcurgerea BFS in acest vector */

    int max_flow = 0;

    /* Exita o parcurgere de la s catre t */
    while (bfs(N,graf, s, t, parent))
    {   
        /* Flow-ul maxim al parcurgerii gasite */
        int path_flow = INF;
        for (v = t; v != s; v = parent[v])
        {
            u = parent[v];
            path_flow = min(path_flow, graf[u][v]);
        }

        /* Actualizez matricea de capacitati residuale */
        for (v = t; v != s; v = parent[v])
        {
            u = parent[v];
            graf[u][v] -= path_flow;
            graf[v][u] += path_flow;
        }

        max_flow += path_flow;
    }

    return max_flow;
}
int main(int argc, char* argv[])
{
	int N, M;/* numarul de varfuri/arce */
	citireDate(N, M, argv[1]);

    ofstream out(argv[2]);
	out<<fordFulkerson(N, M, 0, N - 1);
    out.close();

    return 0;
}