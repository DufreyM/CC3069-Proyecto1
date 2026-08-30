#pragma once

#include "serpiente.h"

#include <vector>

// Determina si dos segmentos circulares se están tocando.
bool segmentosColisionan(const Segmento& a, const Segmento& b);

// Indica si un circulo de radio dado en (x, y) se solapa con algun segmento
// de alguna serpiente de la lista. Se usa para el spawn defensivo de comida
// y de serpientes nuevas, para que no aparezcan encima de una ya existente.
bool posicionOcupada(float x, float y, float radio, const std::vector<Serpiente>& serpientes);

// Elimina las serpientes cuya cabeza chocó contra otra serpiente.
// Si dos cabezas chocan, ambas quedan marcadas para desaparecer.
void eliminarSerpientesColisionadas(std::vector<Serpiente>& serpientes);
