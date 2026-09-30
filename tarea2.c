#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char id[100];
  char artists[300];
  char album_name[300];
  char track_name[300];
  float tempo;
  char track_genre[100];
} Cancion;

// Menú principal
void mostrarMenuPrincipal() {
  limpiarPantalla();
  puts("========================================");
  puts("               Spotifind                ");
  puts("========================================");

  puts("1) Cargar Canciones");
  puts("2) Buscar por Género");
  puts("3) Buscar por Artista");
  puts("4) Buscar por Tempo");
  puts("5) Salir");
}

/**
 * Compara dos claves de tipo string para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo string.
 *
 * @param key1 Primer puntero a la clave string.
 * @param key2 Segundo puntero a la clave string.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

/**
 * Compara dos claves de tipo entero para determinar si son iguales.
 * Esta función se utiliza para inicializar mapas con claves de tipo entero.
 *
 * @param key1 Primer puntero a la clave entera.
 * @param key2 Segundo puntero a la clave entera.
 * @return Retorna 1 si las claves son iguales, 0 de lo contrario.
 */
int is_equal_int(void *key1, void *key2) {
  return *(int *)key1 == *(int *)key2; // Compara valores enteros directamente
}

void mostrar_cancion(Cancion *cancion)
{
  printf("\n------------------------------\n");
  printf("ID: %s\n", cancion->id);
  printf("Artista: %s\n", cancion->artists);
  printf("Álbum: %s\n", cancion->album_name);
  printf("Canción: %s\n", cancion->track_name);
  printf("Tempo: %.2f BPM\n", cancion->tempo);
  printf("Género: %s\n", cancion->track_genre);
}

void cargar_canciones(Map *canciones_bygenres, List *todas_las_canciones)
{
  char ruta[300];

  printf("Ingrese la ruta del archivo CSV: ");
  scanf(" %[^\n]", ruta);

  FILE *archivo = fopen(ruta, "r");
  
  if (archivo == NULL)
  {
    perror("Error al abrir el archivo");
    return;
  }

  char **campos;

  campos = leer_linea_csv(archivo, ','); 
  int cantidad = 0;

  while ((campos = leer_linea_csv(archivo, ',')) != NULL) {

    Cancion *cancion = (Cancion *)malloc(sizeof(Cancion));
    snprintf(cancion->id, sizeof(cancion->id), "%s", campos[0]);
    snprintf(cancion->artists, sizeof(cancion->artists), "%s", campos[2]);
    snprintf(cancion->album_name, sizeof(cancion->album_name), "%s", campos[3]);
    snprintf(cancion->track_name, sizeof(cancion->track_name), "%s", campos[4]);

    cancion->tempo = atof(campos[18]);

    snprintf(cancion->track_genre, sizeof(cancion->track_genre), "%s", campos[20]);
    list_pushBack(todas_las_canciones, cancion);

    MapPair *pair =
    map_search(canciones_bygenres, cancion->track_genre);

    if (pair == NULL)
    {
      List *lista = list_create();

      list_pushBack(lista, cancion);

      map_insert(canciones_bygenres, cancion->track_genre, lista);
    }

    else
    {
      List *lista = pair->value;

      list_pushBack(lista, cancion);

    }


    cantidad++;
  
  }
  fclose(archivo); // Cierra el archivo después de leer todas las líneas
  printf("Se cargaron %d canciones correctamente.\n", cantidad);

}

void buscar_por_artista(List *todas_las_canciones)
{
  char artista_buscado[300];

  printf("Ingrese el artista: ");
  scanf(" %[^\n]", artista_buscado);

  Cancion *cancion = list_first(todas_las_canciones);

  int encontrado = 0;

  while (cancion != NULL)
  {
    char copia_artistas[300];
    strcpy(copia_artistas, cancion->artists);
    List *artistas = split_string(copia_artistas, ";");

    char *artista = list_first(artistas);

    while (artista != NULL)
    {
      if (strcmp(artista, artista_buscado) == 0)
      {
        mostrar_cancion(cancion);
        encontrado = 1;
        break;
      }

      artista = list_next(artistas);
    }

    cancion = list_next(todas_las_canciones);
  }

  if (encontrado == 0)
  {
    printf("No se encontraron canciones del artista %s.\n",
           artista_buscado);
  }
}

void buscar_por_genero(Map *canciones_bygenres) {

  char genero[100];

  printf("Ingrese el género: ");
  scanf("%s", genero);

  MapPair *pair = map_search(canciones_bygenres, genero);

  if (pair == NULL) 
  {
    printf("No se encontraron canciones del género %s.\n", genero);
    return;
  }

  List *canciones = pair->value;

  Cancion *cancion = list_first(canciones);

  while (cancion != NULL) {

    printf("\n------------------------------\n");
    printf("ID: %s\n", cancion->id);
    printf("Artista: %s\n", cancion->artists);
    printf("Álbum: %s\n", cancion->album_name);
    printf("Canción: %s\n", cancion->track_name);
    printf("Tempo: %.2f BPM\n", cancion->tempo);
    printf("Género: %s\n", cancion->track_genre);

    cancion = list_next(canciones);
  }
}



void buscar_por_tempo(List *todas_las_canciones)
{
  int opcion;

  printf("\nSeleccione la velocidad:\n");
  printf("1) Lentas (menos de 80 BPM)\n");
  printf("2) Moderadas (80 - 120 BPM)\n");
  printf("3) Rápidas (más de 120 BPM)\n");

  printf("Ingrese su opción: ");
  scanf("%d", &opcion);

  if (opcion < 1 || opcion > 3)
  {
    printf("Opción inválida.\n");
    return;
  }

  Cancion *cancion =
      list_first(todas_las_canciones);

  int encontrado = 0;

  while (cancion != NULL)
  {
    int coincide = 0;

    if (opcion == 1 && cancion->tempo < 80)
    {
      coincide = 1;
    }
    else if (opcion == 2 &&
             cancion->tempo >= 80 &&
             cancion->tempo <= 120)
    {
      coincide = 1;
    }
    else if (opcion == 3 &&
             cancion->tempo > 120)
    {
      coincide = 1;
    }

    if (coincide)
    {
      mostrar_cancion(cancion);
      encontrado = 1;
    }

    cancion =
        list_next(todas_las_canciones);
  }

  if (encontrado == 0)
  {
    printf("No se encontraron canciones en esa categoría.\n");
  }
}


int main() {
  char opcion; 

  Map *canciones_bygenres = map_create(is_equal_str);
  List *todas_las_canciones = list_create();

  // Recuerda usar un mapa por criterio de búsqueda

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opción: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_canciones(canciones_bygenres, todas_las_canciones);
      break;
    case '2':
      buscar_por_genero(canciones_bygenres);
      break;
    case '3':
      buscar_por_artista(todas_las_canciones);
      break;
    case '4':
      buscar_por_tempo(todas_las_canciones);
      break;
    case '5':
      printf("Saliendo de Spotifind...\n");
      break;
    default:
      printf("Opción inválida.\n");
      break;
    }
    if (opcion != '5')
    {
      presioneTeclaParaContinuar(); 
    }
    
  } while (opcion != '5');

  return 0;
}