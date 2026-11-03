#include <iostream>
#include <string>
#include <memory>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

constexpr int PORT = 2762;
constexpr int BUFFER_SIZE = 1024;

int main() {

	// server_fd is the `file dscriptor` for this process.
	// An FD is basically a small bit of information the Linux Kernel stores about
	// every process.
	// This FD includes the socket that this program has requested from the kernel.
	int server_fd, newSocket;

	struct sockaddr_in address;

	int opt = 1;

	socklen_t addrlen = sizeof(address);
	char buffer[BUFFER_SIZE] = {0};

	// socket(AF_INET, SOCK_STREAM, 0) asks the OS to spin up a TCP/IP socket
	// and returns a file descriptor on success.
	// If no file descriptor, error handle.
	if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	/*
	The "change a setting on this socket" syscall:
	`getsockopt` reads a setting, `setsockopt` writes one. Every socket has a 
	pile of tunable options; this call flips two bits on the socket you just created.

	Params:
	- server_fd: Which socket
	- SOL_SOCKET: Which layer
	- SOL_REUSEADDR | SO_REUSEPORT: Which option(s) to set
	- &opt: What values to set them at
	*/

	// SOL_SOCKET == Generic socket behaviour.
	// SO_REUSEADR == Lets you use `bind()` to a port that's still lingering from
	// a previous run

	// Returns `0` on SUCCESS and `-1` on FAILURE
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Bind the socket to the network address and port

	if(bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}

	// Start listening for incoming connections
	// If listen() returns a negative num then obv error
	if(listen(server_fd, 3) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	std::cout << "Server is listening on port " << PORT << std::endl;

	// Accept incoming connection
	if((newSocket = accept(server_fd, (struct sockaddr*)&address, &addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}

	// Read and echo the received message
	ssize_t valread = read(newSocket, buffer, BUFFER_SIZE);
	std::cout << "Received: " << buffer << std::endl;
	std::string res = "hi from server";
	//send(newSocket, buffer, res.c_str(), 0);
	send(newSocket, res.c_str(), res.size(), 0);
	std::cout << "Response sent" << std::endl;

	// Close the socket
	close(newSocket);
	close(server_fd);
	return 0;
}