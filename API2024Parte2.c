#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h> //void *memset(void *ptr, int value, size_t num);

#include "API2024Parte2.h"

#define MAX_U32 4294967295 //(2^32)-1

/*La función memset() rellena los primeros n bytes del área de memoria 
apuntada por s con el byte constante c*/

u32 Greedy(Grafo G, u32* Orden) {
    u32 num_vert = NumeroDeVertices(G);
    u32 num_colores = 1;

    // Verificar que Orden provea un orden válido de los elementos {0, 1, ..., n − 1}
    for (u32 i = 0; i < num_vert; i++) {
        if (Orden[i] != i) {
            return MAX_U32; // Error: Orden no induce una biyección
        }
    }

    u32* colores_usados = calloc(num_vert, sizeof(u32));
    if (colores_usados == NULL) {
        fprintf(stderr, "No se pudo asignar memoria");
        return MAX_U32 ; // Error: Fallo al asignar memoria temporal
    }

    // Inicializar el arreglo de colores con el valor máximo para indicar que ningún vértice tiene asignado un color
    u32* Color = calloc(num_vert, sizeof(u32));
    if (Color == NULL) {
        fprintf(stderr, "No se pudo asignar memoria");
        free(colores_usados);
        return MAX_U32; // Error: Fallo al asignar memoria temporal
    }

    // Colorear los vértices siguiendo el orden dado en el arreglo Orden
    for (u32 i = 0; i < num_vert; i++) {
        u32 k = Orden[i]; // Obtener el vértice en la posición i del orden
        u32 grado = Grado(k, G);

        // Marcar los colores usados por los vecinos de k
        memset(colores_usados, 0, num_colores * sizeof(u32));
        for (u32 j = 0; j < grado; j++) {
            u32 vecino_k = Vecino(j, k, G);
            if (vecino_k < num_vert && Color[vecino_k] < num_colores) {
                colores_usados[Color[vecino_k]] = 1;
            }
        }

        // Asignar el primer color disponible a k
        for (u32 color_k = 0; color_k < num_colores; color_k++) {
            if (colores_usados[color_k] == 0) {
                Color[k] = color_k;
                break;
            }
        }

        // Si no se encontró un color disponible, asignar uno nuevo
        if (Color[k] == MAX_U32) {
            Color[k] = num_colores;
            num_colores++;
        }
    }

    free(colores_usados);
    free(Color);
    
    return num_colores;
}

void swap(u32* a, u32* b) {
    u32 temp = *a;
    *a = *b;
    *b = temp;
}

u32 partition(u32* colores_ordenados, u32* M, u32* m, u32* indices, u32 low, u32 high) {
    u32 pivot = colores_ordenados[high];
    u32 i = low - 1;

    for (u32 j = low; j < high; j++) {
        if ((colores_ordenados[j] % 4 == 0 && pivot % 4 == 0) ||
            (colores_ordenados[j] % 2 == 0 && colores_ordenados[j] % 4 != 0 && pivot % 2 == 0 && pivot % 4 != 0) ||
            (colores_ordenados[j] % 2 != 0 && pivot % 2 != 0)) {
            if (M[colores_ordenados[j]] > M[pivot]) {
                i++;
                swap(&indices[i], &indices[j]);
                swap(&colores_ordenados[i], &colores_ordenados[j]);
            } else if (M[colores_ordenados[j]] == M[pivot] && m[colores_ordenados[j]] > m[pivot]) {
                i++;
                swap(&indices[i], &indices[j]);
                swap(&colores_ordenados[i], &colores_ordenados[j]);
            }
        }
    }

    swap(&indices[i + 1], &indices[high]);
    swap(&colores_ordenados[i + 1], &colores_ordenados[high]);

    return i + 1;
}

void quickSort(u32* colores_ordenados, u32* M, u32* m, u32* indices, u32 low, u32 high) {
    if (low < high) {
        u32 pi = partition(colores_ordenados, M, m, indices, low, high);

        if (pi > 0) {
            quickSort(colores_ordenados, M, m, indices, low, pi - 1);
        }

        quickSort(colores_ordenados, M, m, indices, pi + 1, high);
    }
}

char GulDukat(Grafo G, u32* Orden) {
    u32 num_vertices = NumeroDeVertices(G);
    u32 num_colores = NumeroDeLados(G); // Suponiendo que los colores van desde 1 hasta el número de lados
    u32* vertices_por_color = calloc(num_colores, sizeof(u32));
    if (vertices_por_color == NULL) {
        fprintf(stderr, "No se pudo asignar memoria");
        return 1; // Error: Fallo al asignar memoria temporal
    }

    // Contar la cantidad de vértices de cada color
    for (u32 i = 0; i < num_vertices; i++) {
        color c = Color(i, G);
        vertices_por_color[c]++;
    }

    // Definir las funciones m(x) y M(x)
    u32* m = malloc(num_colores * sizeof(u32));
    u32* M = malloc(num_colores * sizeof(u32));

    for (u32 x = 0; x < num_colores; x++) {
        m[x] = Delta(G); // Inicializar con un valor grande
        M[x] = 0; // Inicializar con un valor pequeño
    }

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

    // Ordenar los colores según la especificación
    u32* colores_ordenados = malloc(num_colores * sizeof(u32));
    if (colores_ordenados == NULL) {
        fprintf(stderr, "No se pudo asignar memoria");
        free(vertices_por_color);
        free(m);
        free(M);
        return 1; // Error: Fallo al asignar memoria temporal
    }

    // Inicializar los colores ordenados
    for (u32 i = 0; i < num_colores; i++) {
        colores_ordenados[i] = i;
    }

    // Ordenar los colores usando QuickSort
    quickSort(colores_ordenados, M, m, colores_ordenados, 0, num_colores - 1);

    // Llenar el arreglo Orden con los vértices ordenados según los colores
    u32 indice = 0;

    for (u32 i = 0; i < num_colores; i++) {
        color c = colores_ordenados[i];
        for (u32 j = 0; j < num_vertices; j++) {
            if (Color(j, G) == c) {
                Orden[indice++] = j;
            }
        }
    }

    free(vertices_por_color);
    free(m);
    free(M);
    free(colores_ordenados);
    return 0;
}

// Función auxiliar para contar la cantidad de vértices por color, excluyendo los colores 1 y 2
void contarVerticesPorColor(u32* vertices_por_color, u32 num_colores, Grafo G) {
    u32 num_vertices = NumeroDeVertices(G);
    for (u32 i = 0; i < num_vertices; i++) {
        color c = Color(i, G);
        if (c != 1 && c != 2) {
            vertices_por_color[c - 3]++;
        // Los colores se ajustan restando 3 para que comiencen desde 0
        }
    }
}

// Función para ordenar los vértices según los criterios especificados
char ElimGarak(Grafo G, u32* Orden) {
    u32 num_vertices = NumeroDeVertices(G);
    u32 num_colores = NumeroDeLados(G); // Suponiendo que los colores van desde 1 hasta el número de lados
    u32* vertices_por_color = calloc(num_colores - 2, sizeof(u32)); // Excluyendo los colores 1 y 2
    if (vertices_por_color == NULL) {
        fprintf(stderr, "No se pudo asignar memoria");
        return 1; // Error: Fallo al asignar memoria temporal
    }

    // Contar la cantidad de vértices de cada color, excluyendo los colores 1 y 2
    contarVerticesPorColor(vertices_por_color, num_colores, G);

    // Ordenar los colores según la cantidad de vértices, de menor a mayor
    u32* colores_ordenados = malloc((num_colores - 2) * sizeof(u32));
    if (colores_ordenados == NULL) {
        fprintf(stderr, "No se pudo asignar memoria");
        free(vertices_por_color);
        return 1; // Error: Fallo al asignar memoria temporal
    }

    // Inicializar los colores ordenados
    for (u32 i = 0; i < num_colores - 2; i++) {
        colores_ordenados[i] = i + 3; // Ajustar los colores para que comiencen desde 3
    }

    // Ordenar los colores según la cantidad de vértices
    for (u32 i = 0; i < num_colores - 2; i++) {
        for (u32 j = i + 1; j < num_colores - 2; j++) {
            if (vertices_por_color[colores_ordenados[i] - 3] > vertices_por_color[colores_ordenados[j] - 3]) {
                // Swap
                u32 temp = colores_ordenados[i];
                colores_ordenados[i] = colores_ordenados[j];
                colores_ordenados[j] = temp;
            }
        }
    }

    // Llenar el arreglo Orden con los vértices ordenados según los colores
    u32 indice = 0;

    // Colocar los vértices del color con menos vértices primero
    for (u32 i = 0; i < num_colores - 2; i++) {
        color c = colores_ordenados[i];
        for (u32 j = 0; j < num_vertices; j++) {
            if (Color(j, G) == c) {
                Orden[indice] = j;
                indice++;
            }
        }
    }

    // Colocar los vértices del color 2
    for (u32 i = 0; i < num_vertices; i++) {
        if (Color(i, G) == 2) {
            Orden[indice] = i;
            indice++;
        }
    }

    // Colocar los vértices del color 1
    for (u32 i = 0; i < num_vertices; i++) {
        if (Color(i, G) == 1) {
            Orden[indice] = i;
            indice++;
        }
    }

    free(vertices_por_color);
    free(colores_ordenados);
    return 0;
}
