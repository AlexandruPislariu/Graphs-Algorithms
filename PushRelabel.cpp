#include <fstream>
#include <iostream>
#include <algorithm>
#include <queue>
#include <vector>
#define Nmax 1000
#define INF 2000000
using namespace std;

int capacity[Nmax][Nmax];

void citireDate(int& N, int& M, char* fisier_intrare)
{
    ifstream in(fisier_intrare);
    in >> N >> M;

    int sursa, dest, capacitate;
    for (int i = 0; i < M; i++)
    {
        in >> sursa >> dest >> capacitate;
        capacity[sursa][dest] = capacitate;
    }

    in.close();
}

const int inf = 1000000000;
vector< vector<int> > flow;/* flow-ul pentru fiecare arc*/

vector<int> height, excess, seen;/* inaltimea, excesul fiecarui varf */
queue<int> excess_vertices;/* coada in care retin varfurile care au exces */

void push(int u, int v)
{   /* 
        Functia actualizeaza flow-ul arcului (u,v) si (v,u)
        Verifica daca este exces in nodul v
    */
    int d = min(excess[u], capacity[u][v] - flow[u][v]);

    flow[u][v] += d;
    flow[v][u] -= d;

    excess[u] -= d;
    excess[v] += d;

    if (d && excess[v] == d)
        excess_vertices.push(v);
}

void relabel(int N, int u)
{/* 
    Se actualizeaza inaltimea varfului v
    Inaltimea minima a varfurilor existente + 1
*/
    int d = inf;
    for (int i = 0; i < N; i++) 
    {
        if (capacity[u][i] - flow[u][i] > 0)
            d = min(d, height[i]);
    }

    if (d < inf)
        height[u] = d + 1;
}

void discharge(int N,int u)
{
    while (excess[u] > 0) /*Varful se afla in exces */
    {
        if (seen[u] < N) 
        {/* Se cauta primul varf v in care se elimina excesul lui u */
            int v = seen[u];
            if (capacity[u][v] - flow[u][v] > 0 && height[u] > height[v])
                push(u, v);
            else
                seen[u]++;
        }
        else 
        {/* 
            Nu s-a gasit un asemenea varf, se creste inaltimea lui u
            Noua inaltime este (inaltimea minima a varfurilor existente + 1)
        */
            relabel(N,u);
            seen[u] = 0;
        }
    }
}

int pushRelabel(int N, int M, int s, int t)
{
    height.assign(N, 0);/* Inaltimile varfurilor */
    height[s] = N;/* Sursa are inaltimea maxima, de la ea incepe curgerea */

    flow.assign(N, vector<int>(N, 0));/* Iitializez flow-urile arcelor cu 0*/

    excess.assign(N, 0);/* Initializez excesul fiecarui varf */
    excess[s] = inf;

/* 
    Calculez flow-ul fiecarui arc al sursei s 
    Preiau varfurile care se afla in exces
*/
    for (int i = 1; i < N; i++)
        push(s, i);

/* Vector caracteristic(seen[i] = primul nod j a.i h[j]<h[i]) */
    seen.assign(N, 0);

    while (!excess_vertices.empty())/* exista varfuri in exces */
    {
        int u = excess_vertices.front();
        excess_vertices.pop();

        if (u != s && u != t)
            discharge(N,u);
    }

    int max_flow = 0;
    for (int i = 0; i < N; i++)
        max_flow += flow[s][i];

    return max_flow;
}

int main(int argc, char* argv[])
{
    int N, M;/* numarul de varfuri/arce */
    citireDate(N, M, argv[1]);

    ofstream out(argv[2]);
    out << pushRelabel(N, M, 0, N - 1);
    out.close();

    return 0;
}