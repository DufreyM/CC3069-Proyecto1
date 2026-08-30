#include "comida.h"
#include "colisiones.h"

Comida crearComida(const std::vector<Serpiente>& serpientes) {
    Comida comida;

    for (int intento = 0; intento < INTENTOS_MAX_SPAWN; ++intento) {
        comida.posicion.x = aleatorioEnRango(
            RADIO_COMIDA,
            anchoVentana - RADIO_COMIDA
        );

        comida.posicion.y = aleatorioEnRango(
            RADIO_COMIDA,
            altoVentana - RADIO_COMIDA
        );

        if (!posicionOcupada(comida.posicion.x, comida.posicion.y, RADIO_COMIDA, serpientes)) {
            break;
        }
    }

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

void procesarComidas(Serpiente& serpiente, std::vector<Comida>& comidas, const std::vector<Serpiente>& serpientes) {
    for (Comida& c : comidas) {
        if (serpienteComeComida(serpiente, c)) {
            hacerCrecer(serpiente);
            c = crearComida(serpientes);
            break;
        }
    }
}
