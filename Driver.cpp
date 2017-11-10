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

const int NODES = 5;

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

void paddr(unsigned char *a)
{
        printf("%d.%d.%d.%d\n", a[0], a[1], a[2], a[3]);
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
	Node tempNodes[NODES + 1];
	ifstream fin;
	bool isSet[NODES];
	
	
	
	myNode.id = 0;
	myNode.addr = "remote00.cs.binghamton.edu";
	myNode.controlPort = 5999;
	myNode.dataPort = 5998;
	
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
	
	
	fin.open("input.txt");
	
	//Read the input file
	for(int i = 0; i < NODES; i++)
	{
		//cout << "For " << i << endl;
		neighborIndex = 0;
		fin >> tempNode.id >> tempNode.addr >> tempNode.controlPort >> tempNode.dataPort;
		
		cout << tempNode.id << ' ' << tempNode.addr << ' ' << tempNode.controlPort << ' ' << tempNode.dataPort << endl;
		
		fin >> tempChar;
		
		for(int z = 0; z < 10; z++)
		{
			tempNode.neighbors[z] = -1;
		}
		
		//cout << "Middle\n";
		
		while(tempChar != ';')
		{
			//cout << "Temp" << tempChar << endl;
			tempNode.neighbors[neighborIndex] = tempChar - 48; //MIGHT STILL BE A STRING SOMEHOW FUCKING STRING
			neighborIndex++;				
			fin >> tempChar;
		}
		
		tempNodes[nodeIndex] = tempNode;
		cout << tempNodes[nodeIndex].addr << endl;
		nodeIndex++;
	}
	
	//cout << tempNodes[1].id << ' ' << tempNodes[1].addr << ' ' << tempNodes[1].controlPort << ' ' << tempNodes[1].dataPort << ' ' << endl;
	
	string myCommand;
	
	
	struct sockaddr_in myaddr;
	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY); //do we somehow put in the remoteXX.cs... thing here?
	myaddr.sin_port = htons(5999);

	if (::bind(controlSocket, (struct sockaddr *) &myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
	} else 
	{
		cout << "Successfully bound the control socket" << endl;
	}
	
	
	//g = Generate Packet, c = create link, r = remove link
	while(1)
	{
		cin >> myCommand;
		int value1,value2;
		if(myCommand.at(0) == 'g')
		{
			cin >> value1 >> value2;
			
			string myString = "g";
			
			//Process the nodes
		//	for(int j = 0; j < NODES; j++)
		//	{
				//cout << "For j " << j << endl;
				string aString = tempNodes[value1 - 1].addr;
				//cout << aString << endl;
				struct hostent *hn = gethostbyname(aString.c_str());
				
				
				//cout << aString << endl;
				for (int i=0; hn->h_addr_list[i] != 0; i++){
			
				cout << "insid ethe lkjadf" << endl;
						paddr((unsigned char*) hn->h_addr_list[i]);
				//cout << (unsigned char*) hn->h_addr_list[i];
				//cout << (unsigned char*) hn->h_addr_list[i];
				}	
				
				struct sockaddr_in nodeaddr;
				//cout << "Halfway\n";
				memset((char*)&nodeaddr,0,sizeof(nodeaddr));
				nodeaddr.sin_family = AF_INET;
				nodeaddr.sin_port = htons(tempNodes[value1 - 1].dataPort); 
				//cout << "()()()()()()())()()()(())" <<  endl;
				socklen_t myLength = sizeof(nodeaddr);

				memcpy((void*)&nodeaddr.sin_addr, hn->h_addr_list[0], hn->h_length);
					
					
				myString += to_string(value2);	
				
				//cout << "Before if\n";
				if(value1 == tempNodes[value1 - 1].id)
				{
				//	cout << "In if\n";
					//cout << "Sending " << tempNodes[i].id << " to " << myNode.neighbors[j] << endl;
					// about to send a message 
					if (sendto(controlSocket, myString.c_str(), strlen(myString.c_str()), 0, (struct sockaddr *)&nodeaddr, sizeof(nodeaddr)) < 0)
					{	
						perror("sendto failed");	
					}
				}
			//}
		}
		else if(myCommand.at(0) == 'c')
		{
			cin >> value1 >> value2;
			
			string myString = "c";
			
			//Process the nodes
		//	for(int j = 0; j < NODES; j++)
		//	{
				//cout << "For j " << j << endl;
				string aString = tempNodes[value1 - 1].addr;
				//cout << aString << endl;
				struct hostent *hn = gethostbyname(aString.c_str());
				
				
				//cout << aString << endl;
				for (int i=0; hn->h_addr_list[i] != 0; i++){
			
				cout << "insid ethe lkjadf" << endl;
						paddr((unsigned char*) hn->h_addr_list[i]);
				//cout << (unsigned char*) hn->h_addr_list[i];
				//cout << (unsigned char*) hn->h_addr_list[i];
				}	
				
				struct sockaddr_in nodeaddr;
				//cout << "Halfway\n";
				memset((char*)&nodeaddr,0,sizeof(nodeaddr));
				nodeaddr.sin_family = AF_INET;
				nodeaddr.sin_port = htons(tempNodes[value1 - 1].controlPort); 
				//cout << "()()()()()()())()()()(())" <<  endl;
				socklen_t myLength = sizeof(nodeaddr);

				memcpy((void*)&nodeaddr.sin_addr, hn->h_addr_list[0], hn->h_length);
					
					
				myString += to_string(value2);	
				
				//cout << "Before if\n";
				if(value1 == tempNodes[value1 - 1].id)
				{
				//	cout << "In if\n";
					//cout << "Sending " << tempNodes[i].id << " to " << myNode.neighbors[j] << endl;
					// about to send a message 
					if (sendto(controlSocket, myString.c_str(), strlen(myString.c_str()), 0, (struct sockaddr *)&nodeaddr, sizeof(nodeaddr)) < 0)
					{	
						perror("sendto failed");	
					}
				}
			//}
		}
		else if(myCommand.at(0) == 'r')
		{
			cin >> value1 >> value2;
			
			string myString = "r";
			
			//Process the nodes
		//	for(int j = 0; j < NODES; j++)
		//	{
				//cout << "For j " << j << endl;
				string aString = tempNodes[value1 - 1].addr;
				//cout << aString << endl;
				struct hostent *hn = gethostbyname(aString.c_str());
				
				
				//cout << aString << endl;
				for (int i=0; hn->h_addr_list[i] != 0; i++){
			
				cout << "insid ethe lkjadf" << endl;
						paddr((unsigned char*) hn->h_addr_list[i]);
				//cout << (unsigned char*) hn->h_addr_list[i];
				//cout << (unsigned char*) hn->h_addr_list[i];
				}	
				
				struct sockaddr_in nodeaddr;
				//cout << "Halfway\n";
				memset((char*)&nodeaddr,0,sizeof(nodeaddr));
				nodeaddr.sin_family = AF_INET;
				nodeaddr.sin_port = htons(tempNodes[value1 - 1].controlPort); 
				//cout << "()()()()()()())()()()(())" <<  endl;
				socklen_t myLength = sizeof(nodeaddr);

				memcpy((void*)&nodeaddr.sin_addr, hn->h_addr_list[0], hn->h_length);
					
					
				myString += to_string(value2);	
				
				//cout << "Before if\n";
				if(value1 == tempNodes[value1 - 1].id)
				{
				//	cout << "In if\n";
					//cout << "Sending " << tempNodes[i].id << " to " << myNode.neighbors[j] << endl;
					// about to send a message 
					if (sendto(controlSocket, myString.c_str(), strlen(myString.c_str()), 0, (struct sockaddr *)&nodeaddr, sizeof(nodeaddr)) < 0)
					{	
						perror("sendto failed");	
					}
				}
			//}
		}
	}
	
	
	
	
	
	fin.close();
	
	
	
	
	
	
	
	return 0;
}