#pragma once

#include <SDL2/SDL.h>

// Tamano minimo exigido del canvas (640x480). La ventana es redimensionable;
// anchoVentana/altoVentana reflejan el tamano actual y arrancan en estos
// valores.
static const int ANCHO_MINIMO = 640;
static const int ALTO_MINIMO  = 480;
extern int anchoVentana;
extern int altoVentana;

static const int RADIO_SEGMENTO = 8;
static const int SEGMENTOS_POR_SERPIENTE = 12;
static const int N_POR_DEFECTO = 6;
static const int N_MIN = 1;
static const int N_MAX = 200; // limite defensivo para no degradar los FPS
static const int RADIO_COMIDA = 6;

// Cantidad de terminos trigonometricos que se calculan por serpiente por
// frame dentro de la zona de costo mayor: a proposito caro de calcular, para
// tener carga desigual entre serpientes y poder comparar schedule(static)
// contra dynamic/guided en la version paralela.
static const int ITERACIONES_ZONA_COSTOSA = 700;

struct Segmento {
    float x, y;
};

// Circulo relleno via scanline: por cada fila dy, dibuja una linea horizontal
// del ancho correspondiente a la cuerda del circulo en esa altura.
void dibujarCirculoRelleno(SDL_Renderer* renderer, int cx, int cy, int radio);

// Contorno de circulo (algoritmo del punto medio), sin relleno.
void dibujarCirculoContorno(SDL_Renderer* renderer, int cx, int cy, int radio);

// Centro y radio actuales de la zona de costo mayor (circular, centrada en
// el canvas, escala con el tamano de la ventana).
void obtenerZonaCostosa(float& cx, float& cy, float& radio);

// Dibuja la zona de costo mayor como un campo pulsante.
void dibujarZonaCostosa(SDL_Renderer* renderer, Uint32 tiempoMs);

// Indica si un punto cae dentro de la zona de costo mayor.
bool dentroDeZonaCostosa(float x, float y);

// Numero pseudoaleatorio en [minimo, maximo]
float aleatorioEnRango(float minimo, float maximo);

// Lee y valida el parametro N (cantidad de serpientes). Si falta o es invalido,
// avisa por stderr y ajusta a un valor seguro en lugar de fallar.
int leerParametroN(int argc, char* argv[]);
