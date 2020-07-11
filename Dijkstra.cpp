#include <fstream>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#define INF 1000000
#define Nmax 10000

using namespace std;

/* Pentru fiecare nod sursa i retin o pereche(distanta,nod),unde distanta este costul minim pentru a ajuge de la i la nod */
vector < pair<int, int> > G[Nmax];

int dist[Nmax];/* vector pentru algoritmul Dijkstra in care retin distantele minime*/

void Dijkstra(int n, int m, int nod_sursa)
{/* Algoritmul determina costul minim de la nodul sursa catre toate celelalte noduri accesibile din graf */

/* Initializez */
	for (int i = 0; i < n; i++)
		dist[i] = INF;

	dist[nod_sursa] = 0;

/* Creez o multime in care retin perechi(distanta,nod), unde distanta este distanta minima de la sursa la nod */
	set < pair<int, int> > h;
	h.insert(make_pair(0, nod_sursa));

	while (!h.empty()) 
	{
		int nod_curent = h.begin()->second;/* nodul */
		int min_curent = h.begin()->first;/* distanta minima */
		h.erase(h.begin());/* elimin din multime */

	/* parcurg muchiile care ies din nodul curent */
		for (vector< pair<int, int> >::iterator it = G[nod_curent].begin(); it != G[nod_curent].end(); ++it) 
		{/* Recalculez distantele nodurilor care comunica cu nodul curent prin PD*/
			int to = it->first;
			int cost = it->second;
			if (dist[to] > dist[nod_curent] + cost) 
			{
				if (dist[to] != INF) 
				{/* a mai fost modificata, elimin din multime */
					h.erase(h.find(make_pair(dist[to], to)));
				}
				dist[to] = dist[nod_curent] + cost;
				h.insert(make_pair(dist[to], to));
			}
		}
	}
}
int main(int argc,char *argv[])
{
	int n, m;/* numar de noduri/muchii */
	int nod_sursa;

	ifstream in(argv[1]);
	in >> n >> m >> nod_sursa;

	/* citesc muchiile */
	int from, to, cost;
	for (int indice_muchie = 0; indice_muchie < m; indice_muchie++)
	{
		in >> from >> to >> cost;
		G[from].push_back(make_pair(to, cost));
	}

	Dijkstra(n, m,nod_sursa);

	ofstream out(argv[2]);
	for (int nod = 0; nod < n; nod++)
		if (dist[nod] == INF)
			out << "INF ";
		else
			out << dist[nod] << " ";

	in.close();
	out.close();
	return 0;
}