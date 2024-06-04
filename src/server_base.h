#ifndef SERVER_BASE
#define SERVER_BASE

#ifdef __WIN32__
# include <winsock2.h>
#include <sys/types.h>
#include <WS2tcpip.h>
#else
# include <sys/socket.h>
#include <netinet/in.h>
#endif


/// @brief This sturucture defines the input params for creating a socket server
struct ServerParams
{
    int domain;
    int server_type;
    int protocol;
    int port;
    int backlog;

    struct sockaddr_in address;
    unsigned long interface_address;
};

struct Server
{
    struct ServerParams server_prams;
    int socket;
    void (*handler)(struct Server*);

};


/// @brief This function is used to create the socket server
/// @param socket socket params used to create the socket server
/// @param socket_handler handler function for the sever's incomming client connection
/// @return the description of the created socket server 
struct Server create_socket_server(struct ServerParams socket, void (*socket_handler)(struct Server*));



#endif