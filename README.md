# CS118 Accio - TCP File Server

## Reference
https://web.cs.ucla.edu/classes/spring17/cs118/project-1.html

## Project Brief

**CS118: Computer Network Fundamentals — Project 1 ("Accio")** from UCLA (Spring 2017, Prof. Alexander Afanasyev).

Build a simple client-server file transfer application using raw TCP sockets. Implemented in C++ (C++11 allowed for non-networking parts like threading/string parsing). **No high-level network abstractions** (Boost.Asio, etc.) — only Berkeley sockets.

### Server (`server`)
- Runs as `./server <PORT> <FILE-DIR>`
- Listens on specified port, accepts connections from any interface
- Creates a new thread per client connection
- Saves received files as `<FILE-DIR>/<CONNECTION-ID>.file` (1.file, 2.file, etc.)
- 10-second receive timeout → writes `ERROR` (no newline) and aborts
- Handles SIGTERM/SIGQUIT → exit code 0
- Invalid port → print `ERROR:` to stderr, non-zero exit
- Supports files up to 100 MiB

### Client (`client`)
- Runs as `./client <HOSTNAME> <PORT> <FILENAME>`
- Connects to server, transfers entire file, then closes connection
- 10-second connect timeout, 10-second send-buffer timeout
- Invalid hostname/port → print `ERROR:` to stderr, non-zero exit
- Exits code 0 on successful transfer
- Supports files up to 100 MiB

### Key Challenges
- **TCP fragmentation handling**: data may arrive split across reads; both sides must handle partial sends/receives correctly
- **Concurrent connections**: server must handle multiple simultaneous clients (thread-per-connection model)
- **Timeouts**: both sides need proper timeout handling (server receive timeout, client connect/send timeouts)
- **Graceful shutdown**: SIGTERM/SIGQUIT on server, clean connection close after full transfer on client
- **Large file support**: up to 100 MiB — buffer management matters, especially with delay/lossy network tests

### Grading Highlights
- 2.5 pts: ≥3 git commits
- ~15 pts each: medium (1 MiB) and large (100 MiB) file transfers with chunking/delay tests
- 10 pts: 10 sequential + 10 concurrent clients
- 5 pts each: error handling, timeouts, lossy network resilience
- Extra credit: Docker container for server deployment

### Environment
Reference uses Ubuntu 16.04 via Vagrant, with a skeleton `Makefile`, `server.cpp`, and `client.cpp`.
