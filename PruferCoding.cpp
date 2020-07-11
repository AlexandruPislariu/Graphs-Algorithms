#include <iostream>
#include <fstream>
#define Nmax 100000

using namespace std;

void citire(int& n, int parinti[], char* fisier_intrare)
{
	ifstream in(fisier_intrare);

	in >> n;
	for (int nod = 0; nod < n; nod++)
		in >> parinti[nod];

	in.close();
}

void initializare(int n, int cod_prufer[])
{
	for (int nod = 0; nod < n; nod++)
		cod_prufer[nod] = 0;
}
void Prufer(int n, int parinti[], int cod_prufer[])
{	
	int vizitat[Nmax];
	initializare(n, vizitat);

/* Marchez parintii */
	for (int nod = 0; nod < n; nod++)
		vizitat[parinti[nod]]++;

/* Elimin frunzele pe rand */
	int dim = 0;
	for (int i = 0; i < n - 1; i++)
	{
		int j = 0;
		while (j < n)
		{
			if (vizitat[j] == 0)/* este frunza */
			{
				vizitat[j] = -1;/* elimin */
				cod_prufer[dim++] = parinti[j];/* adaug parintele */
				vizitat[parinti[j]]--;/* elimin un copil al parintelui */

				if (vizitat[parinti[j]] == 0 && parinti[j] < j)
					j = parinti[j] - 1;
			}

			j++;
		}
	}
}

void afisare(int n, int cod_prufer[], char* fisier_iesire)
{
	ofstream out(fisier_iesire);
	out << n - 1 << '\n';
	for (int i = 0; i < n - 1; i++)
		out << cod_prufer[i] << " ";
	out.close();
}

int main(int argc, char** argv)
{
	int n; /* numar de noduri */
	int parinti[Nmax]; /* vector caracteristic, pe casuta i se afla parintele nodului i */
	citire(n, parinti, argv[1]);

	int cod_prufer[Nmax];
	Prufer(n, parinti, cod_prufer);

	afisare(n, cod_prufer, argv[2]);

	return 0;
}