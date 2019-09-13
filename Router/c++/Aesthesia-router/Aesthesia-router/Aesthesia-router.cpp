// Aesthesia-router.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#pragma warning(disable:4996)

#define BUFLEN 512
#define PORT 7474

int main()
{
	SOCKET s;
	struct sockaddr_in server, si_other;
	int slen, recv_len;
	char buf[BUFLEN];
	std::string data;
	WSADATA wsa;

	slen = sizeof(si_other);

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed.Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	//Create a socket
	if ((s = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}
	printf("Socket created.\n");

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);

	//Bind
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	puts(" Bind done");

	//Create shared memory to route data to visualizer  
	using namespace boost::interprocess;
	try {
		shared_memory_object::remove("shared_memory");
		shared_memory_object shm_obj(create_only, "shared_memory", read_write);
		shm_obj.truncate(10);
		mapped_region region(shm_obj, read_write);

		//keep listening for data
		while (1)
		{

			//clear the buffer by filling null, it might have previously received data
			memset(buf, '\0', BUFLEN);

			//try to receive some data, this is a blocking call
			if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr *) & si_other, &slen)) == SOCKET_ERROR)
			{
				printf("recvfrom() failed with error code : %d", WSAGetLastError());
				exit(EXIT_FAILURE);
			}

			//print details of the client/peer and the data received
			printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
			printf("Data: %s\n", buf);

			//parse buffer data and write to shared memory
			data = buf;
			data = data.substr(5, 6);
			std::memset(region.get_address(), std::stoi(data), 1);

			//now reply the client with the same data
			if (sendto(s, buf, recv_len, 0, (struct sockaddr*) & si_other, slen) == SOCKET_ERROR)
			{
				printf("sendto() failed with error code : %d", WSAGetLastError());
				exit(EXIT_FAILURE);
			}
		}

		//Destroy shared memory  
		using namespace boost::interprocess;
		shared_memory_object::remove("shared_memory");
	}
	catch (interprocess_exception e) {
		std::cout << e.what() << '\n';
	}

	closesocket(s);
	WSACleanup();

	return 0;
}