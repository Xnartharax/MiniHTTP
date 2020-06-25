#include "ressource.hpp"

using namespace std;
Ressource::Ressource(const std::string &path){
    ifstream file(path);
    std::string str((std::istreambuf_iterator<char>(file)),
                 std::istreambuf_iterator<char>());

    content.assign(str);
}
