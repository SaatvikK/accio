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
	int server_fd, new_socket;

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
	 getsockopt reads a setting, 
	setsockopt writes one. Every socket has a pile of tunable options; this call flips 
	two bits on the socket you just created.

	*/
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
}