#include "juego.h"
//#include "main.h"
#include <iostream>
#include <string>
#include <limits>

Juego::Juego(){
    tablero = std::vector<std::vector<Jugador>>(6, std::vector<Jugador>(7, NONE));
}

void Juego::mostrarTablero(){
    for (const auto& fila : tablero){
        for (const auto& celda : fila) {
            std::cout << (celda == NONE ? '.' : (char)celda) << " ";
        }
        std::cout << std::endl;
    }
}

bool Juego::hacerMovimiento(int columna, Jugador jugador){
    if (columna < 0 || columna >= 7 || tablero[0][columna] != NONE){
        return false;
    }
    for (int fila = 5; fila >= 0; --fila) {
        if (tablero[fila][columna] == NONE){
            tablero[fila][columna] = jugador;
            return true;
        }
    }
    return false;
}

Jugador Juego::comprobarGanador(){
    for (int fila = 0; fila < 6; ++fila){
        for (int columna = 0; columna < 7; ++columna){
            if (tablero[fila][columna] != NONE && movimientoGanador(fila, columna, tablero[fila][columna])){
                return tablero[fila][columna];
            }
        }
    }
    return NONE;
}

bool Juego::movimientoGanador(int fila, int columna, Jugador jugador) {
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


bool Juego::tableroLleno(){
    for (const auto& fila : tablero){
        for (const auto& celda : fila){
            if (celda == NONE){
                return false;
            }
        }
    }
    return true;
}

std::string Juego::obtenerTablero(){
    std::string tableroStr;
    for (const auto& fila : tablero){
        for (const auto& celda : fila) {
            tableroStr += (celda == NONE ? '.' : (char)celda);
            tableroStr += " ";
        }
        tableroStr += "\n";
    }
    return tableroStr;
}


int main(){
    Juego juego;
    Jugador jugadorActual = JUGADOR1;
    Jugador ganador = NONE;

    while (ganador == NONE && !juego.tableroLleno())
    {
        juego.mostrarTablero();
        std::cout << "Jugador " << (char)jugadorActual << ", elige una columna (0-6): ";
        int columna;
        std::cin >> columna;

        while (std::cin.fail() || columna < 0 || columna > 6 || !juego.hacerMovimiento(columna, jugadorActual)){
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Movimiento invalido. Intentalo de nuevo (0-6): ";
            std::cin >> columna;
        }
        ganador = juego.comprobarGanador();
        jugadorActual = (jugadorActual == JUGADOR1) ? JUGADOR2 : JUGADOR1;
    }

    juego.mostrarTablero();
    if (ganador != NONE) {
        std::cout << "Jugador " << (char)ganador << " gana!" << std::endl;
    } else {
        std::cout << "Empate!" << std::endl;
    }
    return 0;
}

