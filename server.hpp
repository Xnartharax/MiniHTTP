#ifndef SERVER_HPP
#define SERVER_HPP
#include <sys/socket.h>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <future>
#include <iostream>
#include "response.hpp"
#include "request.hpp"
#include "ressourcmapper.hpp"
#define HTTP_MSIZE 8192
class HTTPServer{
	private:
		int sockfd;
        struct sockaddr_in *address;
        int addrlen;
        int handle(int socket);
        RessourceMapper *mapper;
        
	public:
		HTTPServer(int port, const char * ressource_path);
		int mainloop();
};
#endif
