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

// Avanza la cabeza, rebota en los bordes y propaga la posicion al resto de la cola
void actualizarSerpiente(Serpiente& s);

// Actualiza todas las serpientes. Cada una es independiente de las demas en
// esta etapa (no leen ni modifican el estado de otra), por lo que el trabajo
// se reparte entre hilos con OpenMP cuando esta disponible.
void actualizarSerpientes(std::vector<Serpiente>& serpientes);

void dibujarSerpiente(SDL_Renderer* renderer, const Serpiente& s);
