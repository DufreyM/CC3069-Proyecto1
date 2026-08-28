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

// Zona rectangular donde el movimiento de una serpiente se vuelve ondulante
// (trigonometria) y, a proposito, mas cara de calcular que en el resto del
// canvas. Sirve para tener carga desigual entre serpientes y poder comparar
// schedule(static) contra dynamic/guided en la version paralela.
static const int ZONA_COSTOSA_X0 = ANCHO_VENTANA / 3;
static const int ZONA_COSTOSA_Y0 = ALTO_VENTANA / 3;
static const int ZONA_COSTOSA_X1 = ANCHO_VENTANA * 2 / 3;
static const int ZONA_COSTOSA_Y1 = ALTO_VENTANA * 2 / 3;
static const int ITERACIONES_ZONA_COSTOSA = 500;

struct Segmento {
    float x, y;
};

// Circulo relleno via scanline: por cada fila dy, dibuja una linea horizontal
// del ancho correspondiente a la cuerda del circulo en esa altura.
void dibujarCirculoRelleno(SDL_Renderer* renderer, int cx, int cy, int radio);

// Dibuja el contorno de la zona de costo mayor (referencia visual).
void dibujarZonaCostosa(SDL_Renderer* renderer);

// Indica si un punto cae dentro de la zona de costo mayor.
bool dentroDeZonaCostosa(float x, float y);

// Numero pseudoaleatorio en [minimo, maximo]
float aleatorioEnRango(float minimo, float maximo);

// Lee y valida el parametro N (cantidad de serpientes). Si falta o es invalido,
// avisa por stderr y ajusta a un valor seguro en lugar de fallar.
int leerParametroN(int argc, char* argv[]);
