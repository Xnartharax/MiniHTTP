#include "ressource.hpp"

using namespace std;
Ressource::Ressource(const std::string &path){
    ifstream file(path);
    std::string str((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());

    content.assign(str);
}
Response * Ressource::buildResp(){
    auto header = std::map<std::string, std::string>();
    header["Content-Type"] = MIME;
    return new OK(content, header);
}

