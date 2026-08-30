/*
 * Screensaver "Multisnake" - Version SECUENCIAL
 * Universidad del Valle de Guatemala - Computacion Paralela y Distribuida
 *
 * N (arg[1]) indica la cantidad de serpientes a renderizar.
 * Version paralela con OpenMP pendiente.
 *
 * Compilacion:
 *   g++ *.cpp -o Screensaver -I<SDL2_include> -L<SDL2_lib> -lmingw32 -lSDL2main -lSDL2 -mwindows
 */

#include "util.h"
#include "serpiente.h"
#include "colisiones.h"
#include "comida.h"

#include <algorithm>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include <vector>

#ifdef _OPENMP
#include <omp.h>
#endif

int main(int argc, char* argv[]) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    int numSerpientes = leerParametroN(argc, argv);

#ifdef _OPENMP
    std::fprintf(stderr, "OpenMP activo: %d hilos disponibles\n", omp_get_max_threads());
#else
    std::fprintf(stderr, "OpenMP no activo (compilar con -fopenmp para paralelizar)\n");
#endif

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::fprintf(stderr, "Error al inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* ventana = SDL_CreateWindow(
        "Multisnake (FPS: --)",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        anchoVentana, altoVentana,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    if (!ventana) {
        std::fprintf(stderr, "Error al crear ventana: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_SetWindowMinimumSize(ventana, ANCHO_MINIMO, ALTO_MINIMO);

    SDL_Renderer* renderer = SDL_CreateRenderer(
        ventana, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        std::fprintf(stderr, "Error al crear renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(ventana);
        SDL_Quit();
        return 1;
    }

    std::vector<Serpiente> serpientes;
    serpientes.reserve(numSerpientes);
    for (int i = 0; i < numSerpientes; ++i) {
        serpientes.push_back(crearSerpiente(serpientes));
    }

    bool ejecutando = true;
    SDL_Event evento;

    int contadorFrames = 0;
    double fpsActual = 0.0;
    Uint32 ultimoReporteFPS = SDL_GetTicks();

    std::vector<Comida> comidas;
    comidas.reserve(NUM_COMIDA);
    for (int i = 0; i < NUM_COMIDA; ++i) {
        comidas.push_back(crearComida(serpientes));
    }

    while (ejecutando) {
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) ejecutando = false;
            if (evento.type == SDL_KEYDOWN && evento.key.keysym.sym == SDLK_ESCAPE) {
                ejecutando = false;
            }
            if (evento.type == SDL_WINDOWEVENT && evento.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
                // El area de juego sigue el tamano real de la ventana, nunca
                // por debajo del minimo exigido (640x480).
                anchoVentana = std::max(evento.window.data1, ANCHO_MINIMO);
                altoVentana = std::max(evento.window.data2, ALTO_MINIMO);
            }
        }

        actualizarSerpientes(serpientes);

        for (Serpiente& s : serpientes) {
            procesarComidas(s, comidas, serpientes);
        }

        eliminarSerpientesColisionadas(serpientes);
        if (serpientes.size() <= 1) {
            // Se rellena hasta numSerpientes en vez de vaciar el vector:
            // si queda una sobreviviente, no choco con nadie y no deberia
            // desaparecer solo por ser la ultima.
            while (static_cast<int>(serpientes.size()) < numSerpientes) {
                serpientes.push_back(crearSerpiente(serpientes));
            }

            comidas.clear();
            for (int i = 0; i < NUM_COMIDA; ++i) {
                comidas.push_back(crearComida(serpientes));
            }

            contadorFrames = 0;
            fpsActual = 0.0;
            ultimoReporteFPS = SDL_GetTicks();
        }

        SDL_SetRenderDrawColor(renderer, 25, 25, 35, 255);
        SDL_RenderClear(renderer);
        dibujarZonaCostosa(renderer, SDL_GetTicks());
        for (const Comida& c : comidas) {
            dibujarComida(renderer, c);
        }

        for (const Serpiente& s : serpientes) {
            dibujarSerpiente(renderer, s);
        }

        SDL_RenderPresent(renderer);

        contadorFrames++;
        Uint32 ahora = SDL_GetTicks();
        if (ahora - ultimoReporteFPS >= 500) {
            fpsActual = contadorFrames / ((ahora - ultimoReporteFPS) / 1000.0);
            contadorFrames = 0;
            ultimoReporteFPS = ahora;

            char titulo[128];
            std::snprintf(titulo, sizeof(titulo), "Multisnake N=%d (FPS: %.2f)", numSerpientes, fpsActual);
            SDL_SetWindowTitle(ventana, titulo);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(ventana);
    SDL_Quit();

    return 0;
}
