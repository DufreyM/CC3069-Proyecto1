/*
 * Screensaver "Multisnake" - Version SECUENCIAL
 * Universidad del Valle de Guatemala - Computacion Paralela y Distribuida
 *
 * N (arg[1]) indica la cantidad de serpientes a renderizar.
 * Version paralela con OpenMP pendiente.
 *
 * Compilacion:
 *   g++ Screensaver.cpp -o Screensaver -I<SDL2_include> -L<SDL2_lib> -lmingw32 -lSDL2main -lSDL2 -mwindows
 */

#include <SDL2/SDL.h>
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <vector>

static const int ANCHO_VENTANA = 640;   // minimo exigido (640x480)
static const int ALTO_VENTANA  = 480;   // minimo exigido
static const int RADIO_SEGMENTO = 8;
static const int SEGMENTOS_POR_SERPIENTE = 12;
static const int N_POR_DEFECTO = 6;
static const int N_MIN = 1;
static const int N_MAX = 200; // limite defensivo para no degradar los FPS
static const int RADIO_COMIDA = 6;

struct Segmento {
    float x, y;
};

struct Serpiente {
    std::vector<Segmento> segmentos;
    float velX, velY;
    Uint8 r, g, b;
};

struct Comida {
    Segmento posicion;
    Uint8 r, g, b;
};

// Circulo relleno via scanline: por cada fila dy, dibuja una linea horizontal
// del ancho correspondiente a la cuerda del circulo en esa altura.
void dibujarCirculoRelleno(SDL_Renderer* renderer, int cx, int cy, int radio) {
    for (int dy = -radio; dy <= radio; ++dy) {
        int dx = static_cast<int>(std::sqrt(static_cast<double>(radio * radio - dy * dy)));
        SDL_RenderDrawLine(renderer, cx - dx, cy + dy, cx + dx, cy + dy);
    }
}

// Numero pseudoaleatorio en [minimo, maximo]
float aleatorioEnRango(float minimo, float maximo) {
    return minimo + static_cast<float>(std::rand()) / RAND_MAX * (maximo - minimo);
}

// Crea una serpiente con posicion inicial, velocidad y color pseudoaleatorios
Serpiente crearSerpiente() {
    Serpiente s;
    s.segmentos.resize(SEGMENTOS_POR_SERPIENTE);

    float xInicial = aleatorioEnRango(RADIO_SEGMENTO * 2.0f, ANCHO_VENTANA - RADIO_SEGMENTO * 2.0f);
    float yInicial = aleatorioEnRango(RADIO_SEGMENTO * 2.0f, ALTO_VENTANA - RADIO_SEGMENTO * 2.0f);
    for (int i = 0; i < SEGMENTOS_POR_SERPIENTE; ++i) {
        s.segmentos[i].x = xInicial - i * (RADIO_SEGMENTO * 1.6f);
        s.segmentos[i].y = yInicial;
    }

    s.velX = aleatorioEnRango(1.5f, 3.5f) * (std::rand() % 2 == 0 ? 1.0f : -1.0f);
    s.velY = aleatorioEnRango(1.5f, 3.5f) * (std::rand() % 2 == 0 ? 1.0f : -1.0f);

    s.r = static_cast<Uint8>(aleatorioEnRango(80.0f, 255.0f));
    s.g = static_cast<Uint8>(aleatorioEnRango(80.0f, 255.0f));
    s.b = static_cast<Uint8>(aleatorioEnRango(80.0f, 255.0f));

    return s;
}

// Avanza la cabeza, rebota en los bordes y propaga la posicion al resto de la cola
void actualizarSerpiente(Serpiente& s) {
    // Primero, cada segmento toma la posición anterior del segmento delantero.
    for (int i = static_cast<int>(s.segmentos.size()) - 1; i > 0; --i) {
        s.segmentos[i] = s.segmentos[i - 1];
    }

    Segmento& cabeza = s.segmentos[0];
    cabeza.x += s.velX;
    cabeza.y += s.velY;

    if (cabeza.x - RADIO_SEGMENTO < 0) {
        cabeza.x = RADIO_SEGMENTO;
        s.velX = std::fabs(s.velX);
    } else if (cabeza.x + RADIO_SEGMENTO > ANCHO_VENTANA) {
        cabeza.x = ANCHO_VENTANA - RADIO_SEGMENTO;
        s.velX = -std::fabs(s.velX);
    }

    if (cabeza.y - RADIO_SEGMENTO < 0) {
        cabeza.y = RADIO_SEGMENTO;
        s.velY = std::fabs(s.velY);
    } else if (cabeza.y + RADIO_SEGMENTO > ALTO_VENTANA) {
        cabeza.y = ALTO_VENTANA - RADIO_SEGMENTO;
        s.velY = -std::fabs(s.velY);
    }
}

// Determina si dos segmentos circulares se están tocando.
bool segmentosColisionan(const Segmento& a, const Segmento& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float distanciaCuadrada = dx * dx + dy * dy;
    float distanciaMinima = RADIO_SEGMENTO * 2.0f;

    return distanciaCuadrada <= distanciaMinima * distanciaMinima;
}

// Elimina las serpientes cuya cabeza chocó contra otra serpiente.
// Si dos cabezas chocan, ambas quedan marcadas para desaparecer.
void eliminarSerpientesColisionadas(std::vector<Serpiente>& serpientes) {
    const std::size_t cantidad = serpientes.size();
    std::vector<bool> eliminada(cantidad, false);

    // Se comparan por parejas para no repetir comprobaciones.
    for (std::size_t i = 0; i < cantidad; ++i) {
        for (std::size_t j = i + 1; j < cantidad; ++j) {
            const Segmento& cabezaI = serpientes[i].segmentos[0];
            const Segmento& cabezaJ = serpientes[j].segmentos[0];

            // Cabeza contra cabeza: desaparecen ambas.
            if (segmentosColisionan(cabezaI, cabezaJ)) {
                eliminada[i] = true;
                eliminada[j] = true;
                continue;
            }

            // La cabeza de i contra el cuerpo de j.
            // Se comienza en 1 porque el segmento 0 es la cabeza.
            for (std::size_t k = 1;
                 k < serpientes[j].segmentos.size() && !eliminada[i];
                 ++k) {
                if (segmentosColisionan(cabezaI, serpientes[j].segmentos[k])) {
                    eliminada[i] = true;
                }
            }

            // La cabeza de j contra el cuerpo de i.
            for (std::size_t k = 1;
                 k < serpientes[i].segmentos.size() && !eliminada[j];
                 ++k) {
                if (segmentosColisionan(cabezaJ, serpientes[i].segmentos[k])) {
                    eliminada[j] = true;
                }
            }
        }
    }

    // El borrado se hace después de revisar todas las colisiones.
    std::size_t indice = 0;
    serpientes.erase(
        std::remove_if(
            serpientes.begin(),
            serpientes.end(),
            [&eliminada, &indice](const Serpiente&) {
                return eliminada[indice++];
            }
        ),
        serpientes.end()
    );
}

void dibujarSerpiente(SDL_Renderer* renderer, const Serpiente& s) {
    for (std::size_t i = 0; i < s.segmentos.size(); ++i) {
        if (i == 0) {
            SDL_SetRenderDrawColor(renderer, s.r, s.g, s.b, 255);
        } else {
            // cuerpo un poco mas oscuro que la cabeza para distinguirla
            SDL_SetRenderDrawColor(renderer, s.r * 3 / 4, s.g * 3 / 4, s.b * 3 / 4, 255);
        }
        dibujarCirculoRelleno(renderer, static_cast<int>(s.segmentos[i].x),
                               static_cast<int>(s.segmentos[i].y), RADIO_SEGMENTO);
    }
}

// Lee y valida el parametro N (cantidad de serpientes). Si falta o es invalido,
// avisa por stderr y ajusta a un valor seguro en lugar de fallar.
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

// LÓGICA PARA LA GENERACIÓN DE COMIDA
Comida crearComida() {
    Comida comida;

    comida.posicion.x = aleatorioEnRango(
        RADIO_COMIDA,
        ANCHO_VENTANA - RADIO_COMIDA
    );

    comida.posicion.y = aleatorioEnRango(
        RADIO_COMIDA,
        ALTO_VENTANA - RADIO_COMIDA
    );

    comida.r = 255;
    comida.g = 80;
    comida.b = 80;

    return comida;
}

void dibujarComida(SDL_Renderer* renderer, const Comida& comida) {
    SDL_SetRenderDrawColor(
        renderer,
        comida.r,
        comida.g,
        comida.b,
        255
    );

    dibujarCirculoRelleno(
        renderer,
        static_cast<int>(comida.posicion.x),
        static_cast<int>(comida.posicion.y),
        RADIO_COMIDA
    );
}

bool serpienteComeComida(const Serpiente& serpiente, const Comida& comida) {
    const Segmento& cabeza = serpiente.segmentos[0];

    float dx = cabeza.x - comida.posicion.x;
    float dy = cabeza.y - comida.posicion.y;

    float distanciaCuadrada = dx * dx + dy * dy;
    float distanciaMinima = RADIO_SEGMENTO + RADIO_COMIDA;

    return distanciaCuadrada <= distanciaMinima * distanciaMinima;
}

void hacerCrecer(Serpiente& serpiente) {
    if (!serpiente.segmentos.empty()) {
        // Agrega un segmento en la posición actual de la cola.
        serpiente.segmentos.push_back(serpiente.segmentos.back());
    }
}

int main(int argc, char* argv[]) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    int numSerpientes = leerParametroN(argc, argv);

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::fprintf(stderr, "Error al inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* ventana = SDL_CreateWindow(
        "Multisnake (FPS: --)",
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

    std::vector<Serpiente> serpientes;
    serpientes.reserve(numSerpientes);
    for (int i = 0; i < numSerpientes; ++i) {
        serpientes.push_back(crearSerpiente());
    }

    bool ejecutando = true;
    SDL_Event evento;

    int contadorFrames = 0;
    double fpsActual = 0.0;
    Uint32 ultimoReporteFPS = SDL_GetTicks();

    Comida comida = crearComida();

    while (ejecutando) {
        while (SDL_PollEvent(&evento)) {
            if (evento.type == SDL_QUIT) ejecutando = false;
            if (evento.type == SDL_KEYDOWN && evento.key.keysym.sym == SDLK_ESCAPE) {
                ejecutando = false;
            }
        }

        for (Serpiente& s : serpientes) {
            actualizarSerpiente(s);
        }

        for (Serpiente& s : serpientes) {
            if (serpienteComeComida(s, comida)) {
                hacerCrecer(s);
                comida = crearComida();
                break;
            }
        }

        eliminarSerpientesColisionadas(serpientes);
        if (serpientes.size() <= 1) {
            serpientes.clear();

            for (int i = 0; i < numSerpientes; ++i) {
                serpientes.push_back(crearSerpiente());
            }

            comida = crearComida();

            contadorFrames = 0;
            fpsActual = 0.0;
            ultimoReporteFPS = SDL_GetTicks();
        }

        SDL_SetRenderDrawColor(renderer, 25, 25, 35, 255);
        SDL_RenderClear(renderer);
        dibujarComida(renderer, comida);

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
