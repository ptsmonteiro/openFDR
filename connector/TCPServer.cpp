#include "TCPServer.h"

using namespace std;

TCPServer::TCPServer() {
}

TCPServer::~TCPServer() {
}

// Change the socket into non-blocking state
void setNonBlocking(SOCKET s) {
#ifdef _WIN32
	u_long iMode = 1; // non-blocking IO
	ioctlsocket(s, FIONBIO, &iMode);
#else
	fcntl(s, F_SETFL, O_NONBLOCK);
#endif
}

int TCPServer::start() {
#ifdef _WIN32
	WSADATA wsa_data;
	if (WSAStartup(MAKEWORD(1, 1), &wsa_data) != 0) {
		XPLMDebugString("openFDR: failed to start WSA\n");
		return -1;
	}
#endif

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd <= 0) {
		XPLMDebugString("openFDR: socket creation failed\n");
		return -1;
	}

	setNonBlocking(sockfd);

	memset(&servaddr, 0, sizeof(servaddr));

	// assign IP, PORT 
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(PORT);

	// Binding newly created socket to given IP and verification
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
		XPLMDebugString("openFDR: socket bind failed...\n");
		return -1;
	}

	// Now server is ready to listen and verification
	if ((listen(sockfd, MAX_CONNECTIONS)) != 0) {
		XPLMDebugString("openFDR: Listen failed...\n");
		return -1;
	}
	len = sizeof(cli);

	return 0;
}

int TCPServer::stop() {

#ifdef _WIN32
	shutdown(sockfd, SD_BOTH);
	closesocket(sockfd); 
	WSACleanup();
#else
	shutdown(sockfd, SHUT_RDWR);
	close(sockfd); 
}
#endif

	sockfd = -1;
	connfd = -1;

	return 0;
}

int TCPServer::transmit(string payload) {
	char buffer[BUFFER_SIZE];
	int bytes_sent = 0;
	bool ready = false;

	if (connfd <= 0) {
		// Accept incoming connections
		connfd = accept(sockfd, (SA*)&cli, &len);
		if (connfd > 0) {
			// Mark the incomig connection socket as non blocking
			setNonBlocking(connfd);
		}
	}

	if (connfd > 0) {
		// Send payload via the established connection
		bytes_sent = send(connfd, payload.c_str(), payload.length(), 0);
		if (bytes_sent <= 0) {
			// Assume connection lost
			connfd = -1;
			return -1;
		}
		else if (bytes_sent < payload.length()) {
			XPLMDebugString("openFDR: Unable to send all the payload...\n");
			return -1;
		}
	}

	return 0;
}