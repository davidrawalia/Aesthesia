#pragma once

#define BUFFER_LEN 512
#define PORT 7474

SOCKET udp_socket;
struct sockaddr_in server_addr;
struct sockaddr_in client_addr;
int client_addr_len = sizeof(client_addr);
int packet_len;
char buffer[BUFFER_LEN];
char source_IP[INET_ADDRSTRLEN];
WSADATA wsa;

rapidjson::Document document;
float amplitude;
int data_index;
int smoothing_value;
double smoothing_subtractor;
float last_packet_data[128];
