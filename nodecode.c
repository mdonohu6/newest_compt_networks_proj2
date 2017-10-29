#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int nodes = 5;

typedef struct Nodes{
	int id;
	char addr[26];
	int port1;
	int port2;
	int neighbors[10];
} myNode;

int main()
{
	FILE *inputFile;
	inputFile = fopen("input.txt","r");

	int i;
	char tempID;
	int realID;
	
	struct Nodes myNode;

	printf("Node #: ");
	scanf("%d", &realID);

	//realID = atoi(tempID);
	

	//printf("Here1\n");
	//printf("%s %d",tempID,realID);
	//printf("Here2\n");
	//printf("We aint dum %d\n",realID);
	for (i = 0; i < nodes; i++)
	{
		









	}



	

	/*

	read in things here



	set up stuff here





	packet running code here





	*/

















	fclose(inputFile);

	return 0;
}
