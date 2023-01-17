# Introduction to the project 
## The code is partly copied from geeksforgeeks

### Server info
This is C++ code for a simple TCP/IP server that listens for incoming connections on port 5000. 
The server uses a master socket to listen for incoming connections, and creates a new socket for each incoming connection. 
The server uses the select() function to monitor multiple socket descriptors, and uses the FD_SET, FD_ISSET, and FD_ZERO macros
to manipulate the set of socket descriptors. The server uses the getNumbers() function to convert strings received from clients
into numbers. The server's purpose is to solve the minimal path from A to B on horse walk and it uses different threads for different clients.

### BFS info
The server uses a class called minMoves. The class is used to find the minimum number of moves for a knight to move from a starting position to an ending position on a chess board. The chess board is represented by a two-dimensional array of booleans, with the size of the board being determined by the boardSize variable, which is set to 8 by default. The class has a constructor that takes an optional integer argument to set the size of the board. The class has a getMinMoves method that takes a starting position and an ending position as arguments and returns the minimum number of moves for the knight to reach the ending position from the starting position. The method uses a breadth-first search algorithm to find the minimum number of moves.

### Clients info
Both of the clients that connect to a server are using a TCP socket. The client attempts to connect to the server at IP address "127.0.0.1" and port 5000. Once a connection is established, the client reads any data that the server sends and displays it to the user. The client then prompts the user to enter starting point, ending point and size of the field, and sends this input to the server. The client then reads the response from the server and displays it to the user. The server use this input to solve the minimal path from A to B on horse walk and returns the result. The client then closes the socket and terminates the program.
