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
	priority_queue<Node*, vector<Node*>, Comparator> arr;/* Nodurile arborelui(prioritatea este frecventa) */
};

Node* newNode(char ch, int fr)
{
	Node* tmp = new Node;
	tmp->data = ch;
	tmp->freq = fr;
	tmp->left = NULL;
	tmp->right = NULL;

	return tmp;
}

void insertMinHeap(HuffmanTree& minHeap, Node* minHeapNode)
{/* Functia insereaza nodul pe pozitia potrivita in coada de prioritati */

	minHeap.arr.push(minHeapNode);
	minHeap.size++;
}

void citire(int& size,char caractere[],int frecvente[],char cod[], char* fisier_intrare)
{
	ifstream in(fisier_intrare);

	in >> size;
	in.get();

	int fr;
	char continut[Nmax];
	for (int i = 0; i < size; i++)
	{
		in.getline(continut, Nmax, '\n');
		
		caractere[i] = ' ';
		int j = 0;
		while (j < strlen(continut) && continut[j] != ' ')
		{
			caractere[i] = continut[j];
			j++;
		}
		j++;

		if (caractere[i] == ' ')
			j++;

		fr = 0;
		while (j < strlen(continut))
		{
			fr = fr * 10 + continut[j] - '0';
			j++;
		}

		frecvente[caractere[i] - '\0'] = fr;

	}

	//in.get();
	in.getline(cod, Nmax, '\n');

	in.close();
}

HuffmanTree createAndBuildMinHeap(char caractere[], int frecvente[], int size)
{/* Construiesc un arbore Huffman*/
	HuffmanTree minHeap;
	minHeap.size = 0;

	for (int nod = 0; nod < size; nod++)
		minHeap.arr.push(newNode(caractere[nod], frecvente[caractere[nod] - '\0']));

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
	while (minHeap.size != 1)/* Exista noduri */
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
void searchCodes(Node* radacina, char codare[], int top, int dimensiune,char* fisier_iesire)
{/* Decodez Huffman */

	ofstream out(fisier_iesire);

	top = 0;
	Node* iterator = radacina;

	while (top!=dimensiune)
	{	
		if (iterator->data != '\0')
		{
			out << iterator->data;
			iterator = radacina;
		}

		if (codare[top] == '0' && iterator->left != NULL)
			iterator = iterator->left;

		if (codare[top] == '1' && iterator->right != NULL)
			iterator = iterator->right;

		top++;
	}

	out << iterator->data;

	out.close();
}

void HuffmanCode(char cod[],char caractere[], int frecvente[], int size,char* fisier_iesire)
{
	Node* radacina = buildHuffman(caractere, frecvente, size);
	int top = 0;
	int dimensiune = strlen(cod);
	searchCodes(radacina, cod,0,dimensiune,fisier_iesire);

}
int main(int argc, char** argv)
{	
	int size;
	char caractere[128];
	int frecvente[128] = { 0 };
	char codare[Nmax];
	citire(size,caractere,frecvente,codare, argv[1]);
	
	HuffmanCode(codare,caractere, frecvente, size,argv[2]);

	return 0;
}