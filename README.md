A simple C++ TCP/UDP server example using: 

1- Iterative server
2- Concurrent with thread pool
3- concurrent with epoll

Description:
The server can be initialized as UDP or TCP server. It should also be provided with a port number and
function that should be called to read the client request from a buffer and write the reply on the same
buffer. All the linux related operations are abstracted away so the client doesnt need to know anything 
about it. 
