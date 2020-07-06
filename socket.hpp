#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <sys/socket.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <openssl/ssl.h>
#include <string>
#include <string.h>
#define SOCKET_BUFFER_SIZE 2048
class Socket{
    protected:
        char buffer[2048];
        char * current;
        virtual void read_buffer();
        int sock;
    public:
        std::string read_header();
        char * read_body(int content_length);
        Socket(int sock);
        virtual void sock_send(const char*, int);
        virtual ~Socket();
};
class TLSSocket : public Socket {
    protected:
        SSL_CTX *ctx;
        SSL * ssl;
        void read_buffer() override;
    public:
        TLSSocket(int sock, SSL_CTX *ctx);
        void sock_send(const char *, int) override;
        ~TLSSocket() override;
};
class HandlerSock{
    protected:
        int sock;
        struct sockaddr_in * address;
        int port;
    public:
        HandlerSock(int port);
        virtual Socket * accept();
};
class TLSHandlerSock: HandlerSock{
    protected:
        SSL_CTX * ctx;
        const SSL_METHOD * method;
    public:
        TLSHandlerSock(int, std::string, std::string);
        TLSSocket * accept() override;
};
#endif
