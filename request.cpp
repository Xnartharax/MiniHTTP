#include "request.hpp"
using namespace std;
void Request::parse(const string &msg){
    regex ln_re("\\n");
    regex ws_re("\\s+"); 
    vector<std::string> lines{
        sregex_token_iterator(msg.begin(), msg.end(), ln_re, -1), {}
    };
    vector<string> line_zero{
        sregex_token_iterator(lines[0].begin(), lines[0].end(), ws_re, -1), {}
    };
    if(line_zero[0].compare("GET") == 0) method = GET;
    else if(line_zero[0].compare("POST") == 0) method = POST;
    ressource = line_zero[1];
}
Request::Request(const string &msg){
    parse(msg);
}
