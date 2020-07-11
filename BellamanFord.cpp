#include <fstream>
#include <iostream>
#define INF 1000000
#define Nmax 1001

using namespace std;

int matrice_distante[Nmax][Nmax];/* matricea initiala de distante a grafului */
int graf_modificat[Nmax][Nmax];/* matricea de distante dupa repoderarea pozitiva*/

void initializare(int n,int distante[])
{/* Functia initializeaza vectorul de distante minime pentru algoritm */
	for (int nod = 0; nod < n; nod++)
		distante[nod] = INF;
}

void Dijkstra(int n, int src,int distante[])
{	
	bool vizitat[Nmax];/* vector caracteristic, vizitat[i] = true daca nodul i se afla in drumul de cost minim*/
	initializare(n, distante);

	for (int i = 0; i < n; i++)
		vizitat[i] = false;

	distante[src] = 0;/* nodul sursa */

	int min_curent, nod_curent;/* distanta minima, nodul curent care contribuie la drumul de cost minim */

	for(int iter=0; iter<n-1; iter++ )
	{/* Caut printre toate nodurile nevizitate, nodul cel mai apropiat de sursa care nu se afla in drumul de cost minim*/
		min_curent = INF;
		for(int nod=0; nod<n; nod++)
			if (!vizitat[nod] && distante[nod] < min_curent)
			{
				min_curent = distante[nod];
				nod_curent = nod;
			}

	/* Marchez ca am gasit */
		vizitat[nod_curent] = true;

		if (min_curent != INF)
		{/* Se poate ajune la acest nod */

		/* Recalculez distantele folosind PD */
			for (int nod = 0; nod < n; nod++)
				if ((distante[nod] > distante[nod_curent] + graf_modificat[nod_curent][nod]) && (matrice_distante[nod_curent][nod] != INF))
					distante[nod] = distante[nod_curent] + graf_modificat[nod_curent][nod];
		}		
	}
}

typedef struct
{
	int c1, c2;
	int cost;
}arc;

int Bellaman_Ford(int n, int m, arc x[],int distante[])
{
	initializare(n, distante);

/* Setez nodul sursa n */
	distante[n] = 0;
/* Adaug muchii de cost 0 de la n la toate celelelate noduri */
	for (int nod = 0; nod < n; nod++)
	{
		x[m + nod].c1 = n;
		x[m + nod].c2 = nod;
		x[m + nod].cost = 0;
	}
	
	for (int nod = 0; nod < n; nod++)
		for (int arc = 0; arc < m + n; arc++)
			if (distante[x[arc].c1] != INF and (distante[x[arc].c1] + x[arc].cost < distante[x[arc].c2]))
				distante[x[arc].c2] = distante[x[arc].c1] + x[arc].cost;

	for (int arc = 0; arc < m + n; arc++)
		if (distante[x[arc].c1] != INF and distante[x[arc].c1] + x[arc].cost < distante[x[arc].c2])
			return -1;

	return 1;
}

void Johnson(int n, int m, char* fisier_iesire)
{/* Creez un vector de arce pe care aplic Bellaman Ford */
	arc x[11000];
	int indice_muchie = 0;
/* adaug muchiile existente */
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			if (matrice_distante[i][j] != INF)
			{
				x[indice_muchie].c1 = i;
				x[indice_muchie].c2 = j;
				x[indice_muchie].cost = matrice_distante[i][j];
				indice_muchie++;
			}

/* Vector folosit pentru a calcula distantele minime pentru BellamanFord*/
	int distante[Nmax];

/* Vector folosit pentru a calcula distantele minime pentru Dijkstra*/
	int distanteDijkstra[Nmax];

	ofstream out(fisier_iesire);
	if (Bellaman_Ford(n, m, x, distante) == -1)/* contine cicluri negative */
		out << -1;
	else
	{/* Reponderarea pozitiva a matricei de costuri */
		for (int i = 0; i < n; i++)
			for (int j = 0; j < n; j++)
				if (matrice_distante[i][j] != INF)/* exista muchia */
				{/* graf_modificat[i][j]>=0 pt orice i,j */
					graf_modificat[i][j] = matrice_distante[i][j] + distante[i] - distante[j];
					out << i << " " << j << " " << graf_modificat[i][j] << '\n';
				}

	/* Rulez Dijkstra pentru fiecare nod */
		for (int i = 0; i < n; i++)
		{
			Dijkstra(n, i, distanteDijkstra);
		/* Afisez distantele minime de la fiecare nod sursa la toate celelalte*/
			for (int nod = 0; nod < n; nod++)
			{	
				if (nod == i)
					out << "0 ";
				if (distanteDijkstra[nod] == INF)
					out << "INF ";
				if (distanteDijkstra[nod] != INF and nod != i)
				{/* Distanta minima din graful initial*/
					distanteDijkstra[nod] = distanteDijkstra[nod] - distante[i] + distante[nod];
					out << distanteDijkstra[nod] << " ";
				}
			}
			out << '\n';
		}
	}
	out.close();
}

int main(int argc, char* argv[])
{
	int n, m;/* numar de noduri/muchii */
	ifstream in(argv[1]);
	in >> n >> m;

/* Initializez matricea */
	for (int i = 0; i < n; i++)
		for (int j = 0; j < n; j++)
			matrice_distante[i][j] = INF;

/* citesc matricea de distante */
	for (int arc = 0; arc < m; arc++)
	{
		int c1, c2, cost;
		in >> c1 >> c2 >> cost;
		matrice_distante[c1][c2] = cost;
	}

	Johnson(n, m,argv[2]);

	in.close();

	return 0;
}