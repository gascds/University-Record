# Simple Text Messenger on LAN using C++

### Background:

> Implementtation of a simple text messenger where two participantscan send text message to each other. The participants can both send and receive messages where their communication is created based on TCP protocol.

### Task list:

**Task 1: Socket initialization**

Description: initialize and create the TCP socket 

Both computers can initiate the connection. Suppose that two computers A and B would like to communicate with each other. If computer A serves as the “server” to wait for computer B to initiate the connection, computer A needs to get the inputs about its IP address and port number to listen. Then computer A needs to conduct the following task

**Task 2: Bind IP and port, listen and accept connections**

Description:

- Bind the input local IP and port number.

- Mark this socket as accepting connections.

- Accept a connection on the socket. 

Computer B inputs Computer A’s IP address and port number that Computer A is waiting for connection. Then Computer B conducts the following task.

**Task 3: Connect to the peer with the given IP address and port number**

**Task 4: Send and receive the text**

Description: each computer can send or receive the text to or from the other participant.

### Useful Libraries for Implementation 

> sys/socket.h: Main sockets header 
> 
> netinet/in.h: Internet address family 
> 
> arpa/inet.h: Definitions for internet 
> 
> WinSock2
> 
> WS2tcpip