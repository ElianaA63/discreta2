#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "API2024Parte2.h"

#define MAX_U32 4294967295 //(2^32)-1

// Función auxiliar para verificar si un arreglo es biyectivo. Costo: O(N)
bool verificarBiyectividad(u32* Orden, u32 n) {
    bool *vistos = calloc(n, sizeof(bool));
    if (!vistos) {
        return false; // Error: No se pudo asignar memoria
    }

    for (u32 i = 0; i < n; ++i) {
        if (Orden[i] >= n || vistos[Orden[i]]) {
            free(vistos);
            return false; // Orden no es biyectiva
        }
        vistos[Orden[i]] = true;
    }

    free(vistos);
    return true;
}

// Función para combinar dos subarreglos ordenados arr[l..m] y arr[m+1..r]
static void merge(u32 arr[], u32 l, u32 m, u32 r) {
    u32 n1 = m - l + 1;
    u32 n2 = r - m;

    // Crear arreglos temporales
    u32 L[n1], R[n2];

    // Copiar datos a los arreglos temporales L[] y R[]
    for (u32 i = 0; i < n1; ++i) {
        L[i] = arr[l + i];
    }
    for (u32 j = 0; j < n2; ++j) {
        R[j] = arr[m + 1 + j];
    }

    // Combinar los arreglos temporales de vuelta en arr[l..r]
    u32 i = 0; // Índice inicial del primer subarreglo
    u32 j = 0; // Índice inicial del segundo subarreglo
    u32 k = l; // Índice inicial del arreglo combinado resultante
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }

    // Copiar los elementos restantes de L[], si hay alguno
    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }

    // Copiar los elementos restantes de R[], si hay alguno
    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

// Función que implementa Merge Sort
static void mergeSort(u32 arr[], u32 l, u32 r) {
    if (l < r) {
        // Encuentra el punto medio
        u32 m = l + (r - l) / 2;

        // Ordena las primeras y segundas mitades
        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        // Combina las mitades ordenadas
        merge(arr, l, m, r);
    }
}

/* La idea es la siguiente: Primero corroboramos que Orden sea efectivamente una biyección, esto nos cuesta O(N). Antes 
de entrar al bucle principal del algoritmo, las dos llamadas a calloc nos cuestan O(Grado(G)) + O(N) respectivamente.
Una vez dentro del bucle, para cada vértice primero obtenemos el arreglo con los colores usados por sus vecinos que es 
O(Grado(v, G)), luego decidimos el color que va a tener v que es O(Grado(v, G)) y reseteamos a 0 el arreglo de colores usados
para el siguiente vértice lo cual también nos cuesta O(Grado(v, G)). TOTAL: O(M) por el lema del apretón de manos.
Por lo tanto la complejidad computacional de Greedy está dada por: O(N) +  O(M) = O(M) */

u32 Greedy(Grafo G, u32* Orden) {
    u32 num_vert = NumeroDeVertices(G);

    // Verificar que Orden provea un orden válido de los elementos {0, 1, ..., n − 1}
    if (!verificarBiyectividad(Orden, num_vert)) {
        return MAX_U32; // Error: Orden no es biyectiva
    }

    // Colorear los vértices siguiendo el orden dado en el arreglo Orden
    u32 v; // Indice del vértice a colorear
    u32 num_vec; // Cantidad de vecinos de v
    color Max_color = 0; // Color máximo utilizado
    bool *colores_usados = calloc(Delta(G), sizeof(bool)); // Arreglo con los colores usados de los vecinos de v
    if (!colores_usados) {
        return MAX_U32; // No se pudo asignar memoria
    }
    color *Colores = calloc(num_vert, sizeof(color)); // Arreglo para guardar los colores del grafo
    if (!Colores) {
        free(colores_usados);
        return MAX_U32; // No se pudo asignar memoria
    }
    for (u32 i = 0; i < num_vert; ++i) {
        v = Orden[i];
        num_vec = Grado (v, G);

        // Rellenamos colores_usados
        for (u32 j = 0; j < num_vec; ++j) {  
            u32 w = Vecino(j, v, G); // Variable para los vecinos de v
            if (Colores[w] == 0) { 
                continue; // Si el color del vecino w es 0, no lo contamos
            }
            colores_usados[Colores[w] - 1] = true;
        }

        // Obtenemos el color de v y actualizamos el color máximo
        u32 k = 0;
        while (colores_usados[k]) {
            ++k;
        }
        Colores[v] = k + 1;
        Max_color = (Colores[v] > Max_color) ? Colores[v] : Max_color; // Actualizo el color máximo

        // Volvemos a false
        for (u32 l = 0; l < num_vec; ++l) {
            colores_usados[l] = false;
        }
    }
    ImportarColores(Colores, G);
    free(colores_usados);
    free(Colores);
    return Max_color;
}


char GulDukat(Grafo G, u32* Orden) {
    u32 num_vertices = NumeroDeVertices(G);
    u32 num_colores = NumeroDeLados(G); // Suponiendo que los colores van desde 1 hasta el número de lados

    // Definir las funciones m(x) y M(x)
    u32* m = malloc(num_colores * sizeof(u32));
    u32* M = malloc(num_colores * sizeof(u32));
    if (m == NULL || M == NULL) {
        fprintf(stderr, "No se pudo asignar memoria");
        free(m);
        free(M);
        return 1; // Error: Fallo al asignar memoria temporal
    }

    // Inicializar las funciones m(x) y M(x)
    for (u32 x = 0; x < num_colores; x++) {
        m[x] = Delta(G); // Inicializar con un valor grande
        M[x] = 0; // Inicializar con un valor pequeño
    }

    // Calcular las funciones m(x) y M(x)
    for (u32 i = 0; i < num_vertices; i++) {
        color c = Color(i, G);
        u32 grado = Grado(i, G);
        if (grado < m[c]) {
            m[c] = grado;
        }
        if (grado > M[c]) {
            M[c] = grado;
        }
    }

    // Ordenar los colores según el criterio especificado usando Merge Sort
    u32* colores_ordenados = malloc(num_colores * sizeof(u32));
    if (colores_ordenados == NULL) {
        fprintf(stderr, "No se pudo asignar memoria");
        free(m);
        free(M);
        return 1; // Error: Fallo al asignar memoria temporal
    }

    // Inicializar los colores ordenados
    for (u32 i = 0; i < num_colores; i++) {
        colores_ordenados[i] = i;
    }

    // Ordenar los colores según el criterio especificado usando Merge Sort
    mergeSort(colores_ordenados, 0, num_colores - 1);

    // Llenar el arreglo Orden con los vértices ordenados según los colores
    u32 indice = 0;

    // Primero, ponemos los vértices de colores divisibles por 4, ordenados según M(x)
    for (u32 i = 0; i < num_colores; i++) {
        color c = colores_ordenados[i];
        if (c % 4 == 0) {
            for (u32 j = 0; j < num_vertices; j++) {
                if (Color(j, G) == c) {
                    Orden[indice++] = j;
                }
            }
        }
    }

    // Luego, ponemos los vértices de colores pares no divisibles por 4, ordenados según M(x) + m(x)
    for (u32 i = 0; i < num_colores; i++) {
        color c = colores_ordenados[i];
        if (c % 2 == 0 && c % 4 != 0) {
            for (u32 j = 0; j < num_vertices; j++) {
                if (Color(j, G) == c) {
                    Orden[indice++] = j;
                }
            }
        }
    }

    // Finalmente, ponemos los vértices de colores impares, ordenados según m(x)
    for (u32 i = 0; i < num_colores; i++) {
        color c = colores_ordenados[i];
        if (c % 2 != 0) {
            for (u32 j = 0; j < num_vertices; j++) {
                if (Color(j, G) == c) {
                    Orden[indice++] = j;
                }
            }
        }
    }

    free(m);
    free(M);
    free(colores_ordenados);
    return 0;
}

// Función para ordenar los vértices según los criterios especificados
char ElimGarak(Grafo G, u32* Orden) {
    u32 num_vertices = NumeroDeVertices(G);
    u32 num_colores = NumeroDeLados(G); 

    // Crear un arreglo de colores ordenados según los criterios especificados
    u32* colores_ordenados = malloc(num_colores * sizeof(u32));
    if (colores_ordenados == NULL) {
        fprintf(stderr, "No se pudo asignar memoria para colores_ordenados\n");
        return 1; // Error: Fallo al asignar memoria temporal
    }

    // Llenar el arreglo con los colores según los criterios especificados
    u32 indice = 0;
    for (u32 i = num_colores; i >= 3; i--) { // Comenzar desde el tercer color (excluyendo 1 y 2)
        colores_ordenados[indice++] = i;
    }
    colores_ordenados[indice++] = 2; // Agregar el color 2 al final
    colores_ordenados[indice++] = 1; // Agregar el color 1 al final

    // Ordenar los colores según la cantidad de vértices utilizando Merge Sort
    mergeSort(colores_ordenados, 0, num_colores - 1);

    // Llenar el arreglo Orden con los vértices ordenados según los colores
    indice = 0;
    for (u32 i = 0; i < num_colores; i++) {
        color c = colores_ordenados[i];
        for (u32 j = 0; j < num_vertices; j++) {
            if (Color(j, G) == c) {
                Orden[indice++] = j;
            }
        }
    }

    // Liberar memoria
    free(colores_ordenados);
    return 0;
}
