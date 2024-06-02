#ifndef JUEGO_H
#define JUEGO_H

#include <vector>
#include <string>

enum Jugador { NINGUNO = 0, JUGADOR1 = '1', JUGADOR2 = '2' };

class Juego {
public:
    Juego();
    void imprimirTablero();
    bool hacerMovimiento(int columna, Jugador jugador);
    Jugador comprobarGanador();
    bool estaTableroLleno();
    std::string obtenerTableroComoString();
    std::vector<std::pair<int, Jugador>> obtenerMovimientos() const;

private:
    std::vector<std::vector<Jugador>> tablero;
    std::vector<std::pair<int, Jugador>> movimientos;
    bool esMovimientoGanador(int fila, int columna, Jugador jugador);
};

#endif // JUEGO_H
