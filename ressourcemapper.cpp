#include "ressourcemapper.hpp"
using namespace std;
Response* RessourceMapper::get(const Request &req){
    Ressource *rec = mapping[req.ressource];
    struct stat buffer;
    if (rec != NULL) return new OK(*rec, req);
    else if (stat((ressource_path + req.ressource).c_str(), &buffer) == 0){
        Ressource rec(ressource_path + req.ressource);
        return new OK(rec, req);
    }
    else return new NotFound();
}
void RessourceMapper::add_mapping(const string &identifier, Ressource * rec){
    mapping[identifier] = rec;
}
