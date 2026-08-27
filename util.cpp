#include "util.h"

#include <cmath>
#include <cstdio>
#include <cstdlib>

void dibujarCirculoRelleno(SDL_Renderer* renderer, int cx, int cy, int radio) {
    for (int dy = -radio; dy <= radio; ++dy) {
        int dx = static_cast<int>(std::sqrt(static_cast<double>(radio * radio - dy * dy)));
        SDL_RenderDrawLine(renderer, cx - dx, cy + dy, cx + dx, cy + dy);
    }
}

float aleatorioEnRango(float minimo, float maximo) {
    return minimo + static_cast<float>(std::rand()) / RAND_MAX * (maximo - minimo);
}

int leerParametroN(int argc, char* argv[]) {
    if (argc <= 1) {
        return N_POR_DEFECTO;
    }

    char* fin = nullptr;
    long valor = std::strtol(argv[1], &fin, 10);
    if (fin == argv[1] || *fin != '\0') {
        std::fprintf(stderr, "Aviso: N='%s' no es un numero valido, usando valor por defecto (%d)\n",
                     argv[1], N_POR_DEFECTO);
        return N_POR_DEFECTO;
    }
    if (valor < N_MIN) {
        std::fprintf(stderr, "Aviso: N=%ld fuera de rango, usando minimo (%d)\n", valor, N_MIN);
        return N_MIN;
    }
    if (valor > N_MAX) {
        std::fprintf(stderr, "Aviso: N=%ld excede el maximo soportado, usando maximo (%d)\n", valor, N_MAX);
        return N_MAX;
    }
    return static_cast<int>(valor);
}
