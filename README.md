# Spotifind

Spotifind es un programa hecho en C que permite cargar una base de datos de
canciones desde un archivo CSV y buscar canciones por género, artista o tempo.

Para guardar y organizar las canciones utilicé los TDAs `List` y `Map`.

## Funciones

El programa tiene las siguientes opciones:

1. **Cargar canciones:** pide la ruta del archivo CSV y carga las canciones.
2. **Buscar por género:** muestra las canciones del género ingresado.
3. **Buscar por artista:** muestra las canciones del artista buscado. También
   reconoce canciones que tienen más de un artista separado por `;`.
4. **Buscar por tempo:** permite buscar canciones lentas (menos de 80 BPM),
   moderadas (80 a 120 BPM) o rápidas (más de 120 BPM).
5. **Salir:** termina el programa.

Todas las opciones se encuentran funcionando correctamente.

## Cómo ejecutar

Primero se debe compilar desde la carpeta `TDAs`:

gcc tarea2.c tdas/list.c tdas/map.c tdas/extra.c -o tarea2

Luego ejecutar:

./tarea2

Para cargar el archivo utilizado en la tarea se ingresa:

data/song_dataset_.csv

## Ejemplo de uso

Ingrese su opción: 1
Ingrese la ruta del archivo CSV: data/song_dataset_.csv
Se cargaron 114000 canciones correctamente.

Ingrese su opción: 3
Ingrese el artista: ZAYN

El programa mostrará todas las canciones encontradas para ese artista,
incluyendo las que tienen más de un artista.

## TDAs utilizados

Utilicé una `List` para guardar todas las canciones cargadas. Esta lista se usa
principalmente para las búsquedas por artista y tempo.

También utilicé un `Map` para organizar las canciones según su género. Cada
género tiene asociada una lista con sus canciones.

## Manejo de errores

El programa avisa si el archivo ingresado no se puede abrir, si no se encuentran
resultados o si se ingresa una opción inválida.

## Contribución

Nombre : Trinidad Tejeda Manzano
El trabajo fue realizado de forma individual. Realicé la carga del archivo,
las búsquedas por género, artista y tempo, el uso de los TDAs y las pruebas
del programa.