#pragma once

#include "util.h"

#include <vector>

struct Serpiente {
    std::vector<Segmento> segmentos;
    float velX, velY;
    Uint8 r, g, b;
};

// Crea una serpiente con posicion inicial, velocidad y color pseudoaleatorios
Serpiente crearSerpiente();

// Avanza la cabeza, rebota en los bordes y propaga la posicion al resto de la cola
void actualizarSerpiente(Serpiente& s);

void dibujarSerpiente(SDL_Renderer* renderer, const Serpiente& s);
