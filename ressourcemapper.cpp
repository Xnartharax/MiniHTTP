#include "ressourcemapper.hpp"
using namespace std;
Response* RessourceMapper::get(const Request &req){
    Ressource * rec;
    mtx.lock();
    rec = mapping[req.ressource];
    mtx.unlock();
    if (rec != NULL) return rec->buildResp();
    std::filesystem::path rec_path(ressource_path + req.ressource);
    if(std::filesystem::is_regular_file(rec_path) || filesystem::is_symlink(rec_path)){
        Ressource *rec= new Ressource(ressource_path + req.ressource);
        add_mapping(req.ressource, rec);
        return rec->buildResp();
    }
    else return new NotFound();
}
void RessourceMapper::add_mapping(const string &identifier, Ressource * rec){
    lock_guard<mutex> l(mtx);
    mapping[identifier] = rec;
}
RessourceMapper::RessourceMapper(std::string ressourcepath){
    ressource_path = ressourcepath;
    std::filesystem::path indexpath(ressource_path + "/index.html");
    if(std::filesystem::is_regular_file(indexpath) || filesystem::is_symlink(indexpath)){
        Ressource *index = new Ressource(ressource_path + "/index.html");
        add_mapping("/", index);
        add_mapping("/index.html", index);
    
    }
}
