#include <iostream>
#include <fstream>
#define Nmax 100000

using namespace std;

void citire(int& n, int cod_prufer[], char* fisier_intrare)
{
	ifstream in(fisier_intrare);

	in >> n;
	for (int nod = 0; nod < n; nod++)
		in >> cod_prufer[nod];

	in.close();
}

int cauta(int n, int vizitat[])
{/* Cel mai mic element care nu apare in cod */
	for (int i = 0; i < n + 1; i++)
		if (!vizitat[i])
			return i;
}

void Prufer_decode(int n, int parinti[], int cod_prufer[])
{	
	int vizitat[Nmax];

	for (int nod = 0; nod < n; nod++)
		vizitat[cod_prufer[nod]] ++;

	parinti[cod_prufer[n - 1]] = -1;
	for (int nod = 0; nod < n; nod++)
	{	
		int y = cauta(n, vizitat);
		parinti[y] = cod_prufer[nod];

		vizitat[cod_prufer[nod]] --;
		vizitat[y] ++;
		
	}
}

void afisare(int n, int parinti[], char* fisier_iesire)
{
	ofstream out(fisier_iesire);
	out << n + 1 << '\n';
	for (int i = 0; i < n + 1; i++)
		out << parinti[i] << " ";
	out.close();
}

int main(int argc, char** argv)
{
	int n; /* numar de noduri */
	int cod_prufer[Nmax]; /* vector caracteristic, pe casuta i se afla parintele nodului i */
	citire(n, cod_prufer, argv[1]);

	int parinti[Nmax+1];
	Prufer_decode(n, parinti, cod_prufer);

	afisare(n, parinti, argv[2]);

	return 0;
}