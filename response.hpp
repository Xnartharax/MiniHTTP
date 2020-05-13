#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <unistd.h>
#include <sys/stat.h>
#include <string>

#include "ressource.hpp"
#include "request.hpp"
class Response{
    public:
        std::string sheader;
        std::map<std::string, std::string> header;
        int status;
        std::string content;
        void stringify_header();
        
};
class OK: public Response{
    private:
        Ressource *rec;
        const Request *req;
    public:
        OK(Ressource &arec,const Request &areq);
};

class NotFound: public Response{
    public:
        NotFound();

};
#endif
