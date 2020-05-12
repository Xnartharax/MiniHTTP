#include "ressource.hpp"

using namespace std;
Ressource::Ressource(const std::string &path){
    fstream file(path);
    stringstream temp;
    temp << file.rdbuf();
    content.assign(temp.str());
    MIME = "text/html";
}
