#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <sys/socket.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string>
#include <string.h>
#define SOCKET_BUFFER_SIZE 2048
class Socket{
    private:
    char buffer[2048];
    char * current;
    void read_buffer();
    int sock;
    public:
        std::string read_header();
        char * read_body(int content_length);
        Socket(int sock);
        void sock_send(const char*, int);
        ~Socket();
};
#endif
