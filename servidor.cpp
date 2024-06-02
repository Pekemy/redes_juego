#include "juego.h"
//#include "main.h"
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>


void manejarConexion(int cliente1, int cliente2){
    Juego juego;
    Jugador jugadorActual = JUGADOR1;
    Jugador ganador = NONE;

    int clienteActual = cliente1;
    int otroCliente = cliente2;

    while (ganador == NONE && !juego.tableroLleno()) {
        std::string tableroStr = juego.obtenerTablero();
        send(clienteActual, tableroStr.c_str(), tableroStr.size(), 0);

        char buffer[256];
        int columna;
        bool movimientoValido = false;

        while (!movimientoValido) {
            int n = recv(clienteActual, buffer, 255, 0);
            if (n < 0) perror("ERROR al leer el socket");
            buffer[n] = '\0';
            columna = atoi(buffer);

            if (columna >= 0 && columna < 7 && juego.hacerMovimiento(columna, jugadorActual)) {
                movimientoValido = true;
            } else {
                const char* mensaje = "Movimiento invalido. Intentalo de nuevo (0-6): ";
                send(clienteActual, mensaje, strlen(mensaje), 0);
            }
        }

        ganador = juego.comprobarGanador();
        std::swap(clienteActual, otroCliente);
        jugadorActual = (jugadorActual == JUGADOR1) ? JUGADOR2 : JUGADOR1;
    }

    std::string tableroStr = juego.obtenerTablero();
    send(cliente1, tableroStr.c_str(), tableroStr.size(), 0);
    send(cliente2, tableroStr.c_str(), tableroStr.size(), 0);

    if (ganador != NONE) {
        std::string mensaje = "Jugador ";
        mensaje += (char)ganador;
        mensaje += " gana!\n";
        send(cliente1, mensaje.c_str(), mensaje.size(), 0);
        send(cliente2, mensaje.c_str(), mensaje.size(), 0);
    } else {
        const char* mensaje = "Empate!\n";
        send(cliente1, mensaje, strlen(mensaje), 0);
        send(cliente2, mensaje, strlen(mensaje), 0);
    }

    close(cliente1);
    close(cliente2);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0] << " <puerto>\n";
        return 1;
    }

    int puerto = atoi(argv[1]);
    int sockfd, newsockfd1, newsockfd2;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("ERROR al abrir socket");
        return 1;
    }

    memset((char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(puerto);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("ERROR al hacer bind");
        return 1;
    }

    listen(sockfd, 5);
    std::cout << "Esperando conexiones..." << std::endl;

    clilen = sizeof(cli_addr);
    while (true) {
        newsockfd1 = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd1 < 0) {
            perror("ERROR al aceptar conexión del jugador 1");
            continue;
        }
        std::cout << "Jugador 1 conectado." << std::endl;

        newsockfd2 = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd2 < 0) {
            perror("ERROR al aceptar conexión del jugador 2");
            close(newsockfd1);
            continue;
        }
        std::cout << "Jugador 2 conectado." << std::endl;

        std::thread(manejarConexion, newsockfd1, newsockfd2).detach();
    }

    close(sockfd);
    return 0;
}

