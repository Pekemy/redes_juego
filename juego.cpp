#include <iostream>
#include <vector>
#include <string>

enum Jugador { NONE = 0, JUGADOR1 = '1', JUGADOR2 = '2' };

class Juego {
    public:
        Juego();
        void mostrarTablero();
        bool hacerMovimiento(int columna, Jugador jugador);
        Jugador comprobarGanador();
        bool tableroLleno();

    private:
        std::vector<std::vector<Jugador>> tablero;
        bool movimientoGanador(int fila, int columna, Jugador jugador);
    
};

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

bool Juego::movimientoGanador(int fila, int columna, Jugador jugador){ 
    for (int col = std::max(0, columna - 3); col <= std::min(6, columna + 3); ++col){
        if (col + 3 < 7 && tablero[fila][col] == jugador && tablero[fila + 1][columna] == jugador && tablero[fila + 2][columna] == jugador && tablero[fila + 3][columna] == jugador){
            return true;
        }
    }
    for (int fil = std::max(0, fila - 3); fil <= std::min(5, fila + 3); ++fil){
        if (fil + 3 < 6 && tablero[fil][columna] == jugador && tablero[fil + 1][columna] == jugador && tablero[fil + 2][columna] == jugador && tablero[fil + 3][columna] == jugador){
            return true;
        }
    }
    for (int fil = std::max(0, fila -3), col = std::max(0, columna -3); fil <= std::min(5, fila + 3) && col <= std::min(6, columna + 3); ++fil, col++){ // diagonal (\)
        if (fil + 3 < 6 && col + 3 < 7 && tablero[fil][col] == jugador && tablero[fil + 1][col + 1] == jugador && tablero[fil + 2][col + 2] == jugador && tablero[fil + 3][col + 3] == jugador){
            return true;
        }
    }
    for (int fil = std::min(5, fila + 3), col = std::max(0, columna - 3); fil >= std::max(0, fila - 3) && col <= std::min(6, columna + 3); --fil, col++){
        if (fil - 3 >= 0 && col + 3 < 7 && tablero[fil][col] == jugador && tablero[fil -1][col + 1] == jugador && tablero[fil -2][col + 2] == jugador && tablero[fil -3][col + 3] == jugador){
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

        if (!juego.hacerMovimiento(columna, jugadorActual)){
            std:: cout << "Movimiento invalido. Intentalo de  nuevo. " << std::endl;
            continue;
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
