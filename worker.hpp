#ifndef WORKER_HPP
#define WORKER_HPP
#include <queue>
#include <thread>
#include <tuple>
#include <map>
#include <mutex>
#include <condition_variable>
#include "queues.hpp"
#include "ressourcemapper.hpp"
class RequestWorker{
    private:
        std::thread m_t;
        void handler();
        RessourceMapper *m_mapper;
        std::condition_variable *m_newMsg;
        std::condition_variable *m_newRep;
    public:
        MessageQueue *MsgQueue; 
        ResponseQueue *RepFinished;
        RequestWorker(RessourceMapper *t_mapper,
         MessageQueue *t_queue, ResponseQueue *t_finshed,
         std::condition_variable *t_newMsg,std::condition_variable * t_newRep);
        ~RequestWorker();
        
};

#endif
