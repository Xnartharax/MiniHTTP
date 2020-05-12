#include "server.hpp"
using namespace std;
HTTPServer::HTTPServer(int port, const char* ressource_path){
    mapper = new RessourceMapper();
    mapper->ressource_path.assign(ressource_path);
    int opt  = 1;
    this->address = new struct sockaddr_in;
    if((this->sockfd = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if(setsockopt(this->sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    this->address->sin_family = AF_INET; 
    this->address->sin_addr.s_addr = INADDR_ANY; 
    this->address->sin_port = htons(port); 
    this->addrlen = sizeof(this->address);   
    // Forcefully attaching socket to the port 8080 
    if(bind(this->sockfd, (struct sockaddr *)this->address,  
                                 sizeof(*this->address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(this->sockfd, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    }
}
int HTTPServer::mainloop(){
    for(;;){
        int sock = accept(this->sockfd, (struct sockaddr *) this->address, (socklen_t *) &this->addrlen);
        std::async(&HTTPServer::handle, this, sock);
    }
    return 0; 
}
int HTTPServer::handle(int sock){
    printf("%d\n", sock);
    char * buf = new char[HTTP_MSIZE];
    read(sock, buf, HTTP_MSIZE);
    std::string str(buf);
    Request req = Request(buf);
    Ressource *rec = mapper->get(req);
    Response rep = Response(*rec, req);
    send(sock, rep.sheader.c_str(), rep.sheader.length(), 0);
    send(sock, "\r\n", 2, 0);
    send(sock, rep.content.c_str(), rep.content.length(), 0); 
    close(sock);
    return 0;
}
