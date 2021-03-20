#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <string>
#include <regex>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include "socket.hpp"

enum METHODS {
    GET = 1,
    POST = 2
};

class Request {
    public:
        int method;
        std::string protocol;
        std::string ressource;
        std::map<std::string, std::string> header;
        std::string body;
        void parse(const std::string &msg);
        Request(const Request&);
        Request();
        Request(const std::string &msg, Socket * sock);
};
inline bool operator<(const Request& lreq, const Request& rreq){
    return lreq.ressource < rreq.ressource;
}
#endif
