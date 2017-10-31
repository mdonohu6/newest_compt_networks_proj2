#include <iostream>
#include <netinet/in.h>

using namespace std;

int main()
{

	// REQUEST A 'PHONE LINE' I.E. OPEN UP A UPD SOCKET
	int controlSock = -1;
	if ((controlSock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
	{
		perror("cannot create controlSocket");
		return 0;
	} else {
		cout << endl << "Successfully created UDP controlSocket" << endl;
		cout << "The controlSocket descriptor is: " << controlSock << endl;
	}

	// ASSIGN A PORT NUMBER TO THE controlSOCKET
	// first need to set up the sockaddr_in struct

	struct sockaddr_in myaddr;

	/* bind to an arbitrary return address */
	/* because this is the client side, we don't care about the address */
	/* since no application will initiate communication here - it will */
	/* just send responses */
	/* INADDR_ANY is the IP address and 0 is the socket */
	/* htonl converts a long integer (e.g. address) to a network representation */
	/* htons converts a short integer (e.g. port) to a network representation */

	memset((char *)&myaddr, 0, sizeof(myaddr));
	myaddr.sin_family = AF_INET;
	myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	myaddr.sin_port = htons(5000);

	if (bind(dataSocket, (struct sockaddr *) &myaddr, sizeof(myaddr)) < 0) {
		perror("bind failed");
		return 0;
	}
}

