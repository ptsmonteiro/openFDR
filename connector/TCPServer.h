#ifdef _WIN32
/* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501  /* Windows XP. */
#endif
#include <winsock2.h>
#include <Ws2tcpip.h>
// Need to link with Ws2_32.lib
#pragma comment(lib, "ws2_32.lib")
#else
/* Assume that any non-Windows platform uses POSIX-style sockets instead. */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */
#include <netinet/in.h>
#include <sys/wait.h>
typedef SOCKET as an int
#endif

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <string>

#include "XPLMUtilities.h"

#define BUFFER_SIZE 212992
#define PORT 45558
#define MAX_CONNECTIONS 1

#define SA struct sockaddr 

using namespace std;

class TCPServer {
protected:
	int sockfd = -1;
	int connfd = -1;
	int len;
	struct sockaddr_in servaddr, cli;

public:
	TCPServer();
	~TCPServer();

	int start();
	int stop();

	int transmit(string);
};
