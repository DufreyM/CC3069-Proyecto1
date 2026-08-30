#include "serpiente.h"
#include "colisiones.h"

#include <cmath>
#include <cstdlib>

// Dentro de la zona de costo mayor, gira el vector de velocidad un angulo
// pequeno cada frame: la serpiente entra recta y sale trazando una espiral,
// bien distinto del rebote normal en el resto del canvas. El angulo sale de
// sumar muchos terminos seno (no una formula cerrada) a proposito, para
// simular un calculo caro por iteracion, como el ejemplo de la diapositiva
// de planificacion de loops. Depende de la posicion de la cabeza para que el
// compilador no pueda precalcularlo en tiempo de compilacion.
static void aplicarOndulacionCostosa(Serpiente& s) {
    const Segmento& cabeza = s.segmentos[0];
    float fase = cabeza.x * 0.017f + cabeza.y * 0.011f;

    float giro = 0.0f;
    for (int k = 1; k <= ITERACIONES_ZONA_COSTOSA; ++k) {
        giro += std::sin(fase + static_cast<float>(k) * 0.05f) * 0.00016f;
    }

    float cosGiro = std::cos(giro);
    float sinGiro = std::sin(giro);
    float nuevaVelX = s.velX * cosGiro - s.velY * sinGiro;
    float nuevaVelY = s.velX * sinGiro + s.velY * cosGiro;
    s.velX = nuevaVelX;
    s.velY = nuevaVelY;
}

// Gira la velocidad de la serpiente hacia la comida mas cercana, sin cambiar
// su rapidez actual. El giro es gradual (steering, no un salto instantaneo)
// para que el movimiento siga viendose organico.
static void buscarComidaCercana(Serpiente& s, const std::vector<Segmento>& posicionesComida) {
    if (posicionesComida.empty()) {
        return;
    }

    const Segmento& cabeza = s.segmentos[0];
    const Segmento* masCercana = &posicionesComida[0];
    float distMinCuadrada = -1.0f;

    for (const Segmento& comida : posicionesComida) {
        float dx = comida.x - cabeza.x;
        float dy = comida.y - cabeza.y;
        float distCuadrada = dx * dx + dy * dy;
        if (distMinCuadrada < 0.0f || distCuadrada < distMinCuadrada) {
            distMinCuadrada = distCuadrada;
            masCercana = &comida;
        }
    }

    float dx = masCercana->x - cabeza.x;
    float dy = masCercana->y - cabeza.y;
    float distancia = std::sqrt(distMinCuadrada);
    if (distancia < 0.0001f) {
        return;
    }

    float rapidez = std::sqrt(s.velX * s.velX + s.velY * s.velY);
    float deseadoX = dx / distancia * rapidez;
    float deseadoY = dy / distancia * rapidez;

    s.velX += (deseadoX - s.velX) * FUERZA_BUSQUEDA_COMIDA;
    s.velY += (deseadoY - s.velY) * FUERZA_BUSQUEDA_COMIDA;

    // Renormaliza para que el giro hacia la comida no vaya frenando ni
    // acelerando a la serpiente con el paso de los frames.
    float rapidezNueva = std::sqrt(s.velX * s.velX + s.velY * s.velY);
    if (rapidezNueva > 0.0001f) {
        float factor = rapidez / rapidezNueva;
        s.velX *= factor;
        s.velY *= factor;
    }
}

Serpiente crearSerpiente(const std::vector<Serpiente>& serpientes) {
    Serpiente s;
    s.segmentos.resize(SEGMENTOS_POR_SERPIENTE);

    float xInicial = 0.0f;
    float yInicial = 0.0f;
    for (int intento = 0; intento < INTENTOS_MAX_SPAWN; ++intento) {
        xInicial = aleatorioEnRango(RADIO_SEGMENTO * 2.0f, anchoVentana - RADIO_SEGMENTO * 2.0f);
        yInicial = aleatorioEnRango(RADIO_SEGMENTO * 2.0f, altoVentana - RADIO_SEGMENTO * 2.0f);

        if (!posicionOcupada(xInicial, yInicial, RADIO_SEGMENTO, serpientes)) {
            break;
        }
    }

    for (int i = 0; i < SEGMENTOS_POR_SERPIENTE; ++i) {
        s.segmentos[i].x = xInicial - i * (RADIO_SEGMENTO * 1.6f);
        s.segmentos[i].y = yInicial;
    }

    s.velX = aleatorioEnRango(VELOCIDAD_MIN, VELOCIDAD_MAX) * (std::rand() % 2 == 0 ? 1.0f : -1.0f);
    s.velY = aleatorioEnRango(VELOCIDAD_MIN, VELOCIDAD_MAX) * (std::rand() % 2 == 0 ? 1.0f : -1.0f);

    s.r = static_cast<Uint8>(aleatorioEnRango(80.0f, 255.0f));
    s.g = static_cast<Uint8>(aleatorioEnRango(80.0f, 255.0f));
    s.b = static_cast<Uint8>(aleatorioEnRango(80.0f, 255.0f));

    return s;
}

void actualizarSerpiente(Serpiente& s, const std::vector<Segmento>& posicionesComida) {
    // Primero, cada segmento toma la posición anterior del segmento delantero.
    for (int i = static_cast<int>(s.segmentos.size()) - 1; i > 0; --i) {
        s.segmentos[i] = s.segmentos[i - 1];
    }

    buscarComidaCercana(s, posicionesComida);

    Segmento& cabeza = s.segmentos[0];
    cabeza.x += s.velX;
    cabeza.y += s.velY;

    if (dentroDeZonaCostosa(cabeza.x, cabeza.y)) {
        aplicarOndulacionCostosa(s);
    }

    if (cabeza.x - RADIO_SEGMENTO < 0) {
        cabeza.x = RADIO_SEGMENTO;
        s.velX = std::fabs(s.velX);
    } else if (cabeza.x + RADIO_SEGMENTO > anchoVentana) {
        cabeza.x = anchoVentana - RADIO_SEGMENTO;
        s.velX = -std::fabs(s.velX);
    }

    if (cabeza.y - RADIO_SEGMENTO < 0) {
        cabeza.y = RADIO_SEGMENTO;
        s.velY = std::fabs(s.velY);
    } else if (cabeza.y + RADIO_SEGMENTO > altoVentana) {
        cabeza.y = altoVentana - RADIO_SEGMENTO;
        s.velY = -std::fabs(s.velY);
    }
}

void actualizarSerpientes(std::vector<Serpiente>& serpientes, const std::vector<Segmento>& posicionesComida) {
    const int total = static_cast<int>(serpientes.size());

    // schedule(static): todas las serpientes hacen la misma cantidad de
    // trabajo por frame, asi que repartirlas en bloques fijos entre hilos
    // alcanza el balance de carga sin el overhead de dynamic/guided.
    // posicionesComida solo se lee, nunca se modifica, asi que compartirla
    // entre hilos es seguro.
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < total; ++i) {
        actualizarSerpiente(serpientes[i], posicionesComida);
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
