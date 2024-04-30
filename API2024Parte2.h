#ifndef __APIP2_H
#define __APIP2_H

#include "APIG24.h"

struct ColorGroupSt {
    u32 *Vertices; // Arreglo de vértices con el mismo color
    u32 Longitud; // Cantidad de vértices con el mismo color
    color Color;   // Color de los vértices
};

typedef struct ColorGroupSt *ColorGroup;

// EXPLICAR
void ColorGroupInit (ColorGroup CG, Grafo G, color c);

// EXPLICAR
void ColorGroupDestroy (ColorGroup CG);

/**
* @brief Greedy colorea los vértices de un grafo G siguiendo un orden dado en el arreglo Orden.
* @return El número de colores utilizados para colorear los vértices del grafo G según el algoritmo Greedy, o 2^32 − 1 en caso de error
*/
u32 Greedy(Grafo G,u32* Orden);

/**
* @brief Ordena los vértices por bloques de colores, priorizando
* los colores según una regla específica que considera el grado de los vértices.
* @return Esta función retorna un caracter indicando el éxito o fracaso de la operación.
*/
char GulDukat(Grafo G,u32* Orden);

/**
* @brief Ordena los vértices por bloques de colores, priorizando los colores según 
* la cantidad de vértices de cada color. Los colores se ordenan de manera descendente, comenzando por aquellos con menos vértices y excluyendo los colores 1 y 2 que se colocan al final
* @return devuelve un carácter que indique el éxito o fracaso de la operación
*/
char ElimGarak(Grafo G,u32* Orden);


#endif