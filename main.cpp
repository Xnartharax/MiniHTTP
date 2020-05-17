#include "server.hpp"
int main(){
    HTTPServer * server = new HTTPServer(8080, "/etc/minihttp/htdocs", 5);
    server->mainloop();
}
