#ifndef APIG24_H
#define APIG24_H


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


//el .h de abajo debe tener definida GrafoSt, u32 y color
#include "EstructuraGrafo24.h"

//Grafo es un puntero a una estructura GrafoSt, la cual debe estar definida en el .h de arriba

typedef struct GrafoSt* Grafo;

//Como decidimos utilizar arreglos dinámicos, necesitamos un tamaño incial para el arreglo
#define VECINADARIO_CAPACIDAD_INICIAL 25

//Funciones de construcción y destrucción

/**
* @brief Lee un grafo desde standard input y lo carga en la estructura GrafoSt
* @return Un puntero a la estructura GrafoSt si termina exitosamente o NULL en caso contrario
*/
Grafo ConstruirGrafo();
 
/**
* @brief Destruye G y libera la memoria alocada
* @param G puntero a la estructura GrafoSt
*/
void DestruirGrafo(Grafo G);

/**
* @brief Función auxiliar que agrega a v como vecino de u y actualiza su grado
* @param v vertice vecino
* @param u vertice al cual se le aumenta el vecindario
* @return True si la operación finalizó exitosamente, False en caso contario
*/
bool AgregarVecino(vertice v, vertice u);

//funciones para extraer datos del grafo
//u32 debe estar definida en el .h de arriba

/**
* @brief Devuelve el número de vértices de G
* @param G puntero a la estructura GrafoSt
* @return Cantidad de vértices de G
*/
u32 NumeroDeVertices(Grafo G);

/**
* @brief Devuelve el número de lados de G
* @param G puntero a la estructura GrafoSt
* @return Cantidad de lados de G
*/
u32 NumeroDeLados(Grafo G);

/**
* @brief Devuelve el mayor grado que tenga un vértice en G
* @param G puntero a la estructura GrafoSt
* @return Grado del vértice con más lados en G
*/
u32 Delta(Grafo G);


//funciones de extraccion de informacion de vertices 

/**
* @brief Devuelve el grado del i-ésimo vértice
* @param i posición del vértice en G
* @param G puntero a la estructura GrafoSt
* @return Grado del i-ésimo vértice o 0 si i no está comprendido entre 0 y n-1
*/
u32 Grado(u32 i, Grafo G);

/**
* @brief Devuelve el color del i-ésimo vértice
* @param i posición del vértice en G
* @param G puntero a la estructura GrafoSt
* @return Color del i-ésimo vértice o MAX_U32 si i no está comprendido entre 0 y n-1
*/
color Color(u32 i, Grafo G);

/**
* @brief Devuelve el j-ésimo vecino del i-ésimo vértice
* @param i posición del vértice en G
* @param j posición del vécino en el i-ésimo vértice
* @param G puntero a la estructura GrafoSt
* @return La posición del j-ésimo vecino del i-ésimo vértice en G
ó MAX_U32 si i no está comprendido entre 0 y n-1  o j no está comprendido entre 0 y Grado(i, G) -1 
*/
u32 Vecino(u32 j, u32 i, Grafo G);

//funciones con colores

/**
* @brief Si i es menor que el número de vértices, esta función asigna el color x al vértice i 
Caso contrario, no hace nada
* @param x un color
* @param i posición del vértice en G
* @param G puntero a la estructura GrafoSt
*/
void AsignarColor(color x, u32 i, Grafo  G);

/**
* @brief Copia el arreglo con los colores de los vértices de G en Color
* @param G puntero a la estructura GrafoSt
* @param Color areglo de colores
*/
void ExtraerColores(Grafo G, color* Color);

/**
* @brief Copia a Color en el arreglo con los colores de los vértices de G
* @param Color areglo de colores
* @param G puntero a la estructura GrafoSt
*/
void ImportarColores(color* Color, Grafo  G);


#endif
