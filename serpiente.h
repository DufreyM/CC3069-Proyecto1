#pragma once

#include "util.h"

#include <vector>

struct Serpiente {
    std::vector<Segmento> segmentos;
    float velX, velY;
    Uint8 r, g, b;
};

// Crea una serpiente con posicion inicial, velocidad y color pseudoaleatorios.
// Reintenta la posicion inicial (hasta INTENTOS_MAX_SPAWN veces) si cae
// encima de una serpiente ya existente en la lista.
Serpiente crearSerpiente(const std::vector<Serpiente>& serpientes);

// Avanza la cabeza, rebota en los bordes y propaga la posicion al resto de
// la cola. Antes de avanzar, gira levemente la velocidad hacia la posicion
// de comida mas cercana (ver FUERZA_BUSQUEDA_COMIDA) para que la serpiente
// busque comer en vez de vagar al azar.
void actualizarSerpiente(Serpiente& s, const std::vector<Segmento>& posicionesComida);

// Actualiza todas las serpientes. Cada una es independiente de las demas en
// esta etapa (no leen ni modifican el estado de otra, solo la lista de
// comida en modo lectura), por lo que el trabajo se reparte entre hilos con
// OpenMP cuando esta disponible.
void actualizarSerpientes(std::vector<Serpiente>& serpientes, const std::vector<Segmento>& posicionesComida);

void dibujarSerpiente(SDL_Renderer* renderer, const Serpiente& s);
