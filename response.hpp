#ifndef RESPONSE_HPP
#define RESPONSE_HPP
#include <string>
#include "ressource.hpp"
#include "request.hpp"
class Response{
    private:
        Ressource rec;
        Request req;
    public:
        std::string sheader;
        std::map<std::string, std::string> header;
        std::string content;
        int status;
        Response(Ressource &arec, Request &areq);
};
#endif
