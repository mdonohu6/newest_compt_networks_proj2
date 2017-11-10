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

void initiateControlThread(Node myNode, Node tempNodes[]) 
{
	cout << "initiateControlThread called by controlThread " << endl;
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
	//cout << myNode.neighCPort[0] << "()()()()()()())()()()(())" <<  endl;

	memcpy((void*)&nodeaddr.sin_addr, hn->h_addr_list[0], hn->h_length);

	//cout << "Broken 2" << endl;
	
	
	int random = rand() % 10;
	
	
	
	string randSome;
	
	/*
	if(random < 5)
	{
		cout << random << endl;
		myString = "Schmello worl";
	}
	
	*/
	
	//myString = "Hello World";
	//sending "hey there buddy" makes recvfrom get 3 different messages, each of the 3 words, spaces aren't counted
	
	//cout << myString << endl;
	
	//for(;;)
	//{
		
		
		
	unsigned char buf[50];
	
	
	
	//cout << "Before\n";
	
	int retval;
	fd_set rfds;
	
	FD_ZERO(&rfds);
	
	FD_SET(controlSocket, &rfds);
	
	int inc = 0;
	
	mutex mtx;
	
	/*
	if(myNode.id == 1)
	{
		cout << "Heyo" << endl;
		// about to send a message 
		if (sendto(controlSocket, myString.c_str(), strlen(myString.c_str()), 0, (struct sockaddr *)&nodeaddr, sizeof(nodeaddr)) < 0)
		{
			perror("sendto failed");	
		}
	}
	*/
	
	while(1) {
		
		FD_ZERO(&rfds);
		FD_SET(controlSocket, &rfds);
		
		struct sockaddr_in nodeaddr;
		memset((char*)&nodeaddr,0,sizeof(nodeaddr));
		nodeaddr.sin_family = AF_INET;
		
		int tempPort;
		//forloop here
		
		//Loop through all temp input data
		for(int i = 0; i < NODES + 1; i++)
		{
			//cout << "For loop " << i << endl;
			
			string sendersAddr = tempNodes[i].addr;
			
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
			
			
			//cout << "In here\n";
			
			struct timeval myTime;
			myTime.tv_sec = 1;
			
			
			retval = select(controlSocket + 1, &rfds, NULL, NULL, &myTime);
			//cout << "After select\n";
			
			//check from first file descriptor up until controlSocket
			if(FD_ISSET(controlSocket, &rfds)) //The only thing this changes is there is no 1 line alternating of the 2 strings, now we have only large chunks of the 2 strings. Upgrade or downgrade?
			{
				//cout << "In if\n";
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
									
									//cout << "Negatives\n";
									cout << tempTable[j].dest << ' ' << tempTable[j].dist << endl;
								
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
											//if()
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
					//cout << "Here" << endl;
				}
				else if(buf[0] == 'c')
				{
					int tempNum = (int)buf[1] - 48;
					//cout << "Linking to " << tempNum << endl;
					
					for(int w = 0; w < NODES; w++)
					{
						if(myNode.myTable[w].dest == tempNum)
						{
							myNode.myTable[w].nextHop = tempNum;
							myNode.myTable[w].dist = 1;
						}
					}
					
					//Print own vectors
					for(int i = 0; i < NODES; i++)
					{
						cout << myNode.myTable[i].dest << ' ' << myNode.myTable[i].nextHop << ' ' << myNode.myTable[i].dist << endl; ///DOESNT OUTPUT 4 AND 5 CORRECTLY, SO GOTTA FIX THIS TOMORROW
					}
				}
				else if(buf[0] == 'r')
				{
					int tempNum = (int)buf[1] - 48;
					//cout << "Linking to " << tempNum << endl;
					
					for(int w = 0; w < NODES; w++)
					{
						if(myNode.myTable[w].dest == tempNum)
						{
							myNode.myTable[w].nextHop = -1;
							myNode.myTable[w].dist = -1;
						}
					}
					
					//Print own vectors
					for(int i = 0; i < NODES; i++)
					{
						cout << myNode.myTable[i].dest << ' ' << myNode.myTable[i].nextHop << ' ' << myNode.myTable[i].dist << endl; ///DOESNT OUTPUT 4 AND 5 CORRECTLY, SO GOTTA FIX THIS TOMORROW
					}
				}
				//cout << "Out here" << endl;
			}
		}
		
		/*
		string sendersAddr = "remote00.cs.binghamton.edu";
		struct hostent *hn = gethostbyname(sendersAddr.c_str());		
		nodeaddr.sin_port = htons(tempNodes[i].controlPort);
		socklen_t myLength = sizeof(nodeaddr);
		memcpy((void*)&nodeaddr.sin_addr, hn->h_addr_list[0], hn->h_length);
		*/
		
		//Process the nodes to send vector to
		string aString = "";
		
		string myString = "v";
	
		//Create a string to represent vector node
		//Format: vABCXYZ - A Node ID, B Destination, C Distance
		for(int i = 0; i < NODES; i++)
		{
			//Determine if node is reachable and isn't own self
			if(myNode.myTable[i].dist >= 0)
			{
				myString += to_string(myNode.myTable[i].dest) + to_string(myNode.myTable[i].nextHop) + to_string(myNode.myTable[i].dist);
			}
		}
		
		for(int i = 0; i < NODES; i++)
		{
			string aString = tempNodes[i].addr;
			struct hostent *hn = gethostbyname(aString.c_str()); ///CHANGED THIS
			
			//cout << "Broken 3" << endl;
			
			/*
			for (int i=0; hn->h_addr_list[i] != 0; i++)
			{
				 paddr((unsigned char*) hn->h_addr_list[i]); 
				//cout << (unsigned char*) hn->h_addr_list[i];
				//cout << (unsigned char*) hn->h_addr_list[i];
			}*/	
			cout << endl;

			//cout << "Broken 1" << endl;
			
			struct sockaddr_in nodeaddr;
			memset((char*)&nodeaddr,0,sizeof(nodeaddr));
			nodeaddr.sin_family = AF_INET;
			nodeaddr.sin_port = htons(myNode.neighCPort[0]); 
			//cout << myNode.neighCPort[0] << "()()()()()()())()()()(())" <<  endl;

			memcpy((void*)&nodeaddr.sin_addr, hn->h_addr_list[0], hn->h_length);
			
			//Process the neighbors
			for(int j = 0; j < NODES; j++)
			{
				if(tempNodes[i].id == myNode.neighbors[j])
				{
					//cout << "Sending " << tempNodes[i].id << " to " << myNode.neighbors[j] << endl;
					// about to send a message 
					if (sendto(controlSocket, myString.c_str(), strlen(myString.c_str()), 0, (struct sockaddr *)&nodeaddr, sizeof(nodeaddr)) < 0)
					{	
						perror("sendto failed");	
					}
				}
			}
		}	
			//mtx.unlock();		
	}	
		
		
		
		
		
		
		
		
		
	
//	}
	
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







//}


	
void initiateDataThread(Node myNode, Node tempNodes[])
{
	
	cout << "initiateControlThread called by controlThread " << endl;
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
	//cout << myNode.neighCPort[0] << "()()()()()()())()()()(())" <<  endl;

	memcpy((void*)&nodeaddr.sin_addr, hn->h_addr_list[0], hn->h_length);

	//cout << "Broken 2" << endl;
	
	
	int random = rand() % 10;
	
	
	
	string randSome;
	
	/*
	if(random < 5)
	{
		cout << random << endl;
		myString = "Schmello worl";
	}
	
	*/
	
	//myString = "Hello World";
	//sending "hey there buddy" makes recvfrom get 3 different messages, each of the 3 words, spaces aren't counted
	
	//cout << myString << endl;
	
	//for(;;)
	//{
		
		
		
	unsigned char buf[50];
	
	
	
	//cout << "Before\n";
	
	int retval;
	fd_set rfds;
	
	FD_ZERO(&rfds);
	
	FD_SET(controlSocket, &rfds);
	
	int inc = 0;
	
	mutex mtx;
	
	/*
	if(myNode.id == 1)
	{
		cout << "Heyo" << endl;
		// about to send a message 
		if (sendto(controlSocket, myString.c_str(), strlen(myString.c_str()), 0, (struct sockaddr *)&nodeaddr, sizeof(nodeaddr)) < 0)
		{
			perror("sendto failed");	
		}
	}
	*/
	
	while(1) {
		
		FD_ZERO(&rfds);
		FD_SET(controlSocket, &rfds);
		
		struct sockaddr_in nodeaddr;
		memset((char*)&nodeaddr,0,sizeof(nodeaddr));
		nodeaddr.sin_family = AF_INET;
		
		int tempPort;
		//forloop here
		
		//Loop through all temp input data
		for(int i = 0; i < NODES + 1; i++)
		{
			//cout << "For loop " << i << endl;
			
			string sendersAddr = tempNodes[i].addr;
			
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
			
			
			//cout << "In here\n";
			
			struct timeval myTime;
			myTime.tv_sec = 1;
			
			
			retval = select(controlSocket + 1, &rfds, NULL, NULL, &myTime);
			//cout << "After select\n";
			
			//check from first file descriptor up until controlSocket
			if(FD_ISSET(controlSocket, &rfds)) //The only thing this changes is there is no 1 line alternating of the 2 strings, now we have only large chunks of the 2 strings. Upgrade or downgrade?
			{
				//cout << "In if\n";
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
									
									//cout << "Negatives\n";
									cout << tempTable[j].dest << ' ' << tempTable[j].dist << endl;
								
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
											//if()
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
					//cout << "Here" << endl;
				}
				else if(buf[0] == 'c')
				{
					int tempNum = (int)buf[1] - 48;
					//cout << "Linking to " << tempNum << endl;
					
					for(int w = 0; w < NODES; w++)
					{
						if(myNode.myTable[w].dest == tempNum)
						{
							myNode.myTable[w].nextHop = tempNum;
							myNode.myTable[w].dist = 1;
						}
					}
					
					//Print own vectors
					for(int i = 0; i < NODES; i++)
					{
						cout << myNode.myTable[i].dest << ' ' << myNode.myTable[i].nextHop << ' ' << myNode.myTable[i].dist << endl; ///DOESNT OUTPUT 4 AND 5 CORRECTLY, SO GOTTA FIX THIS TOMORROW
					}
				}
				else if(buf[0] == 'r')
				{
					int tempNum = (int)buf[1] - 48;
					//cout << "Linking to " << tempNum << endl;
					
					for(int w = 0; w < NODES; w++)
					{
						if(myNode.myTable[w].dest == tempNum)
						{
							myNode.myTable[w].nextHop = -1;
							myNode.myTable[w].dist = -1;
						}
					}
					
					//Print own vectors
					for(int i = 0; i < NODES; i++)
					{
						cout << myNode.myTable[i].dest << ' ' << myNode.myTable[i].nextHop << ' ' << myNode.myTable[i].dist << endl; ///DOESNT OUTPUT 4 AND 5 CORRECTLY, SO GOTTA FIX THIS TOMORROW
					}
				}
				//cout << "Out here" << endl;
			}
		}
		
		/*
		string sendersAddr = "remote00.cs.binghamton.edu";
		struct hostent *hn = gethostbyname(sendersAddr.c_str());		
		nodeaddr.sin_port = htons(tempNodes[i].controlPort);
		socklen_t myLength = sizeof(nodeaddr);
		memcpy((void*)&nodeaddr.sin_addr, hn->h_addr_list[0], hn->h_length);
		*/
		
		//Process the nodes to send vector to
		string aString = "";
		
		string myString = "v";
	
		//Create a string to represent vector node
		//Format: vABCXYZ - A Node ID, B Destination, C Distance
		for(int i = 0; i < NODES; i++)
		{
			//Determine if node is reachable and isn't own self
			if(myNode.myTable[i].dist >= 0)
			{
				myString += to_string(myNode.myTable[i].dest) + to_string(myNode.myTable[i].nextHop) + to_string(myNode.myTable[i].dist);
			}
		}
		
		for(int i = 0; i < NODES; i++)
		{
			string aString = tempNodes[i].addr;
			struct hostent *hn = gethostbyname(aString.c_str()); ///CHANGED THIS
			
			//cout << "Broken 3" << endl;
			
			/*
			for (int i=0; hn->h_addr_list[i] != 0; i++)
			{
				 paddr((unsigned char*) hn->h_addr_list[i]); 
				//cout << (unsigned char*) hn->h_addr_list[i];
				//cout << (unsigned char*) hn->h_addr_list[i];
			}*/	
			cout << endl;

			//cout << "Broken 1" << endl;
			
			struct sockaddr_in nodeaddr;
			memset((char*)&nodeaddr,0,sizeof(nodeaddr));
			nodeaddr.sin_family = AF_INET;
			nodeaddr.sin_port = htons(myNode.neighCPort[0]); 
			//cout << myNode.neighCPort[0] << "()()()()()()())()()()(())" <<  endl;

			memcpy((void*)&nodeaddr.sin_addr, hn->h_addr_list[0], hn->h_length);
			
			//Process the neighbors
			for(int j = 0; j < NODES; j++)
			{
				if(tempNodes[i].id == myNode.neighbors[j])
				{
					//cout << "Sending " << tempNodes[i].id << " to " << myNode.neighbors[j] << endl;
					// about to send a message 
					if (sendto(controlSocket, myString.c_str(), strlen(myString.c_str()), 0, (struct sockaddr *)&nodeaddr, sizeof(nodeaddr)) < 0)
					{	
						perror("sendto failed");	
					}
				}
			}
		}	
			//mtx.unlock();		
	}	
	
	
}

void createThreads(Node myNode, Node tempNodes[]) 
{
	thread controlThread(initiateControlThread, myNode, tempNodes);
	controlThread.join();
	thread dataThread(initiateDataThread, myNode, tempNodes);
	dataThread.join();
}


int main()
{
	char tempChar = '&';
	int tempID;
	int tempAddr;
	int tempCtrlPort;
	int tempDataPort;
	int tempNeighbor;
	int nodeIndex = 1;
	Node tempNode;
	int realID;
	int neighborIndex;
	Node myNode;
	Node tempNodes[NODES + 1];
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

	tempNodes[0].id = 0;
	tempNodes[0].addr = "remote00.cs.binghamton.edu";
	tempNodes[0].controlPort = 5999;
	tempNodes[0].dataPort = 5998;
	
	
	//Read the input file
	for(int i = 1; i < NODES + 1; i++)
	{
		cout << "For " << i << endl;
		neighborIndex = 0;
		fin >> tempNode.id >> tempNode.addr >> tempNode.controlPort >> tempNode.dataPort;
		
		fin >> tempChar;
		
		for(int z = 0; z < 10; z++)
		{
			tempNode.neighbors[z] = -1;
		}
		
		cout << "Middle\n";
		
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
	
	cout << tempNodes[0].id << ' ' << tempNodes[0].addr << ' ' << tempNodes[0].controlPort << ' ' << tempNodes[0].dataPort << ' ' << endl;
	
	
	
	//fin >> tempID;
	
	tempID = realID;
	
	//Check our node against the input data
	for(int i = 0; i < NODES + 1; i++)
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
		for(int k = 0; k < NODES + 1; k++)
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

	createThreads(myNode, tempNodes);

	int sock; //Is the socket the port? Don't know how to "caulculate" the socket like the TCP project
	fd_set rfds;
	int retval;













	return 0;
}
