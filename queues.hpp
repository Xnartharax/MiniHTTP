#ifndef QUEUES_HPP
#define QUEUES_HPP
#include <queue>
#include <mutex>
#include "response.hpp"
//a thread-safe queue for messages to handle
template <typename T>
class ThreadsafeQueue{
    private:
        std::queue<T> m_Queue;
    public:
        std::mutex m_QueueMutex;
        void push(T elem){
            m_Queue.push(elem);
        }
        bool pop(T **place){
            if(m_Queue.empty()) return false;
            if(m_Queue.empty()) return false;
            T *ret = new T(m_Queue.front());
            m_Queue.pop();
            *place = ret;
            return true;
        }
        bool empty(){
            return m_Queue.empty();
        }
        ThreadsafeQueue(): m_Queue(), m_QueueMutex(){
        }
        
};
typedef std::pair<std::string, int> message;//first message string then socket
typedef std::pair<Response *, int> finishedResponse;
typedef ThreadsafeQueue<message> MessageQueue;
typedef ThreadsafeQueue<finishedResponse> ResponseQueue;
#endif
