# TODO - reparto de trabajo (2026-08-25)

Estado actual: versión secuencial con N serpientes, colisiones serpiente-serpiente
y comida (una sola, la serpiente que la toca crece). Falta toda la parte paralela
(OpenMP) y la instrumentación de tiempos para el informe.

Antes de empezar: `git pull`. Todos tocamos `Screensaver.cpp`, así que conviene
avisar en el chat del equipo qué función estás editando y commitear seguido en
piezas chicas para minimizar conflictos de merge.

## Leonardo — prototipo de paralelización con OpenMP [DONE]

- [x] `-fopenmp` agregado a las instrucciones de compilación del README (Windows/Linux/macOS).
- [x] `actualizarSerpientes()` en `serpiente.cpp` reparte el update de cada
  serpiente con `#pragma omp parallel for schedule(static)` — es paralelizable
  porque cada serpiente es independiente de las demás en esta etapa (mismo
  principio que el ejemplo de burbujas de la diapositiva de planificación de
  loops).
- [x] Compila y corre igual con y sin `-fopenmp` (con el flag imprime cuántos
  hilos usa; sin el flag, GCC ignora el pragma y corre secuencial).
- Pendiente / siguiente: aplicar lo mismo a colisiones y comida cuando esos
  loops crezcan, y comparar tiempos secuencial vs. paralelo una vez que Maria
  Jose tenga el modo de medición.

## Maria Jose — instrumentación de tiempos (Anexo 3)

- Agregar un modo de medición (flag `--bench N_FRAMES` o similar) que corra un
  número fijo de frames sin esperar vsync y al terminar imprima: N, frames,
  tiempo total y FPS promedio, en un formato fácil de pasar a tabla (CSV).
- Con eso, correr al menos 10 mediciones por cada N de prueba (ej. 10, 50, 100,
  150, 200) sobre la versión secuencial actual y guardar los resultados
  (`mediciones.csv` o similar) — esto ya adelanta el Anexo 3 aunque la parte
  paralela no esté lista todavía.
- Referencia: requisito E del enunciado — el speedup se calcula con el
  promedio o el máximo de las mediciones anteriores.

## Cindy — comida múltiple y defensiva de spawn [DONE]

- [x] Cambiada la comida única (`Comida comida`) a `std::vector<Comida> comidas`
  con cantidad fija `NUM_COMIDA` (5) en `util.h`.
- [x] `posicionOcupada()` (`colisiones.h`/`.cpp`) evita que `crearComida()` y
  `crearSerpiente()` aparezcan encima de una serpiente existente, reintentando
  la posición hasta `INTENTOS_MAX_SPAWN` veces sin colgarse si no hay hueco
  libre — misma idea defensiva que ya usamos con el argumento `N`.
- [x] `procesarComidas()` en `comida.cpp` encapsula el recorrido del vector de
  comidas por serpiente (`serpienteComeComida` / `hacerCrecer` / respawn).
- Pendiente / siguiente: este loop (y el de `eliminarSerpientesColisionadas`)
  queda como candidato natural para paralelizar con OpenMP más adelante.

## Al terminar

Avisar en el grupo para juntar todo, resolver conflictos de merge si los hay,
y decidir el siguiente paso (probablemente: aplicar OpenMP también a
colisiones/comida, y correr las mediciones paralelas para el speedup real).
