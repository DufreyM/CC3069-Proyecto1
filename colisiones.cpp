#include "colisiones.h"

#include <algorithm>

bool segmentosColisionan(const Segmento& a, const Segmento& b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float distanciaCuadrada = dx * dx + dy * dy;
    float distanciaMinima = RADIO_SEGMENTO * 2.0f;

    return distanciaCuadrada <= distanciaMinima * distanciaMinima;
}

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
