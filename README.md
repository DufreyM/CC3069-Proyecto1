# CC3069 - Proyecto 1: Screensaver "Serpientes IA"

Universidad del Valle de Guatemala - Computación Paralela y Distribuida

## Herramienta gráfica: SDL2

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
   g++ Screensaver.cpp -o Screensaver.exe -I"$SDLROOT/include" -L"$SDLROOT/lib" -lmingw32 -lSDL2main -lSDL2 -mwindows
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

g++ Screensaver.cpp -o Screensaver $(sdl2-config --cflags --libs)
./Screensaver
```

## Uso

El ejecutable acepta un parámetro opcional `N` para el número de segmentos de la serpiente:

```bash
./Screensaver.exe 20
```

Presiona `Esc` o cierra la ventana para salir.
