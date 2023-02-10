#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;


struct Edge
{
	char row;
	char col;
};

struct Data
{
	int* adjMatrix;
	char* labels;
	Edge* edges;
	int* edgeWeights;
	int size;
};

void fillData(Data*&);
void kruskal(Data*&);
void displayMatrix(Data*);
void displayEdges(Data*);

int main()
{
	Data* ptr = new Data;

	fillData(ptr);
	displayMatrix(ptr);
	kruskal(ptr);
	displayEdges(ptr);


	delete[] ptr->adjMatrix;
	delete[] ptr->labels;
	delete[] ptr->edges;
	delete[] ptr->edgeWeights;
	delete ptr;

	return 0;
}


void fillData(Data*& ptr)
{
	string filename;

	cout << endl << "Filename: ";
	cin >> filename;

	ifstream inFile(filename);

	if(!inFile)
	{
		cout << endl << "Error: file can't be found" << endl;
		return;
	}

	char tempInput;
	int numLabels = 0;

	// prime read
	inFile >> tempInput;

	// get number of labels
	while(isalpha(tempInput))
	{
		numLabels++;
		inFile >> tempInput;
	}

	// start getting data from file

	inFile.seekg (0, ios::beg);

	ptr->adjMatrix = new int[numLabels * numLabels];
	ptr->labels = new char[numLabels];
	ptr->edges = new Edge[numLabels];
	ptr->edgeWeights = new int[numLabels];
	ptr->size = numLabels;

	// get labels from file
	for(int n = 0; n < numLabels; n++)
		inFile >> ptr->labels[n];

	for(int i = 0; i < numLabels; i++)
	{
		for(int j = 0; j < numLabels; j++)
		{
			// set to 0
			if(i == j)
				ptr->adjMatrix[i * numLabels + j] = 0;

			// upper right
			else if(j > i)
				inFile >> ptr->adjMatrix[i * numLabels + j];

			// copying upper right to lower left
			else
				ptr->adjMatrix[i * numLabels + j] = ptr->adjMatrix[j * numLabels + i];
		}
	}

	inFile.close();
}


void kruskal(Data*& ptr)
{
	string vertices = "";
	int numVertices = 0;
	int numEdges = 0;

	int row;
	int col;

	// large value
	int lowestWeight = 1000000;

	int n = 0;

	while(n < ptr->size - 1)
	{
		lowestWeight = 1000000;

		// traversing adjacency matrix array
		for(int i = 0; i < ptr->size; i++)
		{
			for(int j = 0; j < ptr->size; j++)
			{
				// condition for lowest weight
				if(j > i && ptr->adjMatrix[i * ptr->size + j] < lowestWeight && ptr->adjMatrix[i * ptr->size + j] != 0)
				{
					lowestWeight = ptr->adjMatrix[i * ptr->size + j];
					row = i;
					col = j;
				}
			}
		}

		numVertices = 0;
		vertices = vertices + ptr->labels[row] + ptr->labels[col];

		// getting number of vertices from the string holding vertices
		for(int z = 0; z < ptr->size; z++)
		{
			if(vertices.find(ptr->labels[z]) != string::npos)
				numVertices++;
		}

		// checking for cycles
		if(numEdges + 1 >= numVertices)
		{
			vertices = vertices.substr(0, vertices.length() - 2);

			if(numEdges >= ptr->size)
				return;
		}

		// if no cycles
		else
		{
			ptr->edges[n].row = ptr->labels[row];
			ptr->edges[n].col = ptr->labels[col];
			ptr->edgeWeights[n] = ptr->adjMatrix[row * ptr->size + col];
			numEdges++;
			n++;
		}

		// setting used index to 0
		ptr->adjMatrix[row * ptr->size + col] = 0;
	}
}


void displayMatrix(Data* ptr)
{
	cout << endl << endl << "     Adjacency Matrix" << endl << endl << "     ";

	for(int n = 0; n < ptr->size; n++)
		cout << ptr->labels[n] << "    ";

	cout << endl;

	for(int i = 0; i < ptr->size; i++)
	{
		cout << ptr->labels[i] << " ";

		for(int j = 0; j < ptr->size; j++)
			cout << setw(4) << ptr->adjMatrix[i * ptr->size + j] << " ";

		cout << endl;
	}
}


void  displayEdges(Data* ptr)
{
	int totalWeight = 0;

	cout << endl << "Edges included in the minimal"  << endl << "spanning tree for this graph:" << endl << endl;

	for(int n = 0; n < ptr->size -1; n++)
	{
		cout << "{"<< ptr->edges[n].row << " " << ptr->edges[n].col << "} weight = " << ptr->edgeWeights[n] << endl;
		totalWeight += ptr->edgeWeights[n];
	}

	cout << endl << endl << "Total weight of the minimal spanning tree is: " << totalWeight;
}
