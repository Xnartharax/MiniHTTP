#include "server.hpp"
int main(){
    HTTPServer * server = new HTTPServer(8080, "/etc/minihttp/htdocs", 7);
    server->mainloop();
}
