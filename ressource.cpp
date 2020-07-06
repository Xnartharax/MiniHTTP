#include "ressource.hpp"

using namespace std;
StaticRessource::StaticRessource(const std::string &path){
    cacheable = true;
    ifstream file(path);
    std::string str((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());

    content.assign(str);
    file.close();
}
Response * StaticRessource::buildResp(Request&){
    auto header = std::map<std::string, std::string>();
    header["Content-Type"] = MIME;
    return new OK(content, header);
}
EchoRessource::EchoRessource(){
    cacheable = false;
};
Response * EchoRessource::buildResp(Request& req){
    if (req.method != POST){
       return new BadRequest();
    } 
    else {
        auto header = std::map<std::string, std::string>();
        return new OK(req.body, header);
    }
}
