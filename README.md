# redes_juego
# Juego de Conecta 4 en Red

INTEGRANTES: MATIAS ALFREDO PEREIRA GUTIERREZ Y JAVIERA ADRIANA HENRIQUEZ MARTINEZ

Este proyecto implementa un juego de Conecta 4 en red, donde un cliente y un servidor pueden jugar el juego en turnos alternos. El servidor puede jugar automáticamente contra el cliente.

## Estructura del Proyecto

El proyecto consta de los siguientes archivos:

- `Servidor.cpp`: Implementa el servidor del juego.
- `Cliente.cpp`: Implementa el cliente del juego.
- `Juego.cpp`: Implementa la lógica del juego.
- `Juego.h`: Define las estructuras y funciones del juego.
- `Makefile`: Proporciona las reglas para compilar.

## Requisitos

Para compilar y ejecutar este proyecto, necesitarás:

- Un compilador de C++ (g++ recomendado).
- GNU Make.

## Compilación

Para compilar el proyecto, abre una terminal en el directorio del proyecto y ejecuta el siguiente comando:

g++ -o cliente Cliente.cpp
g++ -o servidor Servidor.cpp Juego.cpp -pthread

Ejecución del Servidor
Para ejecutar el servidor, usa el siguiente comando:
./servidor
El servidor comenzará a escuchar conexiones en el puerto 12345 por defecto.

Ejecución del Cliente
Para ejecutar el cliente, usa el siguiente comando:
./cliente
El cliente intentará conectarse al servidor en 127.0.0.1 (localhost) en el puerto 12345.

Uso
Al iniciar el juego, se seleccionará al azar quién comienza.
El cliente y el servidor alternarán turnos para hacer movimientos en el juego.
El cliente ingresará el número de columna (0-6) para hacer un movimiento.
El servidor hará movimientos automáticamente.