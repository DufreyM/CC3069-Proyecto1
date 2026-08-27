# TODO - reparto de trabajo (2026-08-25)

Estado actual: versión secuencial con N serpientes, colisiones serpiente-serpiente
y comida (una sola, la serpiente que la toca crece). Falta toda la parte paralela
(OpenMP) y la instrumentación de tiempos para el informe.

Antes de empezar: `git pull`. Todos tocamos `Screensaver.cpp`, así que conviene
avisar en el chat del equipo qué función estás editando y commitear seguido en
piezas chicas para minimizar conflictos de merge.

## Leonardo — prototipo de paralelización con OpenMP

- Agregar `-fopenmp` a las instrucciones de compilación del README (Windows/Linux/macOS).
- Paralelizar `actualizarSerpiente` sobre el vector de serpientes con
  `#pragma omp parallel for` — es paralelizable porque cada serpiente es
  independiente de las demás en esta etapa (mismo principio que el ejemplo de
  burbujas de la diapositiva de planificación de loops).
- Probar primero con `schedule(static)` (carga pareja entre serpientes, bajo
  overhead — diapositiva 13).
- No borrar la versión secuencial: dejar ambas disponibles para poder comparar
  tiempos (p. ej. con un flag de compilación o un segundo target de build).

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

## Cindy — comida múltiple y defensiva de spawn

- Cambiar la comida única (`Comida comida`) a `std::vector<Comida> comidas`
  (por ahora un número fijo, ej. `NUM_COMIDA`).
- Al generar una comida (o serpiente) nueva, evitar que aparezca encima de una
  serpiente existente — validación defensiva de posición, en la línea de lo
  que ya hicimos con el argumento `N`.
- Actualizar `serpienteComeComida` / `hacerCrecer` para recorrer el vector de
  comidas en vez de un solo elemento.
- Esto da más elementos en pantalla y hace que valga la pena paralelizar
  también ese loop más adelante.

## Al terminar

Avisar en el grupo para juntar todo, resolver conflictos de merge si los hay,
y decidir el siguiente paso (probablemente: aplicar OpenMP también a
colisiones/comida, y correr las mediciones paralelas para el speedup real).
