/** A simple web server made in C!
 * Author: Gabriel Francisco
 * Goal: This is a simple project to understand waht happens
 * with a bare metal server that uses plain sockets connections
 * and HTTP principles
 */

#include <stdio.h>      // IO operations
#include <stdlib.h>     // Utils functions(exit, malloc)
#include <string.h>     // String manipulation
#include <unistd.h>     // Unix syscalls
#include <sys/socket.h> // Sockets definitions
#include <netinet/in.h> // Struct for addresses storage

// Constants

#define PORT 8080 // Port number

/*
 * Request buffer size
 * 30000 bytes(about 30KB) is a good and standard size for normal servers
 */
#define BUFFER_SIZE 30000

int main()
{
    int server_fd;              // File descriptor of server socket
    int new_socket;             // File descriptor of the client socket
    int val_read;               // Number of bytes read
    struct sockaddr_in address; // Struct for address(IP/Port) definitions
    int addrlen = sizeof(address);

    // Model of a fixed HTTP response(Header + Body)
    // HTTP/1.1 200 OK: Succes state
    // Content-Type: (plain text)
    // Content-Length: Size of the response
    // \r\n\r\n: Mark the end of the Header and the start of Body

    char *test = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Lenght: 12\r\n\r\nHello World.God bless you!";

    // -----------------------------------
    // 1 - Socket creation
    // -----------------------------------
    // AF_INET      = IPv4
    // SOCK_STREAM  = TCP
    // 0            = Protocol(0 lets the system chose de standard to TCP)

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Error while creating the socket");
        exit(EXIT_FAILURE);
    }
}