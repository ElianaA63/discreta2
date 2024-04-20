#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "APIG24.h"

//Funciones de construcción y destrucción
Grafo ConstruirGrafo(){

    Grafo G = (Grafo)malloc(sizeof(struct GrafoSt));

    if(!G){
        return NULL;
    } //no se pudo asignar memoria para el grafo

    //Inicializar el grafo 
    G->vertices_num= 0;
    G->lados_num=0;
    G->delta = 0;
    G->vertices = NULL;

    //comienzo de lectura del archivo, buscando lineas que empiecen con 'c'.
    char buffer[255];
    while(fgets(buffer,255, stdin) != NULL && buffer[0]=='c');

    //leo encabezado del grafo
    if(sscanf(buffer, "p edge %d %d", &G->vertices_num, &G->lados_num) !=2 || G->lados_num == 0){
        free(G);
        return NULL;
    } // FORMATO DE ENTRADA INCORRECTO

    //reservar memoria para el struct de vertice
    G->vertices = (vertice *)malloc(G->vertices_num * sizeof(struct vertice_t));

    if(!G->vertices){
        free(G);
        return NULL;
    } //no se pudo asignar memoria para los vértices

    // Inicializar los vértices y los vecindarios
    for (u32 i = 0; i < G->vertices_num; ++i) {

        G->vertices[i] = (vertice)malloc(sizeof(struct vertice_t));

        if (!G->vertices[i]) {
            DestruirGrafo(G);
            return NULL;
        } //no se pudo asignar memoria para el vértice

        G->vertices[i]->vecinos = (vecindario)malloc(sizeof(struct vecindario_t));

        if (!G->vertices[i]->vecinos) {
            DestruirGrafo(G);  
            return NULL;
        } //no se pudo asignar memoria para los vecinos

        G->vertices[i]->vecinos->residentes = (vertice *)malloc(VECINADARIO_CAPACIDAD_INICIAL * sizeof(vertice));

        if (!G->vertices[i]->vecinos->residentes) {
            DestruirGrafo(G); 
            return NULL;
        } //no se pudo asignar memoria para los residentes

        G->vertices[i]->vecinos->capacidad = VECINADARIO_CAPACIDAD_INICIAL;
        G->vertices[i]->color = 0;
        G->vertices[i]->grado = 0;
    }

    // Leer los lados del grafo
    u32 i = 0;
    u32 u, v;
    while (i < G->lados_num){

        if (scanf(" e %u %u ", &u, &v) != 2) {
            DestruirGrafo(G);  
            return NULL;
        } // Error: formato de entrada incorrecto

        //Si el grado de un vértice es 0, es porque es la primera vez que aparece
        if (G->vertices[v]->grado == 0){
            G->vertices[v]->id = v;
        }
        if (G->vertices[u]->grado == 0){
            G->vertices[u]->id = u;
        }

        // Agregar los vecinos a sus respectivos vecindarios y actualizar el delta
        if (!AgregarVecino(G->vertices[v], G->vertices[u]) || !AgregarVecino(G->vertices[u], G->vertices[v])){
            DestruirGrafo(G);  
            return NULL;
        } // Error: no se pudo reasignar memoria para algún vecinadrio

        if (G->vertices[v]->grado > G->delta){
            G->delta =  G->vertices[v]->grado;
        }
        else if (G->vertices[u]->grado > G->delta){
                G->delta =  G->vertices[u]->grado;
            }
        ++i;
    }

    if (i != G->lados_num) {
        DestruirGrafo(G); 
        return NULL;
    } // Error: No hay al menos m lados
     
    return G;
}

bool AgregarVecino(vertice v, vertice u){

    if (u->vecinos->capacidad == u->grado){
        u->vecinos->residentes = (vertice *)realloc(u->vecinos->residentes, 2 * u->vecinos->capacidad * sizeof(vertice)); 
        
        if (!u->vecinos->residentes) {
            return false;
        } // no se pudo redimensionar el arreglo

        u->vecinos->capacidad = 2 * u->vecinos->capacidad;
    }

    u->vecinos->residentes[u->grado] = v;
    ++u->grado;
    return true;
}

//Función auxiliar para liberar la memoria de un vecindario
static void destruir_vecinos(vecindario vecinos) {
    if (vecinos != NULL) {
        free(vecinos->residentes);
        vecinos->residentes = NULL;
        free(vecinos);
        vecinos = NULL;
    }
}

//Función auxiliar para liberar la memoria de un vertice
static void destruir_vertice(vertice vert) {
    if (vert != NULL) {
        destruir_vecinos(vert->vecinos);
        free(vert);
        vert = NULL;
    }
}

void DestruirGrafo(Grafo G) {
    if (G != NULL) {
        if (G->vertices != NULL) {
            for (u32 i = 0; i < G->vertices_num; ++i) {
                if (G->vertices[i] != NULL) {
                    destruir_vertice(G->vertices[i]);
                }
            }
            free(G->vertices);
            G->vertices = NULL;
        }
        free(G);
        G = NULL;
    }
}

//funciones para extraer datos del grafo.

u32 NumeroDeVertices(Grafo G){
    return G->vertices_num;
}

u32 NumeroDeLados(Grafo G){
    return G->lados_num;
}

u32 Delta(Grafo G){
    return G->delta;
}

//funciones de extraccion de informacion de vértices 

u32 Grado(u32 i, Grafo G){
    u32 result = 0;
    if (i < G->vertices_num) {
        result = G->vertices[i]->grado;
    }
    return result;
}

color Color(u32 i, Grafo G){
    u32 result = MAX_U32;
    if (i < G->vertices_num) {
        result = G->vertices[i]->color;
    }
    return result;
}

u32 Vecino(u32 j ,u32 i, Grafo G){
    if ( j >= G->vertices[i]->grado || i >= G->vertices_num) {
        return MAX_U32;
    }
    return G->vertices[i]->vecinos->residentes[j]->id;
}

//funciones con colores

void AsignarColor(color x, u32 i, Grafo  G){
    // Verificar si i es menor que el número de vértices            
    if (i < G->vertices_num) {
        G->vertices[i]->color = x;
    }
}

void ExtraerColores(Grafo G, color* Color){
    // Iterar sobre todos los vértices del grafo
    for (u32 i = 0; i < G->vertices_num; i++) {
        // Asignar el color del vértice i al array Color en la posición i
        Color[i] = G->vertices[i]->color;
    }
}                                                                  

void ImportarColores(color* Color,Grafo  G){
    // Obtener el número de vértices del grafo
    u32 num_vertices = G->vertices_num;
    
    // Asignar los colores a los vértices del grafo
    for (u32 i = 0; i < num_vertices; i++) {
        G->vertices[i]->color = Color[i];
    }
}
