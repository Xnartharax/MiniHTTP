#include "worker.hpp"
RequestWorker::RequestWorker(RessourceMapper *t_mapper,
            MessageQueue *t_queue, ResponseQueue *t_RepQueue,
            std::condition_variable *t_newMsg, std::condition_variable * t_newRep){
        m_newRep = t_newRep;
        MsgQueue = t_queue;    
        m_newMsg = t_newMsg;
        RepFinished = t_RepQueue;
        m_mapper = t_mapper;
        m_t = std::thread(&RequestWorker::handler, this);
    }
void RequestWorker::handler(){
    message *task = nullptr;
    for(;;){
        std::unique_lock<std::mutex> l(MsgQueue->m_QueueMutex);
        m_newMsg->wait(l, [this](){return !MsgQueue->empty();});
        if(MsgQueue->pop(&task)&& !std::get<1>(*task)->invalid){ 
            Request req = Request(std::get<0>(*task), std::get<1>(*task));
            Response *rep = m_mapper->get(req);
            std::unique_lock<std::mutex> l(RepFinished->m_QueueMutex);
            RepFinished->push(std::make_pair(rep, std::get<1>(*task)));
            m_newRep->notify_one();
            delete task;
        }
        else std::get<1>(*task)->rc--;
    }
}
RequestWorker::~RequestWorker(){
   m_t.join();
}
