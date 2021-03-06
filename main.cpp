#include "server.hpp"
#include "ressource.hpp"
#include <csignal>

int main(){
    signal(SIGPIPE, SIG_IGN);
    SSL_load_error_strings();	
    OpenSSL_add_ssl_algorithms();
    HTTPServer * server = new HTTPServer(8080, "/etc/minihttp/htdocs", 7, "/etc/minihttp/cert/cert.pem", "/etc/minihttp/cert/key.pem");
    EchoRessource * echo = new EchoRessource();
    server->mapper->add_mapping("/echo", echo);   
    server->mainloop();
}
