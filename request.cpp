#include "request.hpp"
#define HTTP_MSIZE 8192

using namespace std;

void Request::parse(const string &msg){
    std::istringstream strm(msg);
    std::string firstline;
    getline(strm, firstline);
    std::string method_;
    std::istringstream l0(firstline);

    getline(l0, method_, ' ');
    getline(l0, ressource, ' ');
    if (method_ == "GET") method = GET;
    if (method_ == "POST") method = POST;
 
    std::string line;
    std::string::size_type index;
    while(getline(strm, line) && line != "\r"){
        index = line.find(':', 0);
        if (index != std::string::npos){
            header.insert(std::make_pair(
                boost::algorithm::trim_copy(line.substr(0, index)),
                boost::algorithm::trim_copy(line.substr(index +1))
            ));
        }
    }
    
}
Request::Request(const string &msg, Socket *sock){
    parse(msg);
    if (method == POST){
        try{
            body.assign(sock->read_body(4));
            //body.assign(sock->read_body(std::stoi(header["Content-Length"]))); 
        }
        catch(std::invalid_argument& e){}
    }
}

Request::Request(const Request& other){
    method = other.method;
    header = other.header;
    body = other.body;
    ressource = other.ressource;
}
