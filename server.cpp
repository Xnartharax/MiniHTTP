#include "server.hpp"
using namespace std;

HTTPServer::HTTPServer(int port, const char* ressource_path, int workers,
        std::string certpath, std::string keypath):
    mastersock(port, certpath, keypath),
    m_request_workers(),
    m_MsgQueue(),
    m_RepQueue()
    {
    mapper = new RessourceMapper(ressource_path);
    unique_lock<mutex> lRep(m_RepQueue.m_QueueMutex);
    for(int i= 0; i < workers; i++){
            m_request_workers.push_back(new RequestWorker(mapper, &m_MsgQueue,
                         &m_RepQueue, &m_newMsg, &m_newRep));
    }
    m_respond_thread = thread(&HTTPServer::responder, this);

    
}
int HTTPServer::mainloop(){
    for(;;){
        auto sock = mastersock.accept();
        handle(sock);
    }
    return 0; 
}
int HTTPServer::test_mainloop(int n_request){
    //same but used for profiling testing because it terminates
    for(int i=0;i < n_request;i++){
        auto sock = mastersock.accept();
        handle(sock);
    }
    return 0; 
}

int HTTPServer::handle(Socket * socket){
    unique_lock<mutex> l(m_MsgQueue.m_QueueMutex);
    m_MsgQueue.push(pair<string, Socket*>(socket->read_header(), socket));
    m_newMsg.notify_one();
    return 0;
}
int HTTPServer::responder(){
    finishedResponse * temp = nullptr; 
    for(;;){
        unique_lock<mutex> l(m_RepQueue.m_QueueMutex);
        m_newRep.wait(l, [this](){return !m_RepQueue.empty();});
        if(m_RepQueue.pop(&temp)){
            auto rep = std::get<0>(*temp); 
            auto sock = std::get<1>(*temp);
            sock->sock_send(rep->sheader.c_str(), rep->sheader.length());
            sock->sock_send("\r\n", 2);
            sock->sock_send(rep->content.c_str(), rep->content.length()); 
            delete sock;
            delete rep;
            delete temp;
        }
    }
}
