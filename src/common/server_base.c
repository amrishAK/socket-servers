#include "server_base.h"
#include <stdio.h>     // Grants us the use of standard input/output functions for logging and error messages.
#include <stdlib.h>    // Provides essential functions such as exit() for terminating the program.
#include <string.h>    // Allows us to work with strings, which will be useful for handling text data.
#include <unistd.h>  
# include <winsock2.h>
# include <errors.h>

struct Server create_socket_server(struct ServerParams socket_params, void (*socket_handler)(struct Server*))
{
    int option = 1;
    struct Server server;
    

    int socket_d = socket(socket_params.domain,socket_params.server_type, socket_params.protocol);
    
    if(socket_d < 0)
    {
        printf("Error creating socket descriptor return value due to %s\n", socket_d, strerror(errno));
        goto cleanup;
    }

    int socket_option_response = setsockopt(socket_d, SOL_SOCKET, SO_REUSEADDR, (const char*)&option, sizeof(option));

    if(socket_option_response < 0)
    {
        printf("Error occurred while setting socket options due to  %s\n", strerror(errno));
        goto cleanup;
    }
    
    int bind_response = bind(socket_d, (struct sockaddr*)&socket_params.address, sizeof(socket_params.address));

    if(bind_response < 0)
    {
        printf("Error occurred while binding the socket due to %s\n", strerror(errno));
        goto cleanup;
    }

    int listen_response = listen(socket_d, socket_params.backlog);

    if(listen_response < 0)
    {
        printf("Error occurred while starting to listen due to %s\n", strerror(errno));
        goto cleanup;
    }

    
    server.socket = socket_d;
    server.handler= socket_handler;

    return server;

    cleanup:
        close(socket_d);
        server.socket = -1;
        return server;
}
