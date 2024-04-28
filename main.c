#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "API2024Parte2.h" 
#include "APIG24.h"

#define NUM_REORDER_ITERATIONS 50
#define NUM_INITIAL_ORDERS 5
#define NUM_FINAL_ORDERS_ITERATIONS 500


void generarOrdenesIniciales(Grafo G, u32* initial_orders[NUM_INITIAL_ORDERS]) {
    u32 num_vertices = NumeroDeVertices(G);
    
    // Orden natural: 0, 1, ..., n-1
    for (u32 i = 0; i < num_vertices; i++) {
        initial_orders[0][i] = i;
    }

    // Orden inverso: n-1, n-2, ..., 0
    for (u32 i = 0; i < num_vertices; i++) {
        initial_orders[1][i] = num_vertices - 1 - i;
    }

    // Orden pares decreciente e impares creciente
    u32 index = 0;
    for (u32 i = 0; i < num_vertices; i += 2) {
        initial_orders[2][index++] = num_vertices - 1 - i;
    }
    for (u32 i = 1; i < num_vertices; i += 2) {
        initial_orders[2][index++] = i;
    }

    // Ordenar los vértices según su grado, de mayor a menor
    u32* grados = malloc(num_vertices * sizeof(u32));
    for (u32 i = 0; i < num_vertices; i++) {
        grados[i] = Grado(i, G);
    }
    for (u32 i = 0; i < num_vertices; i++) {
        u32 max_grado = 0;
        u32 max_index = 0;
        for (u32 j = 0; j < num_vertices; j++) {
            if (grados[j] > max_grado) {
                max_grado = grados[j];
                max_index = j;
            }
        }
        initial_orders[3][i] = max_index;
        grados[max_index] = 0; // Para no volver a seleccionar este vértice
    }
    free(grados);

    // Orden extra: definir tu orden extra aquí
}

u32 compararCorridas(u32* num_colors, u32 num_initial_orders) {
    u32 min_colors = num_colors[0];
    u32 min_colors_index = 0;
    for (u32 i = 1; i < num_initial_orders; i++) {
        if (num_colors[i] < min_colors) {
            min_colors = num_colors[i];
            min_colors_index = i;
        }
    }
    return min_colors_index;
}

// Función auxiliar que decide si un grafo está coloreado adecuadamente
// Nota: Por el Lema del Apretón de manos, esta función es O(m)
bool esPropio (Grafo G) {
    for (u32 i = 0; i < NumeroDeVertices(G); ++i)
    {
        for (u32 j = 0; i < Grado(i, G); ++j)
        {
            if (Color(i, G) == Color(Vecino(j, i, G), G))
            {
                return false;
            }
        }
    }
    return true;
}

int main() {

/* Main trucho: */

    Grafo G = ConstruirGrafo();

    u32 orden_natural[NumeroDeVertices(G)];

    for (u32 i = 0; i < NumeroDeVertices(G); ++i)
    {
        orden_natural[i] = i;
    }

    orden_natural[0] = 1;
    orden_natural[1] = 0;

    printf("Greedy con orden natural: %u\n", Greedy(G, orden_natural));

    if (esPropio(G))
    {
        printf("El coloreo es propio\n");
    }
    else
    {
        printf("El coloreo no es propio\n");
    }

    return 0;

/* Main posta: */

/*

    // Semilla para generar números aleatorios
    srand(time(NULL));

    // Variables para almacenar los colores obtenidos en cada iteración
    u32 num_colors[NUM_INITIAL_ORDERS];
    u32 num_colors_iterations[NUM_INITIAL_ORDERS][NUM_REORDER_ITERATIONS];

    // Crear el grafo
    Grafo G = ConstruirGrafo();

    // Array de ordenes iniciales
    u32* initial_orders[NUM_INITIAL_ORDERS];

    // Inicializar los ordenes iniciales
    for (u32 i = 0; i < NUM_INITIAL_ORDERS; i++) {
        initial_orders[i] = malloc(NumeroDeVertices(G) * sizeof(u32));
        if (initial_orders[i] == NULL) {
            fprintf(stderr, "Error: No se pudo asignar memoria para los ordenes iniciales\n");
            return 1;
        }
    }

    // Generar los ordenes iniciales
    generarOrdenesIniciales(G, initial_orders);

    // Ejecutar Greedy en cada uno de los ordenes iniciales
    for (u32 i = 0; i < NUM_INITIAL_ORDERS; i++) {
        num_colors[i] = Greedy(G, initial_orders[i]);
        printf("Numero de colores obtenidos con el orden inicial %u: %u\n", i, num_colors[i]);

        // Ejecutar GulDukat y Greedy, y luego ElimGarak y Greedy en cada iteración
        for (u32 j = 0; j < NUM_REORDER_ITERATIONS; j++) {
            GulDukat(G, initial_orders[i]);
            num_colors_iterations[i][j] = Greedy(G, initial_orders[i]);
            printf("Iteracion %u - Numero de colores obtenidos: %u\n", j + 1, num_colors_iterations[i][j]);
            ElimGarak(G, initial_orders[i]);
            num_colors_iterations[i][j] = Greedy(G, initial_orders[i]);
            printf("Iteracion %u - Numero de colores obtenidos: %u\n", j + 1, num_colors_iterations[i][j]);
        }
    }

    // Comparar las 5 corridas y tomar el coloreo con la menor cantidad de colores
    u32 min_colors_index = compararCorridas(num_colors, NUM_INITIAL_ORDERS);

    // Recolorear los vértices con el coloreo que dio la menor cantidad de colores
    u32* final_order = initial_orders[min_colors_index];
    u32 min_colors = num_colors[min_colors_index];
    ImportarColores(final_order, G);

    // Hacer 500 reordenamientos seguidos de Greedy
    for (u32 i = 0; i < NUM_FINAL_ORDERS_ITERATIONS; i++) {
        // Elegir al azar GulDukat o ElimGarak con una probabilidad de 50%
        if (rand() % 2 == 0) {
            GulDukat(G, final_order);
        } else {
            ElimGarak(G, final_order);
        }
        num_colors_iterations[i][0] = Greedy(G, final_order);
        printf("Iteracion %u - Numero de colores obtenidos: %u\n", i + 1, num_colors_iterations[i]);
    }

    // Liberar memoria y destruir el grafo
    for (u32 i = 0; i < NUM_INITIAL_ORDERS; i++) {
        free(initial_orders[i]);
    }
    DestruirGrafo(G);

    return 0;

*/

}
