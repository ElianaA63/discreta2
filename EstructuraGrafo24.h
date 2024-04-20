#ifndef ESTRUCTURAGRAFO24_H
#define ESTRUCTURAGRAFO24_H

typedef unsigned int u32;
typedef unsigned int color;
typedef struct vertice_t *vertice;
typedef struct vecindario_t *vecindario;

#define MAX_U32 4294967295 //(2^32)-1

/**
 * Estructura del grafo
 */
struct GrafoSt {
    u32 vertices_num;       // número de vértices del grafo(n)
    u32 lados_num;         // número de aristas del grafo(m)
    u32 delta;            // delta -> mayor grado de los vértices
    vertice *vertices;   // arreglo con los vértices del grafo
};

/**
 * Estructura del vertice
 */
struct vertice_t {
    u32 id;                  // identificador del vértice
    color color;            // color
    u32 grado;             // número de vecinos
    vecindario vecinos;   // vértices vecinos
};

/**
 *Estructura del vecino
 */
struct vecindario_t {
    vertice *residentes;   // arreglo dinámico de vecinos
    u32 capacidad;        // capacidad máxima de vecinos
};

#endif


