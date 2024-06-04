#include "server_base.h"
#include <stdio.h>
#include <stdlib.h> // Provides essential functions such as exit() for terminating the program.
#include <string.h> // Allows us to work with strings, which will be useful for handling text data.
#include <unistd.h>
#include <signal.h>


#define MAX 1024
int flag =0;
void socket_handler(struct Server *server);
void interrupt_handler(int var);

void create_echo_server()
{

    printf("Start");

    struct ServerParams socket_params;
    socket_params.domain = AF_INET;
    socket_params.protocol = 0;
    socket_params.server_type = SOCK_STREAM;
    socket_params.port = 8080;
    socket_params.backlog = 10;
    socket_params.address.sin_family = AF_INET;
    socket_params.address.sin_addr.s_addr =  htonl(INADDR_ANY);
    socket_params.address.sin_port = htons(8080);

    struct Server server = create_socket_server(socket_params, socket_handler);
    server.handler(&server);
}

int main()
{
    signal(SIGINT,interrupt_handler);
    WSADATA wsaData;
    int err = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (err != 0)
    {
        printf("Error while WSA startup \n");
        return 1;
    }

    create_echo_server();

    WSACleanup();
    return 0;
}

void socket_handler(struct Server *server)
{
    printf("Server running...waiting for connections.\n");

    char buffer[1024]; // A buffer to store incoming data from client requests.
    int read_size;

    // An infinite loop that keeps the server perpetually ready to accept new connections.
    struct sockaddr_in client_address;             // Client address structure to hold the details of the connecting client.
        socklen_t client_len = sizeof(client_address); // Length of the client address structure.

        // The server accepts an incoming connection request and creates a new socket for client communication.
        int new_socket = accept(server->socket, (struct sockaddr *)&client_address, &client_len);
        if (new_socket < 0)
        {
            perror("Accept failed"); // If the server fails to accept the connection, it logs the error.
        }
        else
        {
            printf("Connection accepted\n"); // Confirmation that a new connection has been successfully established.
            strcpy(buffer, "Hello from server! This is an echo sever, if you want to disconnect send EXIT");
            send(new_socket, buffer, strlen(buffer), 0);
        }
        
        while(1)
        {
            Sleep(500); // 500 millisecond delay
            memset(buffer, 0, sizeof(buffer)); // to clear the buffer
            recv(new_socket, buffer, sizeof(buffer), 0); // to check the client socket from any message

            // Check if the message is exit to close the connection
            if (strncmp("EXIT", buffer, 4) == 0) {
                printf("Client socket closed\n");
                break;
            }
            
            send(new_socket, buffer, strlen(buffer), 0); // send back the received message

            if(flag != 0)
            {
                break;
            }
        }

        printf("Cleaning up");
        close(new_socket);
        close(server->socket);
}

void interrupt_handler(int var)
{
    flag = 1;
}
