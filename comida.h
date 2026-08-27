#pragma once

#include "serpiente.h"

struct Comida {
    Segmento posicion;
    Uint8 r, g, b;
};

// LÓGICA PARA LA GENERACIÓN DE COMIDA
Comida crearComida();

void dibujarComida(SDL_Renderer* renderer, const Comida& comida);

bool serpienteComeComida(const Serpiente& serpiente, const Comida& comida);

void hacerCrecer(Serpiente& serpiente);
