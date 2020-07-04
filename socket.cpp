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
    while(true){
        if(current-buffer >= SOCKET_BUFFER_SIZE) read_buffer();
        if(*current == '\r'){
            if(cr) {
                msg += "\r\n";
                current++; 
                break;
            } 
            cr = true;
        }
        if (*current != '\n'&& *current != '\r') cr = false;
        msg += *current;
        current++;
    }
    return msg;
}
char * Socket::read_body(int length){
    char * body = new char[length];
    char * curr = body;
    while (curr < body+length){
        memcpy(curr, buffer, SOCKET_BUFFER_SIZE);
        curr += SOCKET_BUFFER_SIZE;
        read_buffer();
    }
    return body;
}
void Socket::sock_send(const char * msg, int len){
    send(sock, msg, len, 0);
}
Socket::~Socket(){
    close(sock);
}
