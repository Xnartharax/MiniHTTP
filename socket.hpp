#ifndef SOCKET_HPP
#define SOCKET_HPP
#include <sys/socket.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <openssl/ssl.h>
#include <sys/select.h>
#include <poll.h>
#include <string>
#include <string.h>
#include <ctime>
#include <vector>
#include <map>
#include <thread>
#include <mutex>
#include <algorithm>
#define SOCKET_BUFFER_SIZE 2048
class Socket{
    protected:
        char buffer[2048];
        char * current;
        virtual bool read_buffer();
        int sock;
        friend class SocketMultiplexer;
    public:
        bool invalid;
        std::mutex mtx;
        std::string read_header();
        int rc;
        char * read_body(int content_length);
        Socket(int sock);
        int timeout; 
        bool keepalive;
        virtual void sock_send(const char*, int);
        virtual ~Socket();
};
class TLSSocket : public Socket {
    protected:
        SSL_CTX *ctx;
        SSL * ssl;
        bool read_buffer() override;
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
        friend class SocketMultiplexer;
    public:
        HandlerSock(int port);
        virtual Socket * accept();
};
class TLSHandlerSock: public HandlerSock{
    protected:
        SSL_CTX * ctx;
        const SSL_METHOD * method;
    public:
        TLSHandlerSock(int, std::string, std::string);
        TLSSocket * accept() override;
};
class SocketMultiplexer{
    private:
        std::mutex mtx;
        std::vector<pollfd> fds; 
        std::map<int, HandlerSock *> mastersocks;
        std::map<int, Socket *> socks;
        void handleTimeouts();
    public:
        Socket * getNextSock();
        void addMasterSock(HandlerSock *);
        void addSock(Socket *);
        
};
#endif
