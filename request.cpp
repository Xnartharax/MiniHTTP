#include "request.hpp"
using namespace std;
void Request::parse(const string &msg){
    typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
    boost::char_separator<char> ln_sep("\n");
    boost::char_separator<char> ws_sep(" ");
    tokenizer ln_tok{msg, ln_sep};
    vector<std::string> lines{
        ln_tok.begin(), ln_tok.end()
    };
    tokenizer l0_tok{lines[0], ws_sep};
    vector<string> line_zero{
        l0_tok.begin(), l0_tok.end()
    };
    if(line_zero[0].compare("GET") == 0) method = GET;
    else if(line_zero[0].compare("POST") == 0) method = POST;
    ressource = line_zero[1];
}
Request::Request(const string &msg){
    parse(msg);
}
