#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h> //void *memset(void *ptr, int value, size_t num);

#include "API2024Parte2.h"

#define MAX_U32 4294967295 //(2^32)-1

static int menorQue_Colores(const void *c1, const void *c2) {
    const color color1 = *(const color *)c1;
    const color color2 = *(const color *)c2;

    if (color1 < color2) {
        return -1;
    } else if (color1 == color2) {
        return 0;
    } else {
        return 1;
    }
}

// Asumimos que se empieza a colorear desde el 1
static color getColor (color* colores_vec, u32 n) {
    u32 i = 0;
    while (i < n-1) {
        if (colores_vec[i] == colores_vec[i+1] || colores_vec[i] + 1 == colores_vec[i+1]) {
            ++i;
        }
        else
        {
            return colores_vec[i] + 1;
        }
    }
    return colores_vec[i] + 1; // Tengo que usar un nuevo color
}

/* La idea es la siguiente: Primero corroboramos que Orden sea efectivamente una biyección. Esto nos cuesta O(N^2).
Después, para cada vértice, primero obtenemos el arreglo de sus vecinos que es O(Grado(v, G)),
luego ordenamos el arrelgo utilizando el algoritmo quicksort, que es O(N * log(N)) en caso promedio), para poder
recorrer de forma más eficiente los vértices. Finalmente, recorremos el arreglo de vecinos y obtenemos el color de
v, estos nos cuesta O(Grado(v, G)). Por lo tanto la complejidad computacional de Greedy está dada por:
O(N^2) + O(N)* (O(Grado(v, G)) + O(N * log(N)) + O(Grado(v, G))) = O(N^2 * log(N)) */

u32 Greedy(Grafo G, u32* Orden) {
    u32 num_vert = NumeroDeVertices(G);

    // Verificar que Orden provea un orden válido de los elementos {0, 1, ..., n − 1}
    for (u32 i = 0; i < num_vert; ++i) {
        // Chequea que esté dentro del rango [0, n-1]
        if (Orden[i] >= num_vert) {
            return MAX_U32; // Error: Orden no es sobreyectiva
        }
        // Chequea que no haya un repetido
        for (u32 j = 0; j < num_vert; ++j) {
            if (i != j && Orden[i] == Orden[j]) {
                return MAX_U32; // Error: Orden no es inyectiva
            }
        }   
    }

    // Colorear los vértices siguiendo el orden dado en el arreglo Orden
    u32 v; // Indice del vértice a colorear
    u32 num_vec; // Cantidad de vecinos de v
    color *colores_vec = NULL; // Arreglo con los colores de los vecinos de v
    color *Colores = malloc(sizeof(color) * num_vert); // Arreglo para guardar los colores del grafo
    if (!Colores) {
        return MAX_U32; // No se pudo asignar memoria
    }
    
    for (u32 i = 0; i < num_vert; ++i) {
        v = Orden[i];
        num_vec = Grado (v, G);
        colores_vec = realloc(colores_vec, sizeof(color) * num_vec);
        if (!colores_vec) {
        free(Colores);
        return MAX_U32; // No se pudo asignar memoria
        }
        // Rellenamos colores_vec
        for (u32 j = 0; j < num_vec; ++j)
        {
            colores_vec[j] = Color(Vecino(j, v, G), G);
        }
        // Ordenamos colores_vec
        qsort (colores_vec, num_vec, sizeof(color), menorQue_Colores);
        // Obtenemos el color de v y lo agregamos al grafo
        ExtraerColores(G, Colores);
        Colores[v] = getColor(colores_vec, num_vec);
        ImportarColores(Colores, G);
    }
    free(colores_vec);
    //Obtenemos la cantidad de colores utilizados que va a ser igual al color más grande que se use
    color Max_color = 0;
    for (u32 i = 0; i < num_vert; ++i) {
        if (Colores[i] > Max_color) {
            Max_color = Colores[i];
        } 
    }

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

    // Ordenar los colores según el criterio especificado
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

    // Ordenar los colores según el criterio especificado
    qsort(colores_ordenados, num_colores, sizeof(u32), menorQue_Colores);

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

// Función auxiliar para contar la cantidad de vértices por color, excluyendo los colores 1 y 2
void contarVerticesPorColor(u32* vertices_por_color, Grafo G) {
    u32 num_vertices = NumeroDeVertices(G);
    for (u32 i = 0; i < num_vertices; i++) {
        color c = Color(i, G);
        if (c > 2) { // Solo contar vértices de colores distintos de 1 y 2
            vertices_por_color[c - 3]++; // Ajustar el índice para excluir los colores 1 y 2
        }
    }
}

// Función para comparar colores en el ordenamiento
static int comparar_colores(const void *c1, const void *c2) {
    const u32* color1 = (const u32 *)c1;
    const u32* color2 = (const u32 *)c2;

    // Obtener el número de vértices por color
    u32 num_vertices_color1 = *color1;
    u32 num_vertices_color2 = *color2;

    if (num_vertices_color1 < num_vertices_color2) {
        return -1;
    } else if (num_vertices_color1 > num_vertices_color2) {
        return 1;
    } else {
        // Si tienen la misma cantidad de vértices, desempatar utilizando el número de color
        return (*(color1 + 1)) - (*(color2 + 1));
    }
}

// Función para ordenar los vértices según los criterios especificados
char ElimGarak(Grafo G, u32* Orden) {
    u32 num_vertices = NumeroDeVertices(G);
    u32 num_colores = NumeroDeLados(G); 

    // Contar la cantidad de vértices de cada color, excluyendo los colores 1 y 2
    u32* vertices_por_color = calloc(num_colores - 2, sizeof(u32));
    if (vertices_por_color == NULL) {
        fprintf(stderr, "No se pudo asignar memoria para vertices_por_color\n");
        return 1; // Error: Fallo al asignar memoria temporal
    }
    contarVerticesPorColor(vertices_por_color, G);

    // Crear un arreglo de colores ordenados según la cantidad de vértices
    u32* colores_ordenados = malloc((num_colores - 2) * sizeof(u32));
    if (colores_ordenados == NULL) {
        fprintf(stderr, "No se pudo asignar memoria para colores_ordenados\n");
        free(vertices_por_color);
        return 1; // Error: Fallo al asignar memoria temporal
    }
    for (u32 i = 0; i < num_colores - 2; i++) { // Comenzar desde el tercer color (excluyendo 1 y 2)
        colores_ordenados[i] = i + 3;
    }
    // Ordenar los colores según la cantidad de vértices
    qsort(colores_ordenados, num_colores - 2, sizeof(u32), comparar_colores);

    // Llenar el arreglo Orden con los vértices ordenados según los colores
    u32 indice = 0;
    for (u32 i = 0; i < num_colores - 2; i++) { // Comenzar desde el tercer color (excluyendo 1 y 2)
        color c = colores_ordenados[i];
        for (u32 j = 0; j < num_vertices; j++) {
            if (Color(j, G) == c) {
                Orden[indice++] = j;
            }
        }
    }

    // Colocar los vértices del color 2
    for (u32 i = 0; i < num_vertices; i++) {
        if (Color(i, G) == 2) {
            Orden[indice++] = i;
        }
    }

    // Colocar los vértices del color 1
    for (u32 i = 0; i < num_vertices; i++) {
        if (Color(i, G) == 1) {
            Orden[indice++] = i;
        }
    }

    // Liberar memoria
    free(vertices_por_color);
    free(colores_ordenados);
    return 0;
}