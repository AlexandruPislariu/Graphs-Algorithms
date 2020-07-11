#include <iostream>
#include <fstream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <queue>
#define Nmax 10000

using namespace std;

struct Node
{/* Definesc un nod al arborelui de codare Huffman*/
	char data;
	int freq;
	Node* left;
	Node* right;
};

struct Comparator
{/* Comparator pentru coada de prioritati */
	bool operator()(Node* a, Node* b) const
	{
		if (a->freq > b->freq)
			return true;

		if (a->freq == b->freq && a->data > b->data)
			return true;

		return false;
	}
};

struct HuffmanTree
{/* Definesc un arbore de codare Huffman*/
	unsigned size;
	priority_queue<Node* , vector<Node*>, Comparator> arr;/* Nodurile arborelui(prioritatea este frecventa) */
};

Node* newNode(char ch, int fr)
{
	Node* tmp = (Node*)malloc(sizeof(Node));
	tmp->data = ch;
	tmp->freq = fr;
	tmp->left = NULL;
	tmp->right = NULL;

	return tmp;
}

void insertMinHeap(HuffmanTree& minHeap,Node* minHeapNode)
{/* Functia insereaza nodul pe pozitia potrivita in coada de prioritati */

	minHeap.arr.push(minHeapNode);
	minHeap.size++;
}

void citire(char line[], char* fisier_intrare)
{
	ifstream in(fisier_intrare);
	in.getline(line, Nmax, '\n');
	in.close();
}

void prelucrare(int &size,char caractere[],int frecvente[],char line[])
{/* Retin caracterele si frecventele de aparitie*/
	for (int i = 0; i < strlen(line); i++)
	{
		frecvente[line[i] - '\0']++;
		if(frecvente[line[i] - '\0']==1)
			caractere[size++] = line[i];
	}

}

HuffmanTree createAndBuildMinHeap(char caractere[], int frecvente[], int size)
{/* Construiesc un arbore Huffman*/
	HuffmanTree minHeap;
	minHeap.size = 0;

	for (int nod = 0; nod < size; nod++)
		minHeap.arr.push(newNode(caractere[nod], frecvente[caractere[nod]-'\0']));

	minHeap.size = size;

	return minHeap;
}

Node* extractMin(HuffmanTree& minHeap)
{/* Extrag nodul minim din coada de prioritati*/
	Node* tmp = minHeap.arr.top();
	minHeap.size--;
	minHeap.arr.pop();

	return tmp;

}
Node* buildHuffman(char caractere[], int frecvente[], int size)
{/*
	Construiesc un arbore de codare Huffman
	Functia returneaza radacina arborelui
*/
	Node* left, * right, * top;

/* Construiesc o coada de prioritati(prioritatea fiind frecventa nodului) */
	HuffmanTree minHeap = createAndBuildMinHeap(caractere, frecvente, size);

/* Parcurg coada */
	while (minHeap.size!=1)/* Exista noduri */
	{/* Extrag 2 noduri minime */
		left = extractMin(minHeap);
		right = extractMin(minHeap);

	/* Creez un nou nod a carui frecventa este suma frecventelor nodurilor extrase */
		top = newNode('\0', left->freq + right->freq);
		top->left = left;
		top->right = right;

		insertMinHeap(minHeap, top);
	}

	return extractMin(minHeap); /* nodul ramas este radacina */
}

void printCodes(Node* radacina, char codare[], int top,char ch,ostream& out)
{/* Functia construieste un cod binar pentru fiecare nod frunza(caracter) */

	if (radacina->left!=NULL)
	{/* 0 pentru deplasare stanga */
		codare[top] = '0';
		printCodes(radacina->left, codare, top+1,ch,out);
	}

	if (radacina->right!=NULL)
	{/* 1 pentru deplasare la dreapta */
		codare[top] = '1';
		printCodes(radacina->right, codare, top+1,ch,out);
	}

	if (radacina->data == ch)
	{
		codare[top] = NULL;
		out << codare;
		return;
	}
}

void HuffmanCode(char line[],char caractere[], int frecvente[], int size,char * fisier_iesire)
{	
	ofstream out(fisier_iesire);
	out << size << '\n';
	for (int i = 0; i < 128; i++)
		if (frecvente['\0' + i] != 0)
			out << char('\0' + i) << " " << frecvente['\0' + i] << '\n';

	Node* radacina = buildHuffman(caractere, frecvente, size);

/* Afisez Codurile */
	char codare[Nmax];
	int top=0;
	for (int i = 0; i < strlen(line); i++)
		printCodes(radacina, codare, 0, line[i], out);

}
int main(int argc, char** argv)
{
	char line[Nmax];
	citire(line, argv[1]);

	int size = 0;
	char caractere[128];
	int frecvente[128] = { 0 };
	prelucrare(size,caractere,frecvente,line);

	HuffmanCode(line,caractere, frecvente, size, argv[2]);

	return 0;
}
