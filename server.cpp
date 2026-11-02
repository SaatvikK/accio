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
	int server_fd, new_socket;
}