# CC3069 - Proyecto 1: Screensaver "Multisnake"

Universidad del Valle de Guatemala - Computación Paralela y Distribuida

## Herramienta gráfica: SDL2

## Estructura del código

| Archivo | Contenido |
|---|---|
| `main.cpp` | `main()`, inicialización de SDL y el loop principal |
| `util.h/.cpp` | Constantes, `Segmento`, dibujo de círculos, números aleatorios, parseo de `N` |
| `serpiente.h/.cpp` | `Serpiente` y su movimiento/dibujo |
| `colisiones.h/.cpp` | Detección y eliminación de serpientes colisionadas |
| `comida.h/.cpp` | `Comida` y su generación/consumo |

## Compilación en Windows (MinGW-w64)

Estos pasos asumen que ya tienes instalado **MinGW-w64** (g++) y que corres los comandos desde Git Bash / MSYS.

1. Descarga las librerías de desarrollo de SDL2 para MinGW desde la [página de releases de SDL](https://github.com/libsdl-org/SDL/releases) (archivo `SDL2-devel-<version>-mingw.zip`, por ejemplo `SDL2-devel-2.30.9-mingw.zip`).

2. Extrae el `.zip` dentro de una carpeta `libs/` en la raíz del proyecto. Debe quedar así:

   ```
   libs/SDL2-2.30.9/x86_64-w64-mingw32/include/SDL2/SDL.h
   libs/SDL2-2.30.9/x86_64-w64-mingw32/lib/
   libs/SDL2-2.30.9/x86_64-w64-mingw32/bin/SDL2.dll
   ```

   (`libs/` está en `.gitignore` porque son ~7MB de librerías descargadas, no código fuente.)

3. Compila:

   ```bash
   SDLROOT="libs/SDL2-2.30.9/x86_64-w64-mingw32"
   g++ *.cpp -o Screensaver.exe -fopenmp -I"$SDLROOT/include" -L"$SDLROOT/lib" -lmingw32 -lSDL2main -lSDL2 -mwindows
   ```

4. Copia `SDL2.dll` junto al ejecutable (necesario en tiempo de ejecución):

   ```bash
   cp "$SDLROOT/bin/SDL2.dll" .
   ```

5. Ejecuta `Screensaver.exe`.

   > **Nota:** en Git Bash, correr directamente `./Screensaver.exe` puede fallar (exit code 127) porque Git Bash maneja mal las apps con subsistema Windows (`-mwindows`). Ejecútalo desde el explorador de archivos (doble clic), `cmd`, o PowerShell.

## Compilación en Linux / macOS

```bash
sudo apt install libsdl2-dev   # Debian/Ubuntu
# o: brew install sdl2         # macOS

g++ *.cpp -o Screensaver -fopenmp $(sdl2-config --cflags --libs)
./Screensaver
```

## Uso

El ejecutable acepta un parámetro opcional `N` para el número de serpientes a renderizar (por defecto 5, rango válido 1-200). Cada serpiente se genera con posición, velocidad y color pseudoaleatorios:

```bash
./Screensaver.exe 20
```

Si `N` no es un número válido o está fuera de rango, el programa avisa por consola y continúa con un valor seguro en lugar de fallar.

Presiona `Esc` o cierra la ventana para salir.

