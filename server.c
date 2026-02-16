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
#include <asm-generic/socket.h>

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

    // -----------------------------------------------------------------
    // 1 - Socket creation
    // -----------------------------------------------------------------
    // AF_INET      = IPv4
    // SOCK_STREAM  = TCP
    // 0            = Protocol(0 lets the system chose de standard to TCP)

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Error while creating the socket");
        exit(EXIT_FAILURE);
    }

    // -----------------------------------------------------------------
    // 2 - Socket options config
    // -----------------------------------------------------------------
    // Prevents the "Address already in use" error while restarting the server
    // It reutilizes the port and address at the time
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("Fail in setsockopt");
        exit(EXIT_FAILURE);
    }

    // -----------------------------------------------------------------
    // 3 - Defining the address
    // -----------------------------------------------------------------

    address.sin_family = AF_INET; // IPv4 family

    // Accepts connections of any network interface
    address.sin_addr.s_addr = INADDR_ANY;

    // Converts the port number to network order
    address.sin_port = htons(PORT);

    // -----------------------------------------------------------------
    // 4 - Bind
    // -----------------------------------------------------------------
    // Glue the socket to the PORT and the address defined above

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("Failed to bind!");
        exit(EXIT_FAILURE);
    }

    // -----------------------------------------------------------------
    // 5 - Listen
    // -----------------------------------------------------------------
    // Puts the server on passive mode, waiting for connections.
    // 10 = Backlog

    if (listen(server_fd, 10) < 0)
    {
        perror("Failed to listen!");
        exit(EXIT_FAILURE);
    }

    printf("** Server listening on port %d **", PORT);

    while (1)
    {
        printf("\nWaiting for connection...\n");

        // ACCEPT: Blocks the connection until a client connects.
        // When connects, creates a new socket to it client
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("Fail in ACCEPT!");
            continue;
        }

        // Reading the request
        // Clear the buffer with 0s
        char buffer[BUFFER_SIZE] = {0};

        // Read the values sent by browser
        val_read = read(new_socket, buffer, BUFFER_SIZE);
        printf("\nMessage received:\n%s\n", buffer);

        // Sending the request
        //  Writes the string on client socket
        write(new_socket, test, strlen(test));
        printf("\n--> Response sent!\n");

        // Closing the connection
        close(new_socket);
    }

    return 0;
}