#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <algorithm>
#define Nmax 101000
#define Mmax 501000
using namespace std;

vector< pair<int, int> > graf[Nmax];/* pentru fiecare nod retin muchiile sub forma de pereche(dest,index_muchie) */

void citireDate(int& N, int& M,char* fisier_intrare)
{
    ifstream in(fisier_intrare);
    in >> N >> M;

    int sursa, dest;
    for (int i = 0; i < M; i++)
    {
        in >> sursa >> dest;
        
        graf[sursa].push_back({ dest,i });
        graf[dest].push_back({ sursa,i });
    }
    
    in.close();
}

void printCircuit(char* fisier_iesire)
{   
    /* Calea curenta */
    stack<int> curr_path;

    /* circuitul eulerian */
    vector<int> circuit;

    bool vizitat[Mmax] = { 0 };

    curr_path.push(0);
    int curr_v = 0;

    while (!curr_path.empty())/* Exista noduri in calea curenta */
    {   
        curr_v = curr_path.top();
       
    /* Nodul mai contine muchii nefolosite */
        while (1)
        {
            if (graf[curr_v].empty())
                break;

            if (!vizitat[graf[curr_v].back().second])
                break;

            graf[curr_v].pop_back();
        }
        if(graf[curr_v].empty())
        {
        /*
            Daca am folosit toate muchiile varfului
            Retin circuitul
        */
            circuit.push_back(curr_v);
            curr_path.pop();

            continue;
        }

        curr_path.push(graf[curr_v].back().first);
        vizitat[graf[curr_v].back().second] = true;
        graf[curr_v].pop_back();
    }

    ofstream out(fisier_iesire);

/* Afisez ciclul eulerian */
    reverse(circuit.begin(), circuit.end());
    for (int i=0; i<circuit.size()-1; i++)
        out << circuit[i] << " ";

    out.close();
}

int main(int argc, char* argv[])
{
    int N, M; /* noduri/muchii */
    //vector< vector< pair<int,int> > > graf;/* pentru fiecare nod retin muchiile sub forma de pereche(dest,index_muchie) */
    citireDate(N, M, argv[1]);

    printCircuit(argv[2]);

    return 0;
}