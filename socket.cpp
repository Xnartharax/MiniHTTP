#include "socket.hpp"

Socket::Socket(int _sock){
    sock = _sock;
    current = buffer;
}

void Socket::read_buffer(){
    read(sock, buffer, SOCKET_BUFFER_SIZE);
    current = buffer;
}
std::string Socket::read_header(){
    read_buffer();
    std::string msg;
    bool cr = false;
    while(!(cr && *current == '\r')){
        if(current-buffer >= SOCKET_BUFFER_SIZE) read_buffer();
        if(*current == '\r'){
            cr = true;
        }
        if (*current != '\n' && *current != '\r') cr = false;
        msg.push_back(*current);
        current++;
    }
    msg.append("\r\n");
    current += 2;
    return msg;
}
char * Socket::read_body(int length){
    char * body = new char[length];
    memset(body, 0, length);
    char * curr = body;
    while (curr + SOCKET_BUFFER_SIZE < body+length){
        memcpy(curr, current, SOCKET_BUFFER_SIZE);
        curr += SOCKET_BUFFER_SIZE;
        if (curr < body+length){
            read_buffer();
        }
    }
    memcpy(curr, current, length-(curr-body)+1);
    return body;
}
void Socket::sock_send(const char * msg, int len){
    send(sock, msg, len, 0);
}
Socket::~Socket(){
    close(sock);
}
TLSSocket::TLSSocket(int sock, SSL_CTX *ctx): Socket(sock){
    ssl = SSL_new(ctx); 
    SSL_set_fd(ssl, sock);
    SSL_accept(ssl);
}
void TLSSocket::read_buffer(){
    SSL_read(ssl, buffer, SOCKET_BUFFER_SIZE);
    current = buffer;

}
void TLSSocket::sock_send(const char * msg, int len){
   SSL_write(ssl, msg, len);
}
TLSSocket::~TLSSocket(){
    SSL_shutdown(ssl);
    SSL_free(ssl);
}
HandlerSock::HandlerSock(int port){
    this->port = port;
    address = new struct sockaddr_in;
    int opt  = 1;
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) 
    { 
        perror("socket failed"); 
        exit(EXIT_FAILURE); 
    } 
       
    // Forcefully attaching socket to the port 8080 
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) 
    { 
        perror("setsockopt"); 
        exit(EXIT_FAILURE); 
    } 
    address->sin_family = AF_INET; 
    address->sin_addr.s_addr = INADDR_ANY; 
    address->sin_port = htons(port); 
    // Forcefully attaching socket to the port 8080 
    if(bind(sock, (struct sockaddr *) address,  
                                 sizeof(*address))<0) 
    { 
        perror("bind failed"); 
        exit(EXIT_FAILURE); 
    } 
    if (listen(sock, 3) < 0) 
    { 
        perror("listen"); 
        exit(EXIT_FAILURE); 
    }
}

Socket * HandlerSock::accept(){
    int socklen = sizeof(address);
    int client = ::accept(sock, (struct sockaddr *) address, (socklen_t *) &socklen);  
    return new Socket(client);
}

TLSHandlerSock::TLSHandlerSock(int port, std::string certpath, std::string keypath): 
        HandlerSock(port)
{
    method = SSLv23_server_method();
    ctx = SSL_CTX_new(method);
    if (!ctx) {
	    perror("Unable to create SSL context");
	    exit(EXIT_FAILURE);
    }
    SSL_CTX_set_ecdh_auto(ctx, 1);

    /* Set the key and cert */
    if (SSL_CTX_use_certificate_file(ctx, certpath.c_str(), SSL_FILETYPE_PEM) <= 0) {
	    exit(EXIT_FAILURE);
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, keypath.c_str(), SSL_FILETYPE_PEM) <= 0 ) {
	    exit(EXIT_FAILURE);
    }

}
TLSSocket * TLSHandlerSock::accept(){
    int socklen = sizeof(address);
    int client = ::accept(sock, (struct sockaddr *) address, (socklen_t *) &socklen);  
    return new TLSSocket(client, ctx);

}
