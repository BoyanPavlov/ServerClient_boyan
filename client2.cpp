#include <string.h> // strlen
#include <sys/socket.h>
#include <arpa/inet.h> // close
#include <unistd.h>    //close

#include <iostream>

using std::cout;

#define PORT 5000

// brief intro
// project for horse walk - find the minimal path from A to B
// threads - different clients
// solver - the server - returns a possitive number if there is a path

int main(int argc, char const *argv[])
{
    int sock = 0, valread, client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        std::cout << "\n Socket creation error \n";
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        std::cout << "\nInvalid address/ Address not supported \n";
        return -1;
    }

    if ((client_fd = connect(sock, (struct sockaddr *)&serv_addr,
                             sizeof(serv_addr))) < 0)
    {
        std::cout << "\nConnection Failed \n";
        return -1;
    }

    char buffer0[1024] = {};

    valread = read(sock, buffer0, 1024);
    if (valread)
    {
        std::cout << buffer0 << std::endl;
    }

    std::cout << "Please enter starting point and ending point and size of the field, \n";
    std::cout << "Please enter valid input \n";

    // Input: 0 0 11 11 12
    // Output: 8

    std::cin.getline(buffer, 1024);
    send(sock, buffer, strlen(buffer), 0);

    char buffer1[1024] = {};

    valread = read(sock, buffer1, 1024);
    if (valread)
    {
        std::cout << buffer1 << std::endl;
    }

    // closing the connected socket
    close(client_fd);
    return 0;
}
