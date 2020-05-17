# MiniHTTP
A very simple and incomplete http server build in c++. I build this as a learning project for myself. So far it can only handle 
simple GET requests. The architecture should be expandable and supporting other METHODS and parsing the headers should be relatively easy.  

## Brief explanation
The HTTPServer initializes a bunch of RequestWorker threads and a Responder thread. They communicate via queues. When the HTTPServer
receives a message it pushes the message along with the corresponding socket to the message queue and notifies one Worker thread.
The Worker parses the request and finds the corrresponding ressource with the central RessourceMapper.
The RessourceMapper formulates a Response object. This is done in the RessourceMapper to be able to send a 404.
The finished Response ist pushed onto the ResponseQueue along with the socket and the responder is notified.
The responder sends the Response to the client and closes the socket.

## Installation for systemd
`make install` copies the executable into /etc/minihttp/ and sets up an htdocs directory in that folder. It also copies a
Unit file into /etc/systemd/system named mini-http.service. The service can be started with `sudo systemctl enable mini-http.service --now`
