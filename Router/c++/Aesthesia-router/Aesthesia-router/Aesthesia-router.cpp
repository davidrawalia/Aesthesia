#include "pch.h"
#include "Aesthesia_router.h"

int main()
{
	//Initialise winsock
	std::cout << "Initialising Winsock"; 
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		std::cout << "\nFailed.Error Code : " << WSAGetLastError();
		exit(EXIT_FAILURE);
	}
	std::cout << ("\nInitialised.");

	//Create a socket
	if ((udp_socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
	{
		std::cout << "Could not create socket : " << WSAGetLastError();
	}
	std::cout << "Socket created.\n";

	//Prepare the sockaddr_in structure
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	//Bind
	if (bind(	udp_socket,
				(struct sockaddr *)&server_addr, 
				sizeof(server_addr)) 
		== SOCKET_ERROR)
	{
		std::cout << "Bind failed with error code : " << WSAGetLastError();
		exit(EXIT_FAILURE);
	}
	std::cout << "Bind done";

	//Create shared memory to route data to visualizer  
	using namespace boost::interprocess;
	try {
		shared_memory_object::remove("shared_memory");
		managed_shared_memory segment(create_only, "shared_memory", 65536);

		float *shared_data = segment.construct<float>("data")[128](100.0);

		//keep listening for data
		while (1)
		{
			//clear the buffer by filling null, it might have previously received data
			memset(buffer, '\0', BUFFER_LEN);

			for (int i = 0; i < 127; i++) {
				last_packet_data[i] = *(shared_data + i);
			}

			//try to receive some data, this is a blocking call
			if ((packet_len = recvfrom(udp_socket, 
									   buffer,
									   BUFFER_LEN, 
									   0, 
								       (struct sockaddr*) &client_addr, 
					                   &client_addr_len)) 
				 == SOCKET_ERROR)
			{
				std::cout << "recvfrom() failed with error code: " << WSAGetLastError();
				exit(EXIT_FAILURE);
			}
			inet_ntop(AF_INET, &(client_addr.sin_addr), source_IP, INET_ADDRSTRLEN);


			//print details of the client/peer and the data received
			std::cout << "Received packet from " << source_IP << ": "
					  << ntohs(client_addr.sin_port) << "\n" << "Data: " 
					  << buffer << "\n";

			//parse buffer data and write to shared memory
			document.Parse(buffer);
			amplitude = document["data"].GetFloat();
			data_index = document["sourceID"].GetInt();

			//smooth data
			smoothing_value = document["smoothing"].GetInt();
			smoothing_subtractor = (200 / pow(smoothing_value, 0.3)) - 40;
			if (smoothing_value > 0 && last_packet_data[data_index] > amplitude
				&& last_packet_data[data_index] > 50)
			{
				amplitude = last_packet_data[data_index] - smoothing_subtractor;
			}
			
			//write data to shared memory
			*(shared_data + data_index) = amplitude;
		}

		//Destroy shared memory  
		using namespace boost::interprocess;
		shared_memory_object::remove("shared_memory");
	}
	catch (interprocess_exception e) {
		std::cout << e.what() << '\n';
	}

	closesocket(udp_socket);
	WSACleanup();

	return 0;
}