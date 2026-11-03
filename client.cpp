#include <iostream>
#include <string>
#include <memory>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

constexpr int PORT = 2762;
constexpr int BUFFER_SIZE = 1024;

int main() {
  int sock = 0;
  struct sockaddr_in servAddr;

  char buffer[BUFFER_SIZE] = {0};

  // Creating the fd
  if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    std::cerr << "Socket creation error" << std::endl;
    return -1;
  }

  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(PORT);

  // Convert IPv4 and 6 addrs from text to binary form

  if(inet_pton(AF_INET, "127.0.0.1", &servAddr.sin_addr) <= 0) {
    std::cerr << "ivalid address or address not supported" << std::endl;
    return -1;
  }

  // Connect

  if(connect(sock, (struct sockaddr*)&servAddr, sizeof(servAddr)) < 0) {
    std::cerr << "connection failed" << std::endl;
    return -1;
  }

  std::string hello = "hi from client";
  send(sock, hello.c_str(), hello.size(), 0);
  std::cout << "hello msg sent" << std::endl;
  ssize_t valread = read(sock, buffer, BUFFER_SIZE);
  std::cout << "received:" << buffer << std::endl;;

  // close the sock
  close(sock);
  return 0;
}