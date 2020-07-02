#include "request.hpp"
using namespace std;
typedef boost::tokenizer<boost::char_separator<char>> tokenizer;

void Request::parse(const string &msg){
    std::istringstream strm(msg);
    
}
Request::Request(const string &msg, int sock){
    parse(msg);
    if (method == POST){
        
    }
}
