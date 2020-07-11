//Formeaza un arbore de cost minim din N-1 muchii ale grafului
#include <fstream>
#include <algorithm>
#include <vector>
#define Nmax 5000
#define Mmax 50000
using namespace std;
struct muchie
{
    int c1, c2;
    int cost;
};
void citire(int& N, int& M, muchie X[],char* fisier_intrare)
{
    ifstream in(fisier_intrare);
    in >> N >> M;
    //retinem muchiile
    for (int i = 0; i < M; i++)
        in >> X[i].c1 >> X[i].c2 >> X[i].cost;
    in.close();
}
void initializare(int N, int v[])
{
    for (int i = 0; i < N; i++)
        v[i] = i;
}

bool compare(muchie a, muchie b)
{
    if (a.cost >= b.cost)
        return true;

    //if (a.cost == b.cost && (a.c1 > b.c1))
      //  return true;

    //if (a.cost == b.cost && (a.c1 == b.c1) && a.c2 > b.c2)
        //return true;

    return false;
}

void Bubble(int dim, muchie v[])
{
    int nrSortate = 0;
    bool sortat;
    muchie aux;
    do
    {//presupunem ca vectorul este sortat
        sortat = 1;
        for (int i = 0; i < dim - nrSortate - 1; i++)
            if (compare(v[i],v[i+1]))
            {
                sortat = 0;
                aux = v[i];
                v[i] = v[i + 1];
                v[i + 1] = aux;
            }

        //crestem numarul de elemente sortate
        nrSortate++;

    } while (sortat == 0);
}
void Kruskal(int N, muchie X[], int arbori[], int& cost,  vector<muchie>& muchiiArbore)
{
    int muchiiFolosite = 0;
    int muchieActuala = 0;
    int cul1, cul2;//culorile

//cat timp inca nu am format un arbore
    while (muchiiFolosite < N - 1)
    {//verificam sa nu formam un ciclu cu muchiaActuala
        if (arbori[X[muchieActuala].c1] != arbori[X[muchieActuala].c2])
        {//adaugam muchia in arbore
            muchiiArbore.push_back( X[muchieActuala]);
            muchiiFolosite++;

            //adaugam costul
            cost = cost + X[muchieActuala].cost;

            //coloram arborii formati de cele doua noduri cu aceeasi culoare
            //(intersectam cei 2 arbori)
            cul1 = arbori[X[muchieActuala].c1];
            cul2 = arbori[X[muchieActuala].c2];
            for (int i = 0; i < N; i++)
                if (arbori[i] == cul1)arbori[i] = cul2;

        }
        //trecem la urmatoarea muchie
        muchieActuala++;
    }
}

void afisare(int N, vector<muchie>& v, int cost,char* fisier_iesire)
{
    ofstream out(fisier_iesire);
    out << cost << '\n';
    out << N - 1 << '\n';
    for (int i = 0; i < N-1; i++)
            out << v[i].c1 << " " << v[i].c2 << '\n';
    out.close();
}


int main(int argc,char** argv)
{
    int N, M;
    muchie X[Mmax];
    citire(N, M, X,argv[1]);

    //formam "padurea" de arbori
    int arbori[Nmax];
    initializare(N, arbori);

    //sortam muchiile crescator dupa costuri
    Bubble(M, X);

    //determinam arborele de cost minim
    int costArb = 0;
    vector<muchie> muchiiArbore;
    Kruskal(N, X, arbori, costArb, muchiiArbore);

    afisare(N, muchiiArbore, costArb, argv[2]);
    return 0;
}
