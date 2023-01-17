# Introduction to the project 
## The code is partly copied from geeksforgeeks

This is C++ code for a simple TCP/IP server that listens for incoming connections on port 5000. 
The server uses a master socket to listen for incoming connections, and creates a new socket for each incoming connection. 
The server uses the select() function to monitor multiple socket descriptors, and uses the FD_SET, FD_ISSET, and FD_ZERO macros
to manipulate the set of socket descriptors. The server uses the getNumbers() function to convert strings received from clients
into numbers. The playField.hpp file is also included in the code, but it is not shown here. The server's purpose is to 
solve the minimal path from A to B on horse walk and it uses different threads for different clients.
