#pragma once

#include "serpiente.h"

struct Comida {
    Segmento posicion;
    Uint8 r, g, b;
};

// LÓGICA PARA LA GENERACIÓN DE COMIDA
// Reintenta la posición (hasta INTENTOS_MAX_SPAWN veces) si cae encima de
// una serpiente existente; si no encuentra un hueco libre, se queda con el
// ultimo intento en vez de colgarse.
Comida crearComida(const std::vector<Serpiente>& serpientes);

void dibujarComida(SDL_Renderer* renderer, const Comida& comida);

bool serpienteComeComida(const Serpiente& serpiente, const Comida& comida);

void hacerCrecer(Serpiente& serpiente);
