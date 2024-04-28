#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "API2024Parte2.h" 
#include "APIG24.h"

#define NUM_REORDER_ITERATIONS 50
#define NUM_INITIAL_ORDERS 5
#define NUM_FINAL_ORDERS_ITERATIONS 500

/*
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
*/
/*
// Función auxiliar que decide si un grafo está coloreado adecuadamente
// Nota: Por el Lema del Apretón de manos, esta función es O(m)
bool esPropio (Grafo G) {
    for (u32 i = 0; i < NumeroDeVertices(G); ++i)
    {
        for (u32 j = 0; j < Grado(i, G); ++j)
        {
            if (Color(i, G) == Color(Vecino(j, i, G), G))
            {
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

int main() {

 Main trucho:

    Grafo G = ConstruirGrafo();

    u32 orden_natural[NumeroDeVertices(G)];
    u32 orden_modificado[NumeroDeVertices(G)];

    for (u32 i = 0; i < NumeroDeVertices(G); ++i)
    {
        orden_natural[i] = i;
        orden_modificado[i] = i;
    }

    // Ejecutar GulDukat
    char resultado_guldukat = GulDukat(G, orden_natural);
    if (resultado_guldukat != 0) {
        printf("Hubo un error al ejecutar GulDukat.\n");
        DestruirGrafo(G);
        return 1;
    }

    // Ejecutar Greedy después de GulDukat
    u32 colores_greedy = Greedy(G, orden_natural);
    if (colores_greedy == MAX_U32) {
        printf("Error al ejecutar Greedy después de GulDukat.\n");
        DestruirGrafo(G);
        return 1;
    }

    // Imprimir el número de colores obtenido por Greedy despues de GulDukat

    printf("Greedy despues de GulDukat con orden natural: %u\n", colores_greedy);
    // Verificar si el coloreo es propio
    if (esPropio(G)) {
        printf("El coloreo es propio.\n");
    } else {
        printf("El coloreo no es propio.\n");
    }

    printf("Colores de G: ");
    u32 Colores[NumeroDeVertices(G)];
    ExtraerColores(G, Colores);
    arrayDump(Colores, NumeroDeVertices(G));



    // 2. Ejecutar ElimGarak y luego Greedy con ese orden
    char resultado_garak = ElimGarak(G, orden_modificado);
    if (resultado_garak != 0) {
        printf("Hubo un error al ejecutar ElimGarak.\n");
        DestruirGrafo(G);
        return 1;
    }

    u32 colores_greedy_garak = Greedy(G, orden_modificado);
    if (colores_greedy_garak == MAX_U32) {
        printf("Error al ejecutar Greedy después de ElimGarak.\n");
        DestruirGrafo(G);
        return 1;
    }

    printf("Greedy después de ElimGarak con orden modificado: %u\n", colores_greedy_garak);


       // Verificar si el coloreo es propio
    if (esPropio(G)) {
        printf("El coloreo es propio.\n");
    } else {
        printf("El coloreo no es propio.\n");
    }

    // Liberar la memoria del grafo
    DestruirGrafo(G);

    return 0;
}*/

// Función auxiliar para decidir si un grafo está coloreado adecuadamente
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

// Función auxiliar para imprimir un arreglo
void arrayDump(u32 *arr, u32 n) {
    printf("[");
    for (u32 i = 0; i < n - 1; ++i) {
        printf("%u, ", arr[i]);
    }
    printf("%u]\n", arr[n - 1]);
}
// Función para intercambiar dos elementos en un arreglo
void swap(u32 *a, u32 *b) {
    u32 temp = *a;
    *a = *b;
    *b = temp;
}

// Función para mezclar un arreglo usando el algoritmo de Fisher-Yates (mezcla aleatoria)
void shuffle(u32 *arr, u32 n) {
    // Comenzar desde el último elemento e ir intercambiando con un elemento aleatorio anterior
    for (u32 i = n - 1; i > 0; --i) {
        u32 j = rand() % (i + 1); // Generar un índice aleatorio entre 0 y i (inclusive)
        swap(&arr[i], &arr[j]);   // Intercambiar arr[i] con el elemento en arr[j]
    }
}

// Función principal
int main() {
    // Inicializar la semilla para el generador de números pseudoaleatorios
    srand(time(NULL));

    Grafo G = ConstruirGrafo();

    // Ordenamientos iniciales
    u32 ordenamientos[5][NumeroDeVertices(G)];

    // Ejecutar Greedy en 5 ordenamientos distintos
    printf("Greedy en 5 ordenamientos distintos:\n");
    for (int i = 0; i < 5; ++i) {
        for (u32 j = 0; j < NumeroDeVertices(G); ++j) {
            ordenamientos[i][j] = j;
        }

        // Ordenar el arreglo de manera aleatoria para obtener ordenamientos distintos
        shuffle(ordenamientos[i], NumeroDeVertices(G));

        u32 colores_greedy = Greedy(G, ordenamientos[i]);
        if (colores_greedy == MAX_U32) {
            printf("Error al ejecutar Greedy en el ordenamiento %d.\n", i + 1);
            DestruirGrafo(G);
            return 1;
        }

        printf("Ordenamiento %d - Colores utilizados por Greedy: ", i + 1);
        u32 ColoresGreedy[NumeroDeVertices(G)];
        ExtraerColores(G, ColoresGreedy);
        arrayDump(ColoresGreedy, NumeroDeVertices(G));
        printf("Greedy en el ordenamiento %d: %u colores\n", i + 1, colores_greedy);
    }

    printf("\n");

    // Para cada ordenamiento
    for (int i = 0; i < 5; ++i) {
        printf("Ordenamiento %d:\n", i + 1);

        // Correr GulDukat y luego Greedy
        char resultado_guldukat = GulDukat(G, ordenamientos[i]);
        if (resultado_guldukat != 0) {
            printf("Error al ejecutar GulDukat en el ordenamiento %d.\n", i + 1);
            DestruirGrafo(G);
            return 1;
        }

        printf("GulDukat en el ordenamiento %d: Grafo coloreado correctamente\n", i + 1);

        u32 colores_greedy_guldukat = Greedy(G, ordenamientos[i]);
        if (colores_greedy_guldukat == MAX_U32) {
            printf("Error al ejecutar Greedy después de GulDukat en el ordenamiento %d.\n", i + 1);
            DestruirGrafo(G);
            return 1;
        }

        printf("Greedy después de GulDukat en el ordenamiento %d: %u colores\n", i + 1, colores_greedy_guldukat);

        // Correr ElimGarak y luego Greedy
        char resultado_garak = ElimGarak(G, ordenamientos[i]);
        if (resultado_garak != 0) {
            printf("Error al ejecutar ElimGarak en el ordenamiento %d.\n", i + 1);
            DestruirGrafo(G);
            return 1;
        }

        printf("ElimGarak en el ordenamiento %d: Grafo coloreado correctamente\n", i + 1);

        u32 colores_greedy_garak = Greedy(G, ordenamientos[i]);
        if (colores_greedy_garak == MAX_U32) {
            printf("Error al ejecutar Greedy después de ElimGarak en el ordenamiento %d.\n", i + 1);
            DestruirGrafo(G);
            return 1;
        }

        printf("Greedy después de ElimGarak en el ordenamiento %d: %u colores\n", i + 1, colores_greedy_garak);

        printf("\n");
    }

     // Liberar memoria de los ordenamientos
    for (int i = 0; i < 5; ++i) {
        free(ordenamientos[i]);
    }

    DestruirGrafo(G);

    return 0;
}



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


