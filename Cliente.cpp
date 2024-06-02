#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

bool esNumeroValido(const std::string &str) {
    if (str.empty()) return false;
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

int main(int argc, char *argv[]) {
    int puerto = 12345;
    const char* direccionServidor = "127.0.0.1"; // Dirección IP del servidor

    if (argc > 1) {
        direccionServidor = argv[1];
    }
    if (argc > 2) {
        puerto = atoi(argv[2]);
    }

    int clienteSocket;
    struct sockaddr_in servidorDir;
    char buffer[512]; // Aumentar el tamaño del buffer para evitar truncamiento

    std::cout << "Creando socket del cliente..." << std::endl;
    clienteSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clienteSocket < 0) {
        std::cerr << "Error abriendo el socket" << std::endl;
        return 1;
    }

    memset((char *) &servidorDir, 0, sizeof(servidorDir));
    servidorDir.sin_family = AF_INET;
    servidorDir.sin_port = htons(puerto);

    std::cout << "Convirtiendo dirección IP..." << std::endl;
    if (inet_pton(AF_INET, direccionServidor, &servidorDir.sin_addr) <= 0) {
        std::cerr << "Error en inet_pton" << std::endl;
        close(clienteSocket);
        return 1;
    }

    std::cout << "Conectando al servidor..." << std::endl;
    if (connect(clienteSocket, (struct sockaddr *) &servidorDir, sizeof(servidorDir)) < 0) {
        std::cerr << "Error en connect" << std::endl;
        close(clienteSocket);
        return 1;
    }

    std::cout << "Conectado al servidor." << std::endl;

    // Leer el mensaje inicial
    memset(buffer, 0, 512);
    int n = read(clienteSocket, buffer, 511);
    if (n < 0) {
        std::cerr << "Error leyendo del socket" << std::endl;
        close(clienteSocket);
        return 1;
    }
    std::cout << buffer;

    while (true) {
        memset(buffer, 0, 512);
        n = read(clienteSocket, buffer, 511);
        if (n < 0) {
            std::cerr << "Error leyendo del socket" << std::endl;
            break;
        }
        std::cout << buffer;

        if (strstr(buffer, "¡Ganaste!") != NULL || strstr(buffer, "¡El servidor ganó!") != NULL) {
            break;
        }

        // Solo pedir entrada al usuario si es su turno
        if (strstr(buffer, "El servidor hizo su jugada.\n") == NULL) {
            std::string entrada;
            std::cout << "Elige una columna (0-6): ";
            std::cin >> entrada;

            if (!esNumeroValido(entrada)) {
                std::cout << "Entrada no válida. Por favor ingresa un número entre 0 y 6." << std::endl;
                continue;
            }

            int columna = std::stoi(entrada);
            if (columna < 0 || columna > 6) {
                std::cout << "Entrada no válida. Por favor ingresa un número entre 0 y 6." << std::endl;
                continue;
            }

            strcpy(buffer, entrada.c_str());
            n = write(clienteSocket, buffer, strlen(buffer));
            if (n < 0) {
                std::cerr << "Error escribiendo al socket" << std::endl;
                break;
            }
        }
    }

    close(clienteSocket);
    return 0;
}
