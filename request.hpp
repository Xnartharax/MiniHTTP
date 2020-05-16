#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <string>
#include <regex>
#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>
enum METHODS {
    GET = 1,
    POST = 2
};
class Request {
    public:
        int method;
        std::string ressource;
        void parse(const std::string &msg);
        Request();
        Request(const std::string &msg);
};
#endif
