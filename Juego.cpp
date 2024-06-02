#include "Juego.h"
#include <iostream>
#include <algorithm>

Juego::Juego() {
    tablero = std::vector<std::vector<Jugador>>(6, std::vector<Jugador>(7, NINGUNO));
}

void Juego::imprimirTablero() {
    for (const auto& fila : tablero) {
        for (const auto& celda : fila) {
            std::cout << (celda == NINGUNO ? '.' : (char)celda) << " ";
        }
        std::cout << std::endl;
    }
}

bool Juego::hacerMovimiento(int columna, Jugador jugador) {
    if (columna < 0 || columna >= 7 || tablero[0][columna] != NINGUNO) {
        return false;
    }
    for (int fila = 5; fila >= 0; --fila) {
        if (tablero[fila][columna] == NINGUNO) {
            tablero[fila][columna] = jugador;
            movimientos.push_back({columna, jugador}); // Registrar movimiento
            return true;
        }
    }
    return false;
}

Jugador Juego::comprobarGanador() {
    for (int fila = 0; fila < 6; ++fila) {
        for (int columna = 0; columna < 7; ++columna) {
            if (tablero[fila][columna] != NINGUNO && esMovimientoGanador(fila, columna, tablero[fila][columna])) {
                return tablero[fila][columna];
            }
        }
    }
    return NINGUNO;
}

bool Juego::esMovimientoGanador(int fila, int columna, Jugador jugador) {
    // Horizontal
    for (int col = std::max(0, columna - 3); col <= std::min(6, columna + 3) && col + 3 < 7; ++col) {
        if (tablero[fila][col] == jugador && tablero[fila][col + 1] == jugador &&
            tablero[fila][col + 2] == jugador && tablero[fila][col + 3] == jugador) {
            return true;
        }
    }
    // Vertical
    for (int fil = std::max(0, fila - 3); fil <= std::min(5, fila + 3) && fil + 3 < 6; ++fil) {
        if (tablero[fil][columna] == jugador && tablero[fil + 1][columna] == jugador &&
            tablero[fil + 2][columna] == jugador && tablero[fil + 3][columna] == jugador) {
            return true;
        }
    }
    // Diagonal (\)
    for (int fil = std::max(0, fila - 3), col = std::max(0, columna - 3); fil <= std::min(5, fila + 3) && col <= std::min(6, columna + 3) && fil + 3 < 6 && col + 3 < 7; ++fil, ++col) {
        if (tablero[fil][col] == jugador && tablero[fil + 1][col + 1] == jugador &&
            tablero[fil + 2][col + 2] == jugador && tablero[fil + 3][col + 3] == jugador) {
            return true;
        }
    }
    // Diagonal (/)
    for (int fil = std::min(5, fila + 3), col = std::max(0, columna - 3); fil >= std::max(0, fila - 3) && col <= std::min(6, columna + 3) && fil - 3 >= 0 && col + 3 < 7; --fil, ++col) {
        if (tablero[fil][col] == jugador && tablero[fil - 1][col + 1] == jugador &&
            tablero[fil - 2][col + 2] == jugador && tablero[fil - 3][col + 3] == jugador) {
            return true;
        }
    }
    return false;
}

bool Juego::estaTableroLleno() {
    for (const auto& fila : tablero) {
        for (const auto& celda : fila) {
            if (celda == NINGUNO) {
                return false;
            }
        }
    }
    return true;
}

std::string Juego::obtenerTableroComoString() {
    std::string tableroString;
    for (const auto& fila : tablero) {
        for (const auto& celda : fila) {
            tableroString += (celda == NINGUNO ? '.' : (char)celda);
            tableroString += ' ';
        }
        tableroString += '\n';
    }
    return tableroString;
}

std::vector<std::pair<int, Jugador>> Juego::obtenerMovimientos() const {
    return movimientos;
}
