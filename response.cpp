#include "response.hpp"
OK::OK(std::string& t_content, std::map<std::string, std::string>& t_header){
    content = t_content;
    header = t_header;
    header["Content-Length"] = std::to_string(content.length());
    status = 200;
    sheader = "HTTP/1.1 200 OK\n";
    stringify_header();
}
void Response::stringify_header(){
    for (auto it : header){
        sheader += it.first;
        sheader += ": ";
        sheader += it.second;
        sheader += "\n";
    }
}
NotFound::NotFound() {
    status = 404;
    sheader = "HTTP/1.1 404 Not Found";
    content = "<HTML><title>Not Found</title><body><h1>404 Requested Ressource could not be found</h1></body></HTML>";
    header["Content-Type"] = "text/html"; 
    header["Content-Length"] = std::to_string(content.length());
    stringify_header();
}

