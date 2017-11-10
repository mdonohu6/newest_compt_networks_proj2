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



#include <mutex>

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
	
	// this is node 1's address (the sender)
	string sendersAddr = "remote01.cs.binghamton.edu";
	struct hostent *hn = gethostbyname(myNode.addr.c_str());

	for (int i=0; hn->h_addr_list[i] != 0; i++){
	
		cout << "insid ethe lkjadf222" << endl;
                paddr((unsigned char*) hn->h_addr_list[i]);
		//cout << (unsigned char*) hn->h_addr_list[i];
		//cout << (unsigned char*) hn->h_addr_list[i];
	}	
	cout << endl;

	struct sockaddr_in nodeaddr;
	memset((char*)&nodeaddr,0,sizeof(nodeaddr));
	nodeaddr.sin_family = AF_INET;
	nodeaddr.sin_port = htons(5000);

	socklen_t myLength = sizeof(nodeaddr);

	memcpy((void*)&nodeaddr.sin_addr, hn->h_addr_list[0], hn->h_length);

	unsigned char buf[13];
	
	

	while(1) {
		ssize_t recvlen = recvfrom(controlSocket, buf, 13, 0, (struct sockaddr *)&nodeaddr, &myLength);
            printf("received %d bytes\n", recvlen);
            if (recvlen > 0) {
                    buf[recvlen] = 0;
                    printf("received message: \"%s\"\n", buf);
            }
    }
	


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

	///--------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
	
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
	///-------------------------------------------------------------------------------------------------------------------------------------------------------------------
	
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


	

	unsigned char buf[50];
	
	
	
	//cout << "Before\n";
	
	int retval;
	fd_set rfds;
	
	FD_ZERO(&rfds);
	
	FD_SET(controlSocket, &rfds);
	
	int inc = 0;
	
	mutex mtx;
	
	while(1) {
		
		FD_ZERO(&rfds);
		FD_SET(controlSocket, &rfds);
		
		struct sockaddr_in nodeaddr;
		memset((char*)&nodeaddr,0,sizeof(nodeaddr));
		nodeaddr.sin_family = AF_INET;
		
		int tempPort;
		//forloop here
		
		//Loop through all temp input data
		for(int i = 0; i < NODES; i++)
		{
			//cout << "For loop " << i << endl;
			
			string sendersAddr = tempNodes[i].addr;
			
			/*
			// this is node 1's address (the sender)
			if(i == 0) ///THIS IS HARD CODED, ASSUMING ONLY 5 NODES
			{
				sendersAddr = "remote01.cs.binghamton.edu";
			}
			else if (i == 1)
			{
				sendersAddr = "remote02.cs.binghamton.edu";
			}
			else if (i == 2)
			{
				sendersAddr = "remote03.cs.binghamton.edu";
			}
			else if (i == 3)
			{
				sendersAddr = "remote04.cs.binghamton.edu";
			}
			else if (i == 4)
			{
				sendersAddr = "remote05.cs.binghamton.edu";
			}
					*/
			//sendersAddr = "remote01.cs.binghamton.edu";
			struct hostent *hn = gethostbyname(sendersAddr.c_str());

			for (int i=0; hn->h_addr_list[i] != 0; i++){
			
				cout << "insid ethe lkjadf" << endl;
						paddr((unsigned char*) hn->h_addr_list[i]);
				//cout << (unsigned char*) hn->h_addr_list[i];
				//cout << (unsigned char*) hn->h_addr_list[i];
			}	
			cout << endl;
		
			nodeaddr.sin_port = htons(tempNodes[i].controlPort);
		
			socklen_t myLength = sizeof(nodeaddr);
		
			memcpy((void*)&nodeaddr.sin_addr, hn->h_addr_list[0], hn->h_length);
			
			
			cout << "In here\n";
			
			struct timeval myTime;
			myTime.tv_sec = 5;
			
			
			retval = select(controlSocket + 1, &rfds, NULL, NULL, &myTime);
			cout << "After select\n";
			
			//check from first file descriptor up until controlSocket
			if(FD_ISSET(controlSocket, &rfds)) //The only thing this changes is there is no 1 line alternating of the 2 strings, now we have only large chunks of the 2 strings. Upgrade or downgrade?
			{
				cout << "In if\n";
				//mtx.lock();
				//Receive the buffer string
				ssize_t recvlen = recvfrom(controlSocket, buf, 50, 0, (struct sockaddr *)&nodeaddr, &myLength);
				printf("received %d bytes\n", recvlen);
				if (recvlen > 0) {
						buf[recvlen] = 0;
						printf("received message: \"%s\"\n", buf);
						inc++;
						cout << inc << endl; //Sometimes will alternate between the two strings, sometimes will print one string 47-77 times before switching
				}
				
				//Determine if receiving a distance vector
				if(buf[0] == 'v')
				{
					int vectors = (recvlen - 1) / 3;
					cout << "Vectors: " << vectors << endl;
					
					Table* tempTable = (Table*) malloc (vectors * sizeof(int) * 9); //9 works but why?
					int tempIndex = 0;
					
					//Fill up the temp table
					for(int i = 1; i < recvlen; i++)
					{
						tempTable[tempIndex].dest = (int)buf[(i * 3) - 2] - 48;
						tempTable[tempIndex].nextHop = (int)buf[(i * 3) - 1] - 48;
						tempTable[tempIndex].dist = (int)buf[(i * 3)] - 48;
						tempIndex++;
					}
					
					//Print own vectors
					for(int i = 0; i < NODES; i++)
					{
						cout << myNode.myTable[i].dest << ' ' << myNode.myTable[i].nextHop << ' ' << myNode.myTable[i].dist << endl;
					}
					
					cout << "-----------------------------------------\n";
					
					//Print received vectors
					for(int i = 0; i < vectors; i++)
					{
						cout << tempTable[i].dest << ' ' << tempTable[i].nextHop << ' ' << tempTable[i].dist << endl;
					}
					
					//Keeps track of which nodes weren't accounted for
					bool accounted[5];
					
					//Determine if shorter paths or new paths
					//Process our list
					for(int i = 0; i < NODES; i++)
					{
						//Process received list
						for(int j = 0; j < vectors; j++)
						{
							//If destinations match up
							if(myNode.myTable[i].dest == tempTable[j].dest)
							{
								//cout << myNode.myTable[i].dest << " = " << tempTable[j].dest << endl;
								//Determine if shorter path
								if(myNode.myTable[i].dist > tempTable[j].dist + 1) 
								{
								//	cout << myNode.myTable[i].dist << " > " << tempTable[j].dist << " + 1" << endl;
									myNode.myTable[i].dist = tempTable[j].dist + 1;
									//if(myNode.myTable[i].dist == 0) myNode.myTable[i].dist++;
									myNode.myTable[i].nextHop = tempTable[j].dest;
								}
								else if(myNode.myTable[i].dist == -1)
								{
									
									///THIS PART WORKS, BUT THE NEXT HOP IS WRONG. FIGURE OUT HOW TO TELL NODE 2 THAT IT HAS TO HOP TO NODE 1 INSTEAD OF 4 AND 5 FOR DEST'S 4 AND 5
									
									//cout << "Negatives\n";
									//cout << tempTable[j].dest << ' ' << tempTable[j].dist << endl;
								
									myNode.myTable[i].dist = tempTable[j].dist + 1;
									
									//if(tempTable[j].dist + 1 == 0)
									//{
										//cout << "++++++++++++++++++++++++++++++\n";
										//myNode.myTable[i].dist++;
									for(int z = 0; z < NODES; z++)
									{
									//	cout << tempTable[z].dest << endl;
										if(tempTable[z].dist == 0)
										{
											myNode.myTable[i].nextHop = tempTable[z].dest;
										}
									}
									//}
										//myNode.myTable[i].nextHop = tempTable[j].dest;
								}
							}
							//Else if new path
							//else if()
							//{
								//Maybe not do it here
							//}
						}
					}
							
							
					cout << "-------------------------------------------------------\n";
							
					/*
					cout << tempTable[0].dest << ' ' << tempTable[0].nextHop << ' ' << tempTable[0].dist << '\n';
					cout << tempTable[1].dest << ' ' << tempTable[1].nextHop << ' ' << tempTable[1].dist << '\n';
					cout << tempTable[2].dest << ' ' << tempTable[2].nextHop << ' ' << tempTable[2].dist << '\n';
					*/
							
					//Print own vectors
					for(int i = 0; i < NODES; i++)
					{
						cout << myNode.myTable[i].dest << ' ' << myNode.myTable[i].nextHop << ' ' << myNode.myTable[i].dist << endl; ///DOESNT OUTPUT 4 AND 5 CORRECTLY, SO GOTTA FIX THIS TOMORROW
					}
							
							free(tempTable);
							cout << "Here" << endl;
				}
						cout << "Out here" << endl;
						
						
			}
		}
			//mtx.unlock();
			
			
			
	}
		
	

		//pthread attach and pthread detach
		//use lock only when a new distance vector comes in
		//data field changes to include each hop as a packet travels
		//packet ID can be an arbitrary values thats incremented after each packet is generated
	
	//use select() call to wait on multiple client ports once everybody is set up to see who is sending you data, only use for receiving
	//use recvfrom AFTER select so you know who you're receiving from
	//use FD_ISSET() to see WHICH file descriptor has the data
	//clear and reset FD at end of while loop
	
	//what is the relationship between the mini threads we were told to use to process the multiple nodes to read in from at the same time and the 2 main ones
	// we have now? would locking between select() and recvfrom() help alleviate this problem?
	//mutex from slides
	
	
	
	
	
	
	
	
	
	
	
	
	
/*	fin.open("input.txt");

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

	//cout << myNode.addr << ' ' << myNode.controlPort << ' ' << myNode.dataPort << ' ';
	cout << "Neighbors: ";
	for(int j = 0; myNode.neighbors[j] != -1; j++)
	{
		cout << myNode.neighbors[j] << ' ';
	}
	cout << '\n';

	fin.close();
*/

	//createThreads(myNode);

	//int sock; //Is the socket the port? Don't know how to "caulculate" the socket like the TCP project
	//fd_set rfds;
	//int retval;

	return 0;
}
