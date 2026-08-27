#include "serpiente.h"

#include <cmath>
#include <cstdlib>

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
