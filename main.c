#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "API2024Parte2.h" 
#include "APIG24.h"

#define NUM_REORDER_ITERATIONS 50
#define NUM_REORDER_ITERATIONS_COLORS 100 
#define NUM_INITIAL_ORDERS 5
#define NUM_FINAL_ORDERS_ITERATIONS 500
#define DELIMITATION_BAR "$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n"

/*
// Función auxiliar para imprimir un arreglo
void arrayDump(u32 *arr, u32 n) {
    printf("[");
    for (u32 i = 0; i < n - 1; ++i) {
        printf("%u, ", arr[i]);
    }
    printf("%u]\n", arr[n - 1]);
}
*/

// Precondición: n >= 2
static void generarOrdenesIniciales(Grafo G, u32* initial_orders[NUM_INITIAL_ORDERS], u32 n) {

    // Orden natural: 0, 1, ..., n-1
    for (u32 i = 0; i < n; ++i) {
        initial_orders[0][i] = i;
    }

    // Orden inverso: n-1, n-2, ..., 0
    for (u32 i = 0; i < n; ++i) {
        initial_orders[1][i] = (n - 1) - i;
    }

    // Orden pares decreciente e impares creciente
    int num = ((n - 1) % 2 == 0) ? n - 1 : n - 2;
    u32 nprima = (num == (int)(n - 1)) ? (n/2) + 1 : (n/2);
    for (u32 i = 0; i < nprima; ++i) {
        initial_orders[2][i] = num;
        num -= 2;
    }
    num = 1;
    for (u32 i = ((n - 1) % 2 == 0) ? (n/2) + 1 : (n/2); i < n; ++i)
    {
        initial_orders[2][i] = num;
        num += 2;
    } 

    // Ordenar los vértices según su grado, de mayor a menor
    // Nota: si el grafo es regular, es equivalente al orden natual
    u32 *Grados = malloc(n * sizeof(u32));
    for (u32 i = 0; i < n; ++i) {
        Grados[i] = Grado(i, G);
    }
    u32 Max_grado_actual = 0;
    u32 Max_grado_anterior = n - 1; //Se incializa antinaturalmente para que funcione la primera iteración
    u32 min_grado_actual = n - 1;
    u32 min_grado_anterior = 0; //Se incializa antinaturalmente para que funcione la primera iteración
    int indiceM = 0; // Indica donde colocar los vértices del máximo grado anterior
    int indicem = n - 1; // Indica donde colocar los vértices del mínimo grado anterior
    u32 agregados = 0; //Variable para la condición de parada
    while (agregados < n) {
        for (u32 j = 0; j < n; ++j) {
            if (Grados[j] > Max_grado_actual && Grados[j] < Max_grado_anterior) {
                Max_grado_actual = Grados[j];
            }
            else if (Grados[j] == Max_grado_anterior) {
                initial_orders[3][indiceM] = j;
                ++indiceM;
                ++agregados;
            }
            
            if (Grados[j] < min_grado_actual && Grados[j] > min_grado_anterior) {
                min_grado_actual = Grados[j];
            }
            else if (Grados[j] == min_grado_anterior && Grados[j] != Max_grado_anterior) {
                initial_orders[3][indicem] = j;
                --indicem;
                ++agregados;
            }
        }
        Max_grado_anterior = Max_grado_actual;
        min_grado_anterior = min_grado_actual;
        Max_grado_actual = 0;
        min_grado_actual = n - 1;
    }
    free(Grados);
   
    // Orden extra: primero divisible por 3 en orden creciente luego el resto en orden creciente.
    num = 0;
    for (u32 i = 0; num < (int)n; ++i){
            initial_orders[4][i] = num;
            num += 3;
    }
    num = 1;
    for (u32 i = ((n % 3 == 0) ? (n/3) : (n/3) + 1); i < n; i++){
        initial_orders[4][i] = num;
        num = (num % 3 == 2) ? num + 2 : num + 1;
    }
}

/*
// Función auxiliar para decidir si un grafo está coloreado adecuadamente
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
*/

int main (){

    // Semilla para generar números aleatorios
    srand(time(NULL));

    // Crear el grafo
    printf("Cargando grafo ...\n");
    Grafo G = ConstruirGrafo();
    if (G == NULL) {
        fprintf(stderr, "ERROR: Fallo al leer el grafo.\n");
        fprintf(stderr, "Cerrando el programa ...\n");
        exit(1);
    }
    printf("Grafo listo.\n");
    u32 num_vert = NumeroDeVertices(G);

    // Array de ordenes iniciales
    u32 *initial_orders[NUM_INITIAL_ORDERS];

    // Pedimos memoria para los ordenes iniciales
    for (u32 i = 0; i < NUM_INITIAL_ORDERS; i++) {
        initial_orders[i] = malloc(num_vert * sizeof(u32));
        if (initial_orders[i] == NULL) {
            fprintf(stderr, "Error: No se pudo asignar memoria para los ordenes iniciales\n");
            return 1;
        }
    }
    // Generar los ordenes iniciales
    printf("Generando ordenes iniciales ...\n");
    generarOrdenesIniciales(G, initial_orders, num_vert);
    printf("Ordenes iniciales listos.\n");

    // Array para guardar los colores obtenidos en la última iteración
    u32 results[NUM_INITIAL_ORDERS];
    // Array para el último coloreo de las iteraciones
    color *ultimosColoreos[NUM_INITIAL_ORDERS];
    for (size_t i = 0; i < NUM_INITIAL_ORDERS; ++i) {
        ultimosColoreos[i] = malloc(num_vert * sizeof(color));
        if (!ultimosColoreos[i]) {
            fprintf(stderr, "ERROR: Fallo al asignar memoria.\n");
            fprintf(stderr, "Cerrando el programa ...\n");
            exit(1);
        }
    }

    // Ejecutar GulDukat y Greedy, y luego ElimGarak y Greedy en cada iteración
    u32 Greddy_result;
    color *Zero = calloc(num_vert, sizeof(color));// Arreglo de 0´s para reinciar los colores del grafo
    if (!Zero) {
        fprintf(stderr, "Error: Fallo al asignar memoria\n");
        return 1;
    }
    for (u32 i = 0; i < NUM_INITIAL_ORDERS; ++i) {
        printf("Numero de colores obtenidos con el orden inicial %u: %u\n", i, Greedy(G, initial_orders[i]));
        for (u32 j = 0; j < NUM_REORDER_ITERATIONS; ++j) {
            if (GulDukat(G, initial_orders[i]) == '1') {
                fprintf(stderr, "ERROR: Fallo GulDukat.\n");
                fprintf(stderr, "Cerrando el programa ...\n");
                exit(1);
            }
            Greddy_result = Greedy(G, initial_orders[i]);
            printf("Iteracion %u GulDukat - Numero de colores obtenidos: %u\n", j + 1,  Greddy_result);
            if (ElimGarak(G, initial_orders[i]) == '1') {
                fprintf(stderr, "ERROR: Fallo ElimGarak.\n");
                fprintf(stderr, "Cerrando el programa ...\n");
                exit(1);
            }
            Greddy_result = Greedy(G, initial_orders[i]);
            printf("Iteracion %u ElimGarak - Numero de colores obtenidos: %u\n", j + 1,  Greddy_result);
        }
        results[i] = Greddy_result;
        ExtraerColores(G, ultimosColoreos[i]);
        ImportarColores(Zero, G);
        printf(DELIMITATION_BAR);
    }

    // Liberamos memoria que ya no uso
    free(Zero);

    // Comparar las 5 corridas y tomar el orden inicial con la menor cantidad de colores
    // Si dos coloreos utilizan la misma cantidad de colores, se deja la primera en el sentido de los ordenes
    u32 min_color = Delta(G) + 1; // En el peor de los casos Greedy usa Delta(G) + 1 colores
    u32 index = 0;
    for (u32 i = 0; i < NUM_INITIAL_ORDERS; ++i) {
        if (results[i] < min_color) {
            min_color = results[i];
            index = i;
        }
    }

    // Importamos el coloreo de menor colores
    ImportarColores(ultimosColoreos[index], G);

    // Liberamos memoria que ya no usamos
    for (u32 i = 0; i < NUM_INITIAL_ORDERS; ++i) {
        free(ultimosColoreos[i]);
    }

    // Liberamos más memoria que ya no usamos
    for (u32 i = 0; i != index && i < NUM_INITIAL_ORDERS; ++i) {
        free(initial_orders[i]);
    }

    // Hacer 500 reordenamientos seguidos de Greedy
    printf("\nResultados de los ordenamientos finales:\n");
    for (u32 i = 0; i < NUM_FINAL_ORDERS_ITERATIONS; ++i) {
        // Elegir al azar GulDukat o ElimGarak con una probabilidad de 50%
        if (rand() % 2 == 0) {
            if (GulDukat(G, initial_orders[index]) == '1') {
                fprintf(stderr, "ERROR: Fallo GulDukat.\n");
                fprintf(stderr, "Cerrando el programa ...\n");
                exit(1);
            }
        } else {
            if (ElimGarak(G, initial_orders[index]) == '1') {
                fprintf(stderr, "ERROR: Fallo ElimGarak.\n");
                fprintf(stderr, "Cerrando el programa ...\n");
                exit(1);
            }
        }
        printf("Iteracion %u - Numero de colores obtenidos: %u\n", i + 1, Greedy(G, initial_orders[index]));
    }
    printf(DELIMITATION_BAR);

    // Liberamos memoria y destruimos el grafo
    free(initial_orders[index]);
    DestruirGrafo(G);

    return 0;
}
