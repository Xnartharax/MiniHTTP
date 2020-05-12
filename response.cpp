#include "response.hpp"
Response::Response(Ressource &arec, Request &areq): rec(arec), req(areq){
    content = rec.content;
    header["Content-Type"] = rec.MIME;
    header["Content-Length"] = std::to_string(rec.content.length());
    status = 200;
    sheader = "HTTP/1.1 200 OK\n";
    for (auto it : header){
        sheader += it.first;
        sheader += ": ";
        sheader += it.second;
        sheader += "\n";
    }
}
