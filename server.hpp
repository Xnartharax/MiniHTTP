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
#include <condition_variable>
#include "response.hpp"
#include "request.hpp"
#include "ressourcemapper.hpp"
#include "worker.hpp"
#include "queues.hpp"
#include "socket.hpp"
#define HTTP_MSIZE 8192

class HTTPServer{
	private:
        TLSHandlerSock mastersock;
        int handle(Socket *socket);
        std::vector<RequestWorker*> m_request_workers;
        MessageQueue m_MsgQueue;
        ResponseQueue m_RepQueue;
        std::thread m_respond_thread;
        int responder();
        std::condition_variable m_newMsg;
        std::condition_variable  m_newRep;
	public:
		HTTPServer(int port, const char * ressource_path, int workers, 
                std::string certpath, std::string keypath);
        RessourceMapper *mapper;
		int mainloop();
        int test_mainloop(int n_request);
};
#endif
