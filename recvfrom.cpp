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

using namespace std;

struct Node{
	int id;
	string addr;
	int controlPort;
	int dataPort;
	int neighbors[10];

} somethingHere;

const int Nodes = 5;

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
	
		cout << "insid ethe lkjadf" << endl;
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

	unsigned char buf[12];
	
	

	while(1) {
		ssize_t recvlen = recvfrom(controlSocket, buf, 12, 0, (struct sockaddr *)&nodeaddr, &myLength);
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
	int realID;
	int neighborIndex = 0;
	Node myNode;
	ifstream fin;

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
	myaddr.sin_port = htons(5002);

	if (::bind(controlSocket, (struct sockaddr *) &myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
	} else 
	{
		cout << "Successfully bound the control socket" << endl;
	}

	// this is node 1's address (the sender)
	string sendersAddr = "remote01.cs.binghamton.edu";
	struct hostent *hn = gethostbyname(sendersAddr.c_str());

	for (int i=0; hn->h_addr_list[i] != 0; i++){
	
		cout << "insid ethe lkjadf" << endl;
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
	
	
	cout << "Before\n";
	while(1) {
		cout << "In here\n";
		ssize_t recvlen = recvfrom(controlSocket, buf, 13, 0, (struct sockaddr *)&nodeaddr, &myLength);
                printf("received %d bytes\n", recvlen);
                if (recvlen > 0) {
                        buf[recvlen] = 0;
                        printf("received message: \"%s\"\n", buf);
                }
        }
	

	
	
	
	
	//use select() call to wait on multiple client ports once everybody is set up to see who is sending you data, only use for receiving
	//use recvfrom AFTER select so you know who you're receiving from
	//use FD_ISSET() to see WHICH file descriptor has the data
	//clear and reset FD at end of while loop
	
	
	
	
	
	
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

	int sock; //Is the socket the port? Don't know how to "caulculate" the socket like the TCP project
	fd_set rfds;
	int retval;

	return 0;
}
