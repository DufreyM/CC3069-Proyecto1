/*
 * Prueba de concepto - Screensaver "Serpientes IA"
 * Universidad del Valle de Guatemala - Computacion Paralela y Distribuida
 *
 * Version SECUENCIAL y minimal para validar SDL2 antes de la version
 * completa (N serpientes, comida, colisiones, IA, OpenMP).
 *
 * Compilacion:
 *   g++ Screensaver.cpp -o Screensaver -I<SDL2_include> -L<SDL2_lib> -lmingw32 -lSDL2main -lSDL2 -mwindows
 */

#include <SDL2/SDL.h>
#include <cmath>
#include <cstdio>
#include <vector>

static const int ANCHO_VENTANA = 640;   // minimo exigido (640x480)
static const int ALTO_VENTANA  = 480;   // minimo exigido
static const int RADIO_SEGMENTO = 8;
static const int NUM_SEGMENTOS  = 12;

struct Segmento {
    float x, y;
};

// Circulo relleno via scanline: por cada fila dy, dibuja una linea horizontal
// del ancho correspondiente a la cuerda del circulo en esa altura.
void dibujarCirculoRelleno(SDL_Renderer* renderer, int cx, int cy, int radio) {
    for (int dy = -radio; dy <= radio; ++dy) {
        int dx = static_cast<int>(std::sqrt(static_cast<double>(radio * radio - dy * dy)));
        SDL_RenderDrawLine(renderer, cx - dx, cy + dy, cx + dx, cy + dy);
    }
}

int main(int argc, char* argv[]) {
    int numSegmentos = NUM_SEGMENTOS;
    if (argc > 1) {
        int valor = std::atoi(argv[1]);
        if (valor > 0) {
            numSegmentos = valor;
        } else {
            std::fprintf(stderr, "Aviso: parametro N invalido, usando valor por defecto (%d)\n", NUM_SEGMENTOS);
        }
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::fprintf(stderr, "Error al inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* ventana = SDL_CreateWindow(
        "PoC Screensaver - Serpientes IA (FPS: --)",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        ANCHO_VENTANA, ALTO_VENTANA,
        SDL_WINDOW_SHOWN
    );
    if (!ventana) {
        std::fprintf(stderr, "Error al crear ventana: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        ventana, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        std::fprintf(stderr, "Error al crear renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(ventana);
        SDL_Quit();
        return 1;
    }

    std::vector<Segmento> serpiente(numSegmentos);
    for (int i = 0; i < numSegmentos; ++i) {
        serpiente[i].x = 100.0f - i * (RADIO_SEGMENTO * 1.6f);
        serpiente[i].y = ALTO_VENTANA / 2.0f;
    }

    float velX = 2.5f, velY = 1.8f;

    bool ejecutando = true;
    SDL_Event evento;

    int contadorFrames = 0;
    double fpsActual = 0.0;
    Uint32 ultimoReporteFPS = SDL_GetTicks();

    while (ejecutando) {
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) ejecutando = false;
            if (evento.type == SDL_KEYDOWN && evento.key.keysym.sym == SDLK_ESCAPE) {
                ejecutando = false;
            }
        }

        Segmento& cabeza = serpiente[0];
        cabeza.x += velX;
        cabeza.y += velY;
        if (cabeza.x - RADIO_SEGMENTO < 0 || cabeza.x + RADIO_SEGMENTO > ANCHO_VENTANA) velX = -velX;
        if (cabeza.y - RADIO_SEGMENTO < 0 || cabeza.y + RADIO_SEGMENTO > ALTO_VENTANA) velY = -velY;

        // Cada segmento toma la posicion que tenia el anterior (efecto de cola)
        for (int i = numSegmentos - 1; i > 0; --i) {
            serpiente[i].x = serpiente[i - 1].x;
            serpiente[i].y = serpiente[i - 1].y;
        }

        SDL_SetRenderDrawColor(renderer, 25, 25, 35, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < numSegmentos; ++i) {
            if (i == 0) {
                SDL_SetRenderDrawColor(renderer, 80, 220, 120, 255);
            } else {
                SDL_SetRenderDrawColor(renderer, 40, 160, 90, 255);
            }
            dibujarCirculoRelleno(renderer, static_cast<int>(serpiente[i].x),
                                   static_cast<int>(serpiente[i].y), RADIO_SEGMENTO);
        }

        SDL_RenderPresent(renderer);

        contadorFrames++;
        Uint32 ahora = SDL_GetTicks();
        if (ahora - ultimoReporteFPS >= 500) {
            fpsActual = contadorFrames / ((ahora - ultimoReporteFPS) / 1000.0);
            contadorFrames = 0;
            ultimoReporteFPS = ahora;

            char titulo[128];
            std::snprintf(titulo, sizeof(titulo), "PoC Screensaver - Serpientes IA (FPS: %.2f)", fpsActual);
            SDL_SetWindowTitle(ventana, titulo);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    SDL_Quit();

    return 0;
}
