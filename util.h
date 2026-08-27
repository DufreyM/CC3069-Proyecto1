#pragma once

#include <SDL2/SDL.h>

// Dimensiones y constantes compartidas por todo el screensaver.
static const int ANCHO_VENTANA = 640;   // minimo exigido (640x480)
static const int ALTO_VENTANA  = 480;   // minimo exigido
static const int RADIO_SEGMENTO = 8;
static const int SEGMENTOS_POR_SERPIENTE = 12;
static const int N_POR_DEFECTO = 6;
static const int N_MIN = 1;
static const int N_MAX = 200; // limite defensivo para no degradar los FPS
static const int RADIO_COMIDA = 6;

struct Segmento {
    float x, y;
};

// Circulo relleno via scanline: por cada fila dy, dibuja una linea horizontal
// del ancho correspondiente a la cuerda del circulo en esa altura.
void dibujarCirculoRelleno(SDL_Renderer* renderer, int cx, int cy, int radio);

// Numero pseudoaleatorio en [minimo, maximo]
float aleatorioEnRango(float minimo, float maximo);

// Lee y valida el parametro N (cantidad de serpientes). Si falta o es invalido,
// avisa por stderr y ajusta a un valor seguro en lugar de fallar.
int leerParametroN(int argc, char* argv[]);
