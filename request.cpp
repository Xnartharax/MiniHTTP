#include "request.hpp"
using namespace std;
typedef boost::tokenizer<boost::char_separator<char>> tokenizer;
void Request::parse(const string &msg){
    boost::char_separator<char> ln_sep("\n");
    boost::char_separator<char> ws_sep(" ");
    tokenizer ln_tok{msg, ln_sep};
    string l0(*ln_tok.begin());
    tokenizer l0_tok{l0, ws_sep};
    auto token = l0_tok.begin();
    if(token->compare("GET") == 0) method = GET;
    else if(token->compare("POST") == 0) method = POST;
    advance(token, 1);
    ressource = *token;
}
Request::Request(const string &msg){
    parse(msg);
}
