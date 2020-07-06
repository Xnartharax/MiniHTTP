#ifndef RESSOURCE_HPP
#define RESSOURCE_HPP
#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <sstream>
#include "response.hpp"
#include "request.hpp"
class Ressource {
   public: 
        bool cacheable;
        virtual Response * buildResp( Request& req) = 0;
};
class StaticRessource: public Ressource{
    public:
        StaticRessource(const std::string &path);
        std::string content;
        std::string MIME;
        Response * buildResp(Request& req);
};
class EchoRessource: public Ressource{
    public:
        EchoRessource();
        Response * buildResp(Request& req);
};

#endif
