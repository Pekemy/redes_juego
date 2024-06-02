#ifndef JUEGO_H
#define JUEGO_H

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
    std::string obtenerTablero();

private:
    std::vector<std::vector<Jugador>> tablero;
    bool movimientoGanador(int fila, int columna, Jugador jugador);
};

void juegoPrincipal();

#endif
