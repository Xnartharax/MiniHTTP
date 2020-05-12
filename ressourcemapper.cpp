#include "ressourcmapper.hpp"
using namespace std;
Ressource* RessourceMapper::get(const Request &req){
    Ressource *rec = mapping[req.ressource];
    if (rec != NULL) return rec;
    else return new Ressource(ressource_path + req.ressource); 
}
void RessourceMapper::add_mapping(const string &identifier, Ressource * rec){
    mapping[identifier] = rec;
}
