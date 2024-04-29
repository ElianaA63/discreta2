#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "API2024Parte2.h" 
#include "APIG24.h"

#define NUM_ORDENES 5
#define NUM_ITERACIONES 50
#define NUM_REPETICIONES 500

// Función para liberar la memoria asignada a un arreglo dinámico
void liberarMemoria(u32 *arreglo) {
    free(arreglo);
}

// Función para generar el orden natural 0, 1, ..., n-1 de los vértices
void generarOrdenNatural(u32 *Orden, u32 num_vertices) {
    for (u32 i = 0; i < num_vertices; i++) {
        Orden[i] = i;
    }
}

// Función para generar el orden inverso n-1, n-2, ..., 0 de los vértices
void generarOrdenInverso(u32 *Orden, u32 num_vertices) {
    for (u32 i = 0; i < num_vertices; i++) {
        Orden[i] = num_vertices - 1 - i;
    }
}

// Función para ejecutar GulDukat y Greedy
void ejecutarGulDukatYGreedy(Grafo G, u32 *Orden, u32 *num_colores_iteracion, u32 *ultimo_coloreo) {
    for (u32 i = 0; i < NUM_ITERACIONES; i++) {
        // Ejecutar GulDukat
        GulDukat(G, Orden);
        // Ejecutar Greedy
        num_colores_iteracion[i] = Greedy(G, Orden);
        // Guardar el último coloreo
        ExtraerColores(G, ultimo_coloreo);
    }
}

// Función para ejecutar ElimGarak y Greedy
void ejecutarElimGarakYGreedy(Grafo G, u32 *Orden, u32 *num_colores_iteracion, u32 *ultimo_coloreo) {
    for (u32 i = 0; i < NUM_ITERACIONES; i++) {
        // Ejecutar ElimGarak
        ElimGarak(G, Orden);
        // Ejecutar Greedy
        num_colores_iteracion[i] = Greedy(G, Orden);
        // Guardar el último coloreo
        ExtraerColores(G, ultimo_coloreo);
    }
}

// Función para recolorar los vértices con el coloreo que produjo la menor cantidad de colores
void recolorarConMenorColoreo(Grafo G, u32 *ultimo_coloreo) {
    ImportarColores(ultimo_coloreo, G);
}

// Función para elegir aleatoriamente entre GulDukat y ElimGarak
char elegirAleatoriamente() {
    return rand() % 2; // Genera un número aleatorio entre 0 y 1
}

// Función auxiliar que decide si un grafo está coloreado adecuadamente
// Nota: Por el Lema del Apretón de manos, esta función es O(m)
bool esPropio(Grafo G) {
    for (u32 i = 0; i < NumeroDeVertices(G); ++i) {
        for (u32 j = 0; j < Grado(i, G); ++j) {
            if (Color(i, G) == Color(Vecino(j, i, G), G)) {
                return false;
            }
        }
    }
    return true;
}

// Función auxiliar que imprime un arreglo
void arrayDump(u32* arr, u32 n) {
    printf("[");
    for (u32 i = 0; i < n-1; ++i) {
        printf("%u, ", arr[i]);
    }
    printf("%u]\n", arr[n-1]);
}

/*

int main() {
    srand(time(NULL)); // Inicializar la semilla para generar números aleatorios

    Grafo G = ConstruirGrafo(); // Cargar el grafo G

    u32 num_vertices = NumeroDeVertices(G);
    u32 num_colores_inicial[NUM_ORDENES];
    u32 min_num_colores = MAX_U32;
    u32 *ultimo_coloreo = malloc(num_vertices * sizeof(u32));
    if (!ultimo_coloreo) {
        fprintf(stderr, "Error: No se pudo asignar memoria para ultimo_coloreo\n");
        return 1;
    }

    // Funciones de generación de orden
    void (*generarOrden[NUM_ORDENES])(u32*, u32) = {
        generarOrdenNatural,
        generarOrdenInverso,
        // Implementar la lógica para las otras formas de orden
    };

    // Ejecutar Greedy en 5 ordenes distintos y almacenar la cantidad de colores
    for (u32 i = 0; i < NUM_ORDENES; i++) {
        u32 Orden[num_vertices];
       

        // Generar el orden correspondiente
        generarOrden[i](Orden, num_vertices);

        // Ejecutar Greedy y almacenar la cantidad de colores
        num_colores_inicial[i] = Greedy(G, Orden);
        if (num_colores_inicial[i] < min_num_colores) {
            min_num_colores = num_colores_inicial[i];
        }
        printf("Greedy con orden %u: %u colores\n", i, num_colores_inicial[i]);

        // Variables para almacenar la cantidad de colores y el último coloreo en cada iteración
        u32 num_colores_iteracion[NUM_ITERACIONES];

   
        // Ejecutar GulDukat y Greedy, y luego ElimGarak y Greedy 50 veces
        ejecutarGulDukatYGreedy(G, Orden,  num_colores_iteracion, ultimo_coloreo);
        ejecutarElimGarakYGreedy(G, Orden,  num_colores_iteracion, ultimo_coloreo);

        // Comparar los coloreos y recolorear los vértices con el menor coloreo
        recolorarConMenorColoreo(G, ultimo_coloreo);

        // Variables para almacenar el número de colores obtenidos
        u32 num_colores_obtenidos;

        // Iterar 500 veces para cada orden inicial
        for (u32 k = 0; k < NUM_REPETICIONES; k++) {
            // Elegir aleatoriamente entre GulDukat y ElimGarak
            char eleccion = elegirAleatoriamente();
            
            if (eleccion) {
                // Ejecutar GulDukat y Greedy
                GulDukat(G, Orden);
                num_colores_obtenidos = Greedy(G, Orden);
            } else {
                // Ejecutar ElimGarak y Greedy
                ElimGarak(G, Orden);
                num_colores_obtenidos = Greedy(G, Orden);
            }
            
            // Imprimir la cantidad de colores obtenida
            printf("Iteración %u, coloreo %u: %u colores\n", i * NUM_REPETICIONES + k + 1, eleccion, num_colores_obtenidos);

            // Comprobar si el coloreo es propio y usar la función para imprimir el arreglo
            if (!esPropio(G)) {
                printf("El coloreo no es propio.\n");
            }
            arrayDump(ultimo_coloreo, num_vertices);

            
        }
    }
   
    liberarMemoria(ultimo_coloreo);
    DestruirGrafo(G);

    return 0;
}

*/

/* Main trucho:*/ 

int main() {
    
    Grafo G = ConstruirGrafo();
    if (G == NULL) {
        fprintf(stderr, "ERROR: Fallo al leer el grafo.\n");
        fprintf(stderr, "Cerrando el programa ...\n");
        exit(1);
    }

    u32 *orden_natural = malloc(NumeroDeVertices(G) * sizeof(u32));

    for (u32 i = 0; i < NumeroDeVertices(G); ++i)
    {
        orden_natural[i] = i;
    }

    printf("Greedy con orden natural: %u\n", Greedy(G, orden_natural));

    free(orden_natural);
    DestruirGrafo(G);

    return 0;
}


