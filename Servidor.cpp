#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <thread>
#include <cstdlib>
#include <ctime>
#include "Juego.h"

bool esNumeroValido(const std::string &str) {
    if (str.empty()) return false;
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

void manejarCliente(int clienteSocket) {
    Juego juego;
    char buffer[512];
    int columna;
    Jugador jugadorActual;
    Jugador ganador = NINGUNO;

    // Selección al azar de quién comienza
    srand(time(0));
    jugadorActual = (rand() % 2 == 0) ? JUGADOR1 : JUGADOR2;

    // Enviar mensaje inicial indicando quién comienza
    std::string mensajeInicial = (jugadorActual == JUGADOR1) ? "Tú comienzas.\n" : "El servidor comienza.\n";
    strcpy(buffer, mensajeInicial.c_str());
    int n = write(clienteSocket, buffer, strlen(buffer));
    if (n < 0) {
        std::cerr << "Error escribiendo el mensaje inicial al socket" << std::endl;
        close(clienteSocket);
        return;
    }

    // Enviar el estado inicial del tablero
    std::string tableroInicial = juego.obtenerTableroComoString();
    strcpy(buffer, tableroInicial.c_str());
    n = write(clienteSocket, buffer, strlen(buffer));
    if (n < 0) {
        std::cerr << "Error escribiendo el tablero inicial al socket" << std::endl;
        close(clienteSocket);
        return;
    }

    while (ganador == NINGUNO && !juego.estaTableroLleno()) {
        if (jugadorActual == JUGADOR1) {
            // Movimiento del cliente
            bool movimientoValido = false;
            while (!movimientoValido) {
                memset(buffer, 0, 512);
                n = read(clienteSocket, buffer, 511);
                if (n < 0) {
                    std::cerr << "Error leyendo del socket" << std::endl;
                    break;
                }
                std::string entrada(buffer);
                if (!esNumeroValido(entrada) || (columna = std::stoi(entrada)) < 0 || columna > 6) {
                    std::string mensaje = "Movimiento inválido. Inténtalo de nuevo.\n";
                    strcpy(buffer, mensaje.c_str());
                    n = write(clienteSocket, buffer, strlen(buffer));
                    if (n < 0) {
                        std::cerr << "Error escribiendo al socket" << std::endl;
                        break;
                    }
                    continue;
                }
                if (!juego.hacerMovimiento(columna, jugadorActual)) {
                    std::string mensaje = "Movimiento inválido. Inténtalo de nuevo.\n";
                    strcpy(buffer, mensaje.c_str());
                    n = write(clienteSocket, buffer, strlen(buffer));
                    if (n < 0) {
                        std::cerr << "Error escribiendo al socket" << std::endl;
                        break;
                    }
                    continue;
                }
                movimientoValido = true;
            }
        } else {
            // Movimiento del servidor (jugador automático)
            do {
                columna = rand() % 7;
            } while (!juego.hacerMovimiento(columna, jugadorActual));
        }

        ganador = juego.comprobarGanador();
        std::string tablero = juego.obtenerTableroComoString();

        if (ganador != NINGUNO) {
            std::string mensajeGanador = (ganador == JUGADOR1) ? "¡Ganaste!\n" : "¡El servidor ganó!\n";
            tablero += mensajeGanador;
        }

        strcpy(buffer, tablero.c_str());
        n = write(clienteSocket, buffer, strlen(buffer));
        if (n < 0) {
            std::cerr << "Error escribiendo al socket" << std::endl;
            break;
        }

        if (ganador != NINGUNO) {
            break;
        }

        jugadorActual = (jugadorActual == JUGADOR1) ? JUGADOR2 : JUGADOR1;
    }
    close(clienteSocket);
}

int main(int argc, char *argv[]) {
    int puerto = 12345;
    if (argc > 1) {
        puerto = atoi(argv[1]);
    }

    int servidorSocket, clienteSocket;
    struct sockaddr_in servidorDir, clienteDir;
    socklen_t clienteLen;

    servidorSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (servidorSocket < 0) {
        std::cerr << "Error abriendo el socket" << std::endl;
        return 1;
    }

    memset((char *) &servidorDir, 0, sizeof(servidorDir));
    servidorDir.sin_family = AF_INET;
    servidorDir.sin_addr.s_addr = INADDR_ANY;
    servidorDir.sin_port = htons(puerto);

    if (bind(servidorSocket, (struct sockaddr *) &servidorDir, sizeof(servidorDir)) < 0) {
        std::cerr << "Error en el binding" << std::endl;
        close(servidorSocket);
        return 1;
    }

    if (listen(servidorSocket, 5) < 0) {
        std::cerr << "Error en listen" << std::endl;
        close(servidorSocket);
        return 1;
    }
    std::cout << "Servidor esperando por conexiones en el puerto " << puerto << "..." << std::endl;

    clienteLen = sizeof(clienteDir);

    while (true) {
        clienteSocket = accept(servidorSocket, (struct sockaddr *) &clienteDir, &clienteLen);
        if (clienteSocket < 0) {
            std::cerr << "Error en el accept" << std::endl;
            close(servidorSocket);
            return 1;
        }
        std::cout << "Cliente conectado." << std::endl;
        std::thread(manejarCliente, clienteSocket).detach();
    }

    close(servidorSocket);
    return 0;
}
