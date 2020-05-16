#include "server.hpp"
int main(){
    HTTPServer * server = new HTTPServer(8080, "./test", 5);
    server->test_mainloop();
}
