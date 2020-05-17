#include "ressourcemapper.hpp"
using namespace std;
Response* RessourceMapper::get(const Request &req){
    Ressource * rec;
    mtx.lock();
    rec = mapping[req.ressource];
    mtx.unlock();
    if (rec != NULL) return new OK(*rec, req);
    std::filesystem::path rec_path(ressource_path + req.ressource);
    if(std::filesystem::is_regular_file(rec_path) || filesystem::is_symlink(rec_path)){
        Ressource *rec= new Ressource(ressource_path + req.ressource);
        add_mapping(req.ressource, rec);
        return new OK(*rec, req);
    }
    else return new NotFound();
}
void RessourceMapper::add_mapping(const string &identifier, Ressource * rec){
    lock_guard<mutex> l(mtx);
    mapping[identifier] = rec;
}
