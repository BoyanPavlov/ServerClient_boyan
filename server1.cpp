
#include <iostream>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>    //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h> //FD_SET, FD_ISSET, FD_ZERO macros
#include <cmath>

// brief intro
// project for horse walk - find the minimal path from A to B
// threads - different clients
// solver - TCP/IP server - returns a possitive number if there is a path

#include "playField.hpp"

#define PORT 5000

// function for converting string to numbers
unsigned int *getNumbers(const char *text)
{
    unsigned int *arr = new unsigned int[4]{};
    if (!arr)
    {
        return nullptr;
    }

    int len = strlen(text);
    int result = 0;
    int index = 0;
    bool flag = false;
    bool negative = false;

    for (size_t i = 0; i < len; i++)
    {
        if ('0' <= text[i] && '9' >= text[i])
        {
            result = 10 * result + (text[i] - '0');
            flag = true;
        }

        else if (flag && text[i] == ' ')
        {

            arr[index] = result;

            index++;
            result = 0;
        }
        else // space
        {
            flag = false;
        }
    }

    if (result && flag)
    {
        arr[index] = result;
        result = 0;
        flag = false;
    }

    return arr;
}

int main(int argc, char *argv[])
{
    int opt = 1;
    int master_socket = 0;
    int addrlen = 0;
    int new_socket = 0;
    int client_socket[30] = {}; // initialise all client_socket[] to 0 as not checked
    const int max_clients = 30;
    int activity = 0;
    int valread = 0;
    int sd = 0;
    int max_sd;
    struct sockaddr_in address;
    std::string toReturn;
    char buffer[1025];

    // set of socket descriptors
    fd_set readfds;

    // a message for the clients
    char *message = "Greetings, now you are using the horse field\n";

    // creating a master socket
    if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        std::cerr << "socket failed";
        return -1;
    }

    // set master socket to allow multiple connections ,
    // this is just a good habit, it will work without this
    if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
                   sizeof(opt)) < 0)
    {
        std::cerr << "error in setsockopt\n";
        return -1;
    }

    // type of socket created, part for windows
    address.sin_family = AF_INET;         // specifying the address family of socket
    address.sin_addr.s_addr = INADDR_ANY; // binding the socket to all available interfaces on the host
    address.sin_port = htons(PORT);

    // bind the socket to localhost port 5000
    if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        std::cerr << "bind failed\n";
        return -1;
    }
    std::cout << "Listener on port " << PORT << "\n";

    // specifing the maximum of 5 pending connections for the master socket
    if (listen(master_socket, 5) < 0) // creating a queue for binding
    {
        std::cerr << "listening failed";
        return -1;
    }

    // accept the incoming connection
    addrlen = sizeof(address);
    std::cout << "Waiting for connections ... \n";

    while (true)
    {
        // function for clearing the socket set
        FD_ZERO(&readfds);

        // add master socket to set
        FD_SET(master_socket, &readfds);
        max_sd = master_socket;

        // add child sockets to set
        for (int i = 0; i < max_clients; i++)
        {
            // socket descriptor
            sd = client_socket[i];

            // if valid socket descriptor then add to read list
            if (sd > 0)
            {
                FD_SET(sd, &readfds);
            }

            // highest file descriptor number, need it for the select function
            if (sd > max_sd)
            {
                max_sd = sd;
            }
        }

        // wait for an activity on one of the sockets, timeout is NULL - waits indefinitely
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);

        if ((activity < 0) && (errno != EINTR))
        {
            std::cerr << "select error";
        }

        // If something happened on the master socket,
        // then its an incoming connection
        if (FD_ISSET(master_socket, &readfds))
        {
            if ((new_socket = accept(master_socket,
                                     (struct sockaddr *)&address,
                                     (socklen_t *)&addrlen)) < 0)
            {
                std::cerr << "accept error";
                return -1;
            }

            // inform user of socket number - used in send and receive commands
            std::cout << "New connection, socket_fd:" << new_socket
                      << ", ip: " << inet_ntoa(address.sin_addr)
                      << ", port : " << ntohs(address.sin_port)
                      << std::endl;

            // send new connection greeting message
            if (send(new_socket, message, strlen(message), 0) != strlen(message))
            {
                std::cerr << "send error";
            }

            std::cout << "The greeting message was send successfully to the user with port: "
                      << ntohs(address.sin_port) << std::endl;

            // add new socket to array of sockets
            for (int i = 0; i < max_clients; i++)
            {
                if (!client_socket[i])
                {
                    client_socket[i] = new_socket;
                    std::cout << "Added to the list of sockets as " << i << std::endl;
                    break;
                }
            }
        }

        // else its some IO operation on some other socket
        for (int i = 0; i < max_clients; i++)
        {
            sd = client_socket[i];

            if (FD_ISSET(sd, &readfds))
            {
                // Check if it was for closing, and also read the incoming message
                if ((valread = read(sd, buffer, 1024)) == 0)
                {
                    // One client has been disconnected, gets his details and print
                    getpeername(sd, (struct sockaddr *)&address,
                                (socklen_t *)&addrlen);
                    std::cout << "Host disconnected, ip " << inet_ntoa(address.sin_addr)
                              << " , port " << ntohs(address.sin_port) << "\n";

                    // Closing the socket and mark as 0 in list for reuse
                    close(sd);
                    client_socket[i] = 0;
                }

                // Echo back the message that came in
                else
                {
                    buffer[valread] = '\0';

                    // here we execute the BFS (horse walk)
                    unsigned int *arr = getNumbers(buffer);

                    minMoves ms(arr[4]);
                    int result = ms.getMinMoves({arr[0], arr[1]}, {arr[2], arr[3]});

                    toReturn = "If number is possitive - max moves to reach your destination is: ";
                    std::string temp = std::to_string(result);

                    toReturn.append(temp);

                    delete[] arr;

                    if (send(sd, toReturn.c_str(), toReturn.size(), 0) != toReturn.size())
                    {
                        std::cerr << "send error";
                    }
                }
            }
        }
    }

    return 0;
}