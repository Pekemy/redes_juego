# Variables
CXX = g++
CXXFLAGS = -Wall -std=c++11
TARGETS = servidor cliente

# Archivos fuente y objetos
SERVER_SRC = Servidor.cpp Juego.cpp
CLIENT_SRC = Cliente.cpp Juego.cpp
SERVER_OBJ = $(SERVER_SRC:.cpp=.o)
CLIENT_OBJ = $(CLIENT_SRC:.cpp=.o)

# Reglas para compilar los binarios
all: $(TARGETS)

servidor: $(SERVER_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

cliente: $(CLIENT_OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regla para compilar archivos fuente en objetos
%.o: %.cpp Juego.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para limpiar los archivos compilados
clean:
	rm -f $(TARGETS) $(SERVER_OBJ) $(CLIENT_OBJ)

# Regla para ejecutar el servidor
run-servidor:
	./servidor

# Regla para ejecutar el cliente
run-cliente:
	./cliente 127.0.0.1 12345

.PHONY: all clean run-servidor run-cliente
