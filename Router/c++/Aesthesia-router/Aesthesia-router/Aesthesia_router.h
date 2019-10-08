#pragma once

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")
#pragma warning(disable:4996)

#define BUFLEN 512
#define PORT 7474

SOCKET s;
struct sockaddr_in server, si_other;
int slen, recv_len;
char buf[BUFLEN];
rapidjson::Document d;
std::string data_string;
float data;
int dataIndex;
int smoothingAmount;
float smoothing;
float last_packet_data[128];
WSADATA wsa;