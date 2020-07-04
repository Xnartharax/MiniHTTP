#ifndef RESSOURCEMAPPER_HPP
#define RESSOURCEMAPPER_HPP
#include <string>
#include <map>
#include <mutex>
#include <filesystem>
#include "ressource.hpp"
#include "request.hpp"
#include "response.hpp"
class RessourceMapper{
    public:
        Response* get(const Request &req);   
        std::string ressource_path;
        void add_mapping(const std::string &identifier, Ressource *rec);
        RessourceMapper(std::string);
    private:
        std::map<std::string, Ressource*> mapping;
        std::mutex mtx;
};
#endif
