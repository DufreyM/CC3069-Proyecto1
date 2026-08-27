#pragma once

#include "serpiente.h"

#include <vector>

// Determina si dos segmentos circulares se están tocando.
bool segmentosColisionan(const Segmento& a, const Segmento& b);

// Elimina las serpientes cuya cabeza chocó contra otra serpiente.
// Si dos cabezas chocan, ambas quedan marcadas para desaparecer.
void eliminarSerpientesColisionadas(std::vector<Serpiente>& serpientes);
