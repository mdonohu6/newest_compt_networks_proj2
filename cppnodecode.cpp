#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Node{
	int id;
	string addr;
	int controlPort;
	int dataPort;
	int neighbors[10];

} somethingHere;

const int Nodes = 5;

int main()
{
	char tempChar = '&';
	int tempID;
	int realID;
	int neighborIndex = 0;
	Node myNode;
	ifstream fin;

	fin.open("input.txt");

	cout << "Node #: ";
	cin >> realID;

	for(int z = 0; z < 10; z++)
	{
		myNode.neighbors[z] = -1;
	}

	for(int i = 0; i < Nodes; i++)
	{
		fin >> tempID;

		if(tempID == realID)
		{
			myNode.id = realID;
			fin >> myNode.addr >> myNode.controlPort >> myNode.dataPort;
			
			fin >> tempChar;
			while(tempChar != ';')
			{
				//cout << "Temp" << tempChar << endl;
				myNode.neighbors[neighborIndex] = tempChar - 48; //MIGHT STILL BE A STRING SOMEHOW FUCKING STRING
				neighborIndex++;				
				fin >> tempChar;
			}
		}
		else
		{
			fin.ignore(100, '\n');
		}
	}

	cout << myNode.addr << ' ' << myNode.controlPort << ' ' << myNode.dataPort << ' ';

	for(int j = 0; j < neighborIndex + 3; j++)
	{
		cout << myNode.neighbors[j] << ' ';
	}
	cout << '\n';

	fin.close();

	return 0;
}
