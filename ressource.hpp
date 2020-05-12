#ifndef RESSOURCE_HPP
#define RESSOURCE_HPP
#include <string>
#include <iostream>
#include <fstream>
#include <streambuf>
#include <sstream>
class Ressource{
    public:
        Ressource(const std::string &path);
        std::string content;
        std::string MIME;
};
#endif
