CC = g++
STD = c++17
FLAGS = -O3 -g -Wall 
INCLUDE = /usr/local/etc/boost_1_73_0/
SRC = ./
LIBS = -lpthread -lstdc++fs
OBJECTS = socket.o worker.o response.o ressource.o ressourcemapper.o request.o server.o main.o
server: $(OBJECTS)
	g++ $(OBJECTS) -I $(INCLUDE) -o HTTP $(FLAGS) --std=$(STD) $(LIBS)
%.o: $(SRC)%.cpp
	$(CC) $^ -c $(FLAGS) -I $(INCLUDE) --std=$(STD) $(LIBS) -o $@
clean:
	rm $(OBJECTS)
install:
	cp mini-http.service.template /etc/systemd/system/mini-http.service 
	make server
	mkdir /etc/minihttp/
	mv ./HTTP /etc/minihttp/HTTP
	mkdir /etc/minihttp/htdocs
	
