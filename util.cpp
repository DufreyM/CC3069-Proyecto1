#include "util.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>

int anchoVentana = ANCHO_MINIMO;
int altoVentana = ALTO_MINIMO;

void dibujarCirculoRelleno(SDL_Renderer* renderer, int cx, int cy, int radio) {
    for (int dy = -radio; dy <= radio; ++dy) {
        int dx = static_cast<int>(std::sqrt(static_cast<double>(radio * radio - dy * dy)));
        SDL_RenderDrawLine(renderer, cx - dx, cy + dy, cx + dx, cy + dy);
    }
}

void dibujarCirculoContorno(SDL_Renderer* renderer, int cx, int cy, int radio) {
    int x = radio;
    int y = 0;
    int error = 1 - radio;

    while (x >= y) {
        SDL_RenderDrawPoint(renderer, cx + x, cy + y);
        SDL_RenderDrawPoint(renderer, cx + y, cy + x);
        SDL_RenderDrawPoint(renderer, cx - y, cy + x);
        SDL_RenderDrawPoint(renderer, cx - x, cy + y);
        SDL_RenderDrawPoint(renderer, cx - x, cy - y);
        SDL_RenderDrawPoint(renderer, cx - y, cy - x);
        SDL_RenderDrawPoint(renderer, cx + y, cy - x);
        SDL_RenderDrawPoint(renderer, cx + x, cy - y);

        ++y;
        if (error < 0) {
            error += 2 * y + 1;
        } else {
            --x;
            error += 2 * (y - x) + 1;
        }
    }
}

void obtenerZonaCostosa(float& cx, float& cy, float& radio) {
    cx = anchoVentana / 2.0f;
    cy = altoVentana / 2.0f;
    radio = std::min(anchoVentana, altoVentana) * 0.28f;
}

void dibujarZonaCostosa(SDL_Renderer* renderer, Uint32 tiempoMs) {
    float cx, cy, radioBase;
    obtenerZonaCostosa(cx, cy, radioBase);

    // Pulso lento (0..1) para que el campo respire en vez de quedar estatico.
    float pulso = 0.5f + 0.5f * std::sin(tiempoMs * 0.0015f);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    const int anillos = 4;
    for (int i = 0; i < anillos; ++i) {
        float t = static_cast<float>(i) / (anillos - 1);
        int r = static_cast<int>(radioBase * (0.7f + 0.3f * t) + pulso * 4.0f);
        Uint8 alpha = static_cast<Uint8>(80.0f * (1.0f - t) + 15.0f);
        SDL_SetRenderDrawColor(renderer, 235, 150, 70, alpha);
        dibujarCirculoContorno(renderer, static_cast<int>(cx), static_cast<int>(cy), r);
    }
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
}

bool dentroDeZonaCostosa(float x, float y) {
    float cx, cy, radio;
    obtenerZonaCostosa(cx, cy, radio);
    float dx = x - cx;
    float dy = y - cy;
    return (dx * dx + dy * dy) <= (radio * radio);
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
