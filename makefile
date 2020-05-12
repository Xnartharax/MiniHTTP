CC = g++
STD = c++14
FLAGS = -g -Wall
SRC = ./
LIBS = -lpthread
OBJECTS = response.o ressource.o ressourcemapper.o request.o server.o main.o
server: $(OBJECTS)
	g++ $(OBJECTS) -o HTTP $(FLAGS) --std=$(STD) $(LIBS)
%.o: $(SRC)%.cpp
	$(CC) $^ -c $(FLAGS) --std=$(STD) $(LIBS) -o $@
clean:
	rm $(OBJECTS)
	
