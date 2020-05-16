#include "server.hpp"
using namespace std;
HTTPServer::HTTPServer(int port, const char* ressource_path,int workers):
    m_request_workers(),
    m_MsgQueue(),
    m_RepQueue()
    {
    mapper = new RessourceMapper();
    mapper->ressource_path.assign(ressource_path); 
    unique_lock<mutex> lRep(m_RepQueue.m_QueueMutex);
    for(int i= 0; i < workers; i++){
            m_request_workers.push_back(new RequestWorker(mapper, &m_MsgQueue,
                         &m_RepQueue, &m_newMsg, &m_newRep));
    }
    m_respond_thread = thread(&HTTPServer::responder, this);

    int opt  = 1;
    this->address = new struct sockaddr_in;
    if((this->sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if(setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    this->address->sin_family = AF_INET; 
    this->address->sin_addr.s_addr = INADDR_ANY; 
    this->address->sin_port = htons(port); 
    this->addrlen = sizeof(this->address);   
    // Forcefully attaching socket to the port 8080 
    if(bind(this->sockfd, (struct sockaddr *)this->address,  
                                 sizeof(*this->address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(this->sockfd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    }
}
int HTTPServer::mainloop(){
    for(;;){
        int sock = accept(this->sockfd, (struct sockaddr *) this->address, (socklen_t *) &this->addrlen);
        //std::async(&HTTPServer::handle, this, sock);
        handle(sock);
    }
    return 0; 
}
int HTTPServer::test_mainloop(int n_request){
    //same but used for profiling testing because it terminates
    for(int i=0;i < n_request;i++){
        int sock = accept(this->sockfd, (struct sockaddr *) this->address, (socklen_t *) &this->addrlen);
        //std::async(&HTTPServer::handle, this, sock);
        handle(sock);
    }
    return 0; 
}
int HTTPServer::handle(int sock){
    char * buf = new char[HTTP_MSIZE];
    read(sock, buf, HTTP_MSIZE);
    string msg = string(buf);
    unique_lock<mutex> l(m_MsgQueue.m_QueueMutex);
    m_MsgQueue.push(pair<string, int>(msg, sock));
    m_newMsg.notify_one();
    //send(sock, rep->sheader.c_str(), rep->sheader.length(), 0);
    //send(sock, "\r\n", 2, 0);
    //send(sock, rep->content.c_str(), rep->content.length(), 0); 
    //close(sock);
    delete[] buf;
    
    return 0;
}
int HTTPServer::responder(){
    finishedResponse * temp = nullptr; 
    for(;;){
        unique_lock<mutex> l(m_RepQueue.m_QueueMutex);
        m_newRep.wait(l);
        if(m_RepQueue.pop(&temp)){
            auto rep = std::get<0>(*temp); 
            auto sock = std::get<1>(*temp);
            send(sock, rep->sheader.c_str(), rep->sheader.length(), 0);
            send(sock, "\r\n", 2, 0);
            send(sock, rep->content.c_str(), rep->content.length(), 0); 
            close(sock);       
            delete rep;
            delete temp;
        }
    }
}
