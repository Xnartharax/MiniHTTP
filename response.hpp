#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <unistd.h>
#include <sys/stat.h>
#include <string>


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
    public:
        OK(std::string&, std::map<std::string, std::string>&);
};

class NotFound: public Response{
    public:
        NotFound();

};
#endif
