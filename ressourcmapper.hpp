#ifndef RESSOURCEMAPPER_HPP
#define RESSOURCEMAPPER_HPP
#include <string>
#include <map>
#include "ressource.hpp"
#include "request.hpp"
class RessourceMapper{
    public:
        Ressource* get(const Request &req);   
        std::string ressource_path;
        void add_mapping(const std::string &identifier, Ressource *rec);
    private:
        std::map<std::string, Ressource*> mapping;
};
#endif
