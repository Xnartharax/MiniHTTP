#include "ressourcemapper.hpp"
using namespace std;
Response* RessourceMapper::get(Request &req){
    Response * rep;
    cmtx.lock();
    rep = cache[req];
    cmtx.unlock();
    if(rep) return rep;
    Ressource * rec;
    mtx.lock();
    rec = rec_mapping[req.ressource];
    mtx.unlock();
    if (rec != NULL) return rec->buildResp(req);
    std::filesystem::path rec_path(ressource_path + req.ressource);
    if(std::filesystem::is_regular_file(rec_path) || filesystem::is_symlink(rec_path)){
        Ressource *rec= new StaticRessource(ressource_path + req.ressource);
        add_mapping(req.ressource, rec);
        if (rec->cacheable){
            cache.insert(std::make_pair(Request(req), rec->buildResp(req)));
            return cache[req];
        }
        return rec->buildResp(req);
        
    }
    else return new NotFound();
}
void RessourceMapper::add_mapping(const string &identifier, Ressource * rec){
    lock_guard<mutex> l(mtx);
    rec_mapping[identifier] = rec;
}
RessourceMapper::RessourceMapper(std::string ressourcepath){
    ressource_path = ressourcepath;
    std::filesystem::path indexpath(ressource_path + "/index.html");
    if(std::filesystem::is_regular_file(indexpath) || filesystem::is_symlink(indexpath)){
        StaticRessource *index = new StaticRessource(ressource_path + "/index.html");
        add_mapping("/", index);
        add_mapping("/index.html", index);
    
    }
}
