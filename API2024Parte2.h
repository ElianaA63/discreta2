#ifndef __APIP2_H
#define __APIP2_H

#include "APIG24.h"

#define TOLERANCIA 30 // Vértices extra que puedo tolerar en un grupo de colores (cobra sentido en grafos pequeños)

struct ColorGroupSt {
    u32 *Vertices; // Arreglo de vértices con el mismo color
    u32 Longitud; // Cantidad de vértices con el mismo color
    color Color;   // Color de los vértices
};

typedef struct ColorGroupSt *ColorGroup;

/**
* @brief Función para crear una estructura del tipo ColorGroupSt
* @note Nosotros tomamos un tope para los grupos de colores el cual no va a ser cierto para todo grafo debido a problemas 
* de memoria. Podríamos sacrificar complejidad computacional para fijar el tamaño exacto de cada grupo pero eso se escapa de las 
* limitaciones de este laboratorio
* @param CG Grupo de colores a inicializar
* @param G Grafo
* @param c Color del grupo de colores
*/
void ColorGroupInit (ColorGroup CG, Grafo G, color c);

/**
* @brief Función para destuir una estructura del tipo ColorGroupSt
* @param CG Grupo de colores a inicializar
*/
void ColorGroupDestroy (ColorGroup CG);

/**
* @brief Greedy colorea los vértices de un grafo G siguiendo Orden
* @note Esta función no asume que Orden sea biyectivo, pero si que apunta a una sección de memoria con n lugares
* @param G Grafo
* @param Orden Arreglo con las posiciones de los vértices en base al orden en que se deben colorear
* @return El número de colores utilizados para colorear los vértices del grafo G según el algoritmo Greedy, o 2^32 − 1 en caso de error
*/
u32 Greedy(Grafo G,u32* Orden);

/**
* @brief Ordena los vértices por bloques de colores, priorizando aquellos que tengan más vertices.
* Los vértices con colores 2 y 1 se ponen al final independientemente de cuantos sean
* @param G Grafo
* @param Orden Arreglo con las posiciones de los vértices en base al orden en que se deben colorear
* @return Esta función retorna un caracter indicando el éxito(0) o fracaso(1) de la operación.
*/
char ElimGarak(Grafo G,u32* Orden);


/**
* @brief Ordena los vértices en base a si su color es divisible por 4 y luego en base a sus grados tomando el máximo
* de mayor a menor. Después continúa con los que tengan color par pero no divisible por 4 en base a sus grados pero tomando
* el mínimo y también de mayor a menor. Finalmente coloca los vértices cuyo color sea impar en base a sus grados pero esta
* vez tomando la suma entre el máximo grado de ese color y el mínimo grado de ese color, igualmente de mayor a menor
* @param G Grafo
* @param Orden Arreglo a modificar con las posiciones de los vértices en base al orden en que se deben colorear
* @return Esta función retorna un caracter indicando el éxito(0) o fracaso(1) de la operación.
*/
char GulDukat(Grafo G,u32* Orden);

#endif