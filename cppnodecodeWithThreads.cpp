#include <iostream>
#include <string>
#include <fstream>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <cstring>
#include <netdb.h>
#include <ctime>

using namespace std;

struct Table{
	int dest;
	int nextHop;
	int dist;
} otherthingHere;

struct Node{
	int id;
	string addr;
	int controlPort;
	int dataPort;
	int neighbors[10];
	string neighAddr[10];
	int neighCPort[10];
	int neighDPort[10];
	Table myTable[5];

} somethingHere;

const int NODES = 5;

void paddr(unsigned char *a)
{
        printf("%d.%d.%d.%d\n", a[0], a[1], a[2], a[3]);
}

void initiateControlThread(Node myNode) 
{
	//cout << "initiateControlThread called by controlThread " << endl;
	// create the socket
	int controlSocket = -1;
	if ((controlSocket = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("cannot create controlSocket");
	}
	else 
	{
		cout << endl << "Successfully created UDP controlSocket" << endl;
		cout << "The controlSocket descriptor is: " << controlSocket << endl;
	}	

	// need to assign the controlPort number from the input file: myNode.controlPort
	// but first need to set up the sockaddr_in struct
	struct sockaddr_in myaddr;
	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY); //do we somehow put in the remoteXX.cs... thing here?
	myaddr.sin_port = htons(myNode.controlPort);

	if (::bind(controlSocket, (struct sockaddr *) &myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
	} else 
	{
		cout << "Successfully bound the control socket" << endl;
	}

	cout << endl << " ——— NOTE: have created socket and bound it to port, not sure how to move forward with sending message to other client. ——— " << endl;

	string aString = "remote02.cs.binghamton.edu";
	struct hostent *hn = gethostbyname(aString.c_str()); ///CHANGED THIS
	
	//cout << "Broken 3" << endl;
	
	for (int i=0; hn->h_addr_list[i] != 0; i++)
	{
         paddr((unsigned char*) hn->h_addr_list[i]); 
		//cout << (unsigned char*) hn->h_addr_list[i];
		//cout << (unsigned char*) hn->h_addr_list[i];
	}	
	cout << endl;

	//cout << "Broken 1" << endl;
	
	struct sockaddr_in nodeaddr;
	memset((char*)&nodeaddr,0,sizeof(nodeaddr));
	nodeaddr.sin_family = AF_INET;
	nodeaddr.sin_port = htons(myNode.neighCPort[0]); 

	memcpy((void*)&nodeaddr.sin_addr, hn->h_addr_list[0], hn->h_length);

	//cout << "Broken 2" << endl;
	
	int random = rand() % 10;
	string myString = "Hello, World!";
	if(random < 5)
	{
		cout << random << endl;
		myString = "Schmello worl";
	}
	
	for(;;)
	{
		// about to send a message 
		if (sendto(controlSocket, myString.c_str(), strlen(myString.c_str()), 0, (struct sockaddr *)&nodeaddr, sizeof(nodeaddr)) < 0) {
			perror("sendto failed");
		}	
	}
	
	//senders and receivers data and control ports will be respectively in sync
	
	
	
	
	
	
	
	

/*  ———————————————————————————————————————————————————————————————
	 Need to use things like
		sendto(…..) 
		struct hostent
		and gethostbyname(….) ??

	if(myNode.id == 1) {
		sleep(2);
		cout << "1 JUST woke up after sleeping " << endl;
		string message = "Hello from a socket!";
		sendto(5002, &message, strlen(message), 0, );
	} else if (myNode.id == 2) {
		sleep(4);
		cout << "2 JUST woke up after sleeping " << endl;
		receiveIt();
	}

———————————————————————————————————————————————————————————————— */
}

void createThreads(Node myNode) 
{
	thread controlThread(initiateControlThread, myNode);
	controlThread.join();
}


int main()
{
	char tempChar = '&';
	int tempID;
	int tempAddr;
	int tempCtrlPort;
	int tempDataPort;
	int tempNeighbor;
	int nodeIndex = 0;
	Node tempNode;
	int realID;
	int neighborIndex;
	Node myNode;
	Node tempNodes[NODES];
	ifstream fin;
	bool isSet[NODES];

	fin.open("input.txt");

	cout << "Node #: ";
	cin >> realID;

	srand(time(NULL));

	//Set initial values
	for(int z = 0; z < NODES; z++)
	{
		myNode.neighbors[z] = -1;
		myNode.neighAddr[z] = "";
		myNode.neighCPort[z] = -1;
		myNode.neighDPort[z] = -1;
		isSet[z] = false;
	}

	//Read the input file
	for(int i = 0; i < NODES; i++)
	{
		neighborIndex = 0;
		fin >> tempNode.id >> tempNode.addr >> tempNode.controlPort >> tempNode.dataPort;
		
		fin >> tempChar;
		
		for(int z = 0; z < 10; z++)
		{
			tempNode.neighbors[z] = -1;
		}
		
		while(tempChar != ';')
		{
			//cout << "Temp" << tempChar << endl;
			tempNode.neighbors[neighborIndex] = tempChar - 48; //MIGHT STILL BE A STRING SOMEHOW FUCKING STRING
			neighborIndex++;				
			fin >> tempChar;
		}
		
		tempNodes[nodeIndex] = tempNode;
		nodeIndex++;
	}
	
	//cout << tempNodes[0].id << ' ' << tempNodes[0].addr << ' ' << tempNodes[0].controlPort << ' ' << tempNodes[0].dataPort << ' ' << endl;
	
	
	
	//fin >> tempID;
	
	tempID = realID;
	
	//Check our node against the input data
	for(int i = 0; i < NODES; i++)
	{
		//cout << "In for loop" << endl;
		//cout << tempNodes[i].id << endl;
		
		if(realID == tempNodes[i].id)
		{
			//cout << "SOMETHING" << endl;
			myNode = tempNodes[i];
			//cout << "WORKED\n";
		}
	}
	
	
	//cout << myNode.addr << ' '	<< myNode.controlPort << ' ' << myNode.dataPort << ' ';
	cout << "Neighbors: ";
	
	//Process the neighbors
	for(int j = 0; myNode.neighbors[j] != -1; j++)
	{
		cout << myNode.neighbors[j] << ' ';
		
		//Fill neighbor info
		for(int k = 0; k < NODES; k++)
		{
			//Match number IDs
			if(myNode.neighbors[j] == tempNodes[k].id)
			{
				myNode.neighAddr[j] = tempNodes[k].addr;
				myNode.neighCPort[j] = tempNodes[k].controlPort;
				myNode.neighDPort[j] = tempNodes[k].dataPort;
			}
		}
		cout << myNode.neighAddr[j] << ' ' << myNode.neighCPort[j] << ' ' << myNode.neighDPort[j] << endl;
	}
	cout << '\n';

	fin.close();

	
	
	
	
	//I believe now we have access to the remote addresses as well as the node we need
	
	
	

	
	//Outer for loop processes actual current node
	for(int i = 0; i < NODES; i++)
	{
		//Inner for loop checks neighbors
		for(int j = 0; j < NODES; j++)
		{
			//Check if node's id
			if(i+1 == myNode.id && isSet[i] == false)
			{
				myNode.myTable[i].dest = myNode.id;
				myNode.myTable[i].nextHop = myNode.id;
				myNode.myTable[i].dist = 0;
				isSet[i] = true;
			}
			//Check if neighbor node
			else if(i+1 == myNode.neighbors[j] && isSet[i] == false)
			{
				myNode.myTable[i].dest = i+1;
				myNode.myTable[i].nextHop = i+1;
				myNode.myTable[i].dist = 1;
				isSet[i] = true;
			}
			//Set all values for current node to -1
			else
			{
				if(isSet[i] == false)
				{
					myNode.myTable[i].dest = -1;
					myNode.myTable[i].nextHop = -1;
					myNode.myTable[i].dist = -1;
				}			
			}
		}
		myNode.myTable[i].dest = i+1;
	}
	
	for(int i = 0; i < NODES; i++)
	{
		cout << myNode.myTable[i].dest << ' ' << myNode.myTable[i].nextHop << ' ' << myNode.myTable[i].dist << endl;
	}

	createThreads(myNode);

	int sock; //Is the socket the port? Don't know how to "caulculate" the socket like the TCP project
	fd_set rfds;
	int retval;













	return 0;
}
