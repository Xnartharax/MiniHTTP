#include "socket.hpp"

Socket::Socket(int _sock){
    invalid = false;
    timeout = time(nullptr) + 150;
    sock = _sock;
    current = buffer;
}

bool Socket::read_buffer(){
    std::lock_guard<std::mutex> l{mtx};
    int length = read(sock, buffer, SOCKET_BUFFER_SIZE);
    if(length <= 0) {
        invalid = true;
        memset(buffer, 0, SOCKET_BUFFER_SIZE);
        return false;
    }
    current = buffer;
    return true;
}
std::string Socket::read_header(){
    if(!read_buffer()) return "";
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
    std::lock_guard<std::mutex> l{mtx};
    if(0>send(sock, msg, len, 0)) invalid = true;
}
Socket::~Socket(){
    std::lock_guard<std::mutex> lmtx{mtx};
    close(sock);
}
TLSSocket::TLSSocket(int sock, SSL_CTX *ctx): Socket(sock){
    timeout = time(nullptr) + 150;
    ssl = SSL_new(ctx); 
    SSL_set_fd(ssl, sock);
    SSL_accept(ssl);
}
bool TLSSocket::read_buffer(){
    std::lock_guard<std::mutex> l{mtx};
    if(0>=SSL_read(ssl, buffer, SOCKET_BUFFER_SIZE)){ 
        invalid = true;
        memset(buffer, 0, SOCKET_BUFFER_SIZE);
        return false;
    }
    current = buffer;
    return true;
}
void TLSSocket::sock_send(const char * msg, int len){
    std::lock_guard<std::mutex> l{mtx};
    if(0>SSL_write(ssl, msg, len)) invalid = true;
}
TLSSocket::~TLSSocket(){
    std::lock_guard<std::mutex> lmtx{mtx};
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sock);
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
void SocketMultiplexer::addMasterSock(HandlerSock * sock){
    std::lock_guard<std::mutex> lock(mtx);
    pollfd newfd;
    newfd.fd = sock->sock; 
    newfd.events = POLLIN;
    fds.push_back(newfd);
    mastersocks[sock->sock] = sock;
}
void SocketMultiplexer::addSock(Socket * sock){
    std::lock_guard<std::mutex> lock(mtx);
    if (socks[sock->sock] == nullptr){
        pollfd newfd;
        newfd.fd = sock->sock; 
        newfd.events = POLLIN;
        fds.push_back(newfd);
        socks[sock->sock] = sock;
    }
}
Socket * SocketMultiplexer::getNextSock(){
    std::lock_guard<std::mutex> lock(mtx);
    handleTimeouts();  
    int nfds = poll(fds.data(), fds.size(), 500);
    while(nfds <= 0){
        handleTimeouts();  
        nfds = poll(fds.data(), fds.size(), 500);
    }
    for (auto pfd : fds){
        if(pfd.revents & POLLIN){
            auto sock = socks[pfd.fd];
            if (sock != nullptr && !sock->invalid) {
                return sock;
            }
            else{
                auto master = mastersocks[pfd.fd];
                if (master != nullptr)
                    return master->accept();
            }
        
        }
    }
    return nullptr;
}

void SocketMultiplexer::handleTimeouts(){
    std::time_t now = time(nullptr);
    for(auto const & tmp : socks){
        auto val = std::get<1>(tmp);
        auto key = std::get<0>(tmp);
        if (val == nullptr) continue; //look-ups apperently insert a null pointer
        if ((val->timeout < now || val->invalid)&& val->rc == 0){
            auto it = std::find_if(fds.begin(), fds.end(),
                 [key](auto pfd){return pfd.fd==key;});  
            fds.erase(it);
            socks.erase(key);
            break; // break the loop dont modify while iterating 
            // sockets have to be removed one at a time
        }
    } 
}

